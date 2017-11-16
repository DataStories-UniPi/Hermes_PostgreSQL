/*
 * Authors: Panagiotis Tampakis (panosbp@gmail.com).
 */

DROP TYPE IF EXISTS global_clustid CASCADE;
CREATE TYPE global_clustid AS (
	chunk_id integer,
	sub_chunk_id integer,
	clust_id integer
);

CREATE OR REPLACE FUNCTION QUT(data_source character varying, ti timestamp without time zone, te timestamp without time zone, non_common_lifespan interval, similarity_threshold double precision, tau_append interval, tau interval, d double precision, gamma double precision, only_repr boolean default true)
RETURNS void AS

$BODY$

declare

row record;
r record;
q record;
i integer;
j integer;
k integer;
if_repr_tbl_exists integer;
prev_te timestamp without time zone;
curr_ti timestamp without time zone;
eq_class  integer;
similarity double precision;
ds1_nof_objs_in_cluster integer;
ds2_nof_objs_in_cluster integer;
common_IDs integer;
n_of_segs integer;
last_seg_id integer;
append_obj_id integer;
append_rs1_traj_id integer;
append_rs1_subtraj_id integer;
append_rs2_traj_id integer;
append_rs2_subtraj_id integer;
check_non_common_lifespan boolean;
merge boolean;
append boolean;
avg_voting double precision;
distance_subtraj double precision;


begin
	
	set enable_seqscan = off;

	execute 'DROP TABLE IF EXISTS '||data_source||'_repr_results';

	execute 'DROP TABLE IF EXISTS '||data_source||'_result_set';
	
	execute 'DROP TABLE IF EXISTS '||data_source||'_originrepr_results';

	execute 'CREATE TEMPORARY TABLE '||data_source||'_repr_results(equivelance_class integer, chunk_id integer, sub_chunk_id integer, obj_id integer, traj_id integer, subtraj_id integer, clust_id global_clustid[], seg_id integer, seg segmentst, CONSTRAINT '||data_source||'_repr_results_pkey PRIMARY KEY (obj_id, traj_id, subtraj_id, seg_id))';
	
	execute 'CREATE TABLE '||data_source||'_originrepr_results(equivelance_class integer, chunk_id integer, sub_chunk_id integer, obj_id integer, traj_id integer, subtraj_id integer, clust_id integer[], seg_id integer, seg segmentst, CONSTRAINT '||data_source||'_repr_results_pkey PRIMARY KEY (obj_id, traj_id, subtraj_id, seg_id))';

	execute 'CREATE INDEX '||data_source||'_repr_results_idx1 ON '||data_source||'_repr_results USING btree(chunk_id, sub_chunk_id, obj_id, traj_id, clust_id)';

	execute 'CREATE INDEX '||data_source||'_repr_results_clust_id_idx ON '||data_source||'_repr_results USING btree(clust_id)';
	
	execute 'CREATE INDEX '||data_source||'_repr_results_eq_class_idx ON '||data_source||'_repr_results USING btree(equivelance_class)';
	
	execute 'CREATE INDEX '||data_source||'_repr_results_seg_idx ON '||data_source||'_repr_results USING gist(seg)';

	execute 'CREATE TABLE '||data_source||'_result_set(chunk_id integer, sub_chunk_id integer, obj_id integer, traj_id integer, subtraj_id integer, clust_id global_clustid[], seg_id integer, seg segmentst, CONSTRAINT '||data_source||'_result_set_pkey PRIMARY KEY (obj_id, traj_id, subtraj_id, clust_id, seg_id))';
	
	--initially filter the chunks that overlap the given period and for each of them the corresponding valid sub-chunks
	for r in execute 'SELECT chunk_id, sub_chunk_id FROM '||data_source||'_sub_chunks_metadata WHERE intersects(sub_chunk_period, period('''||ti||''', '''||te||'''))' loop

		i := r.chunk_id;
		j := r.sub_chunk_id;
		
		execute 'SELECT count(*) FROM pg_tables WHERE tablename = '''||data_source||'_c_'||i||'_sc_'||j||'_repr''' into if_repr_tbl_exists;
		
		if if_repr_tbl_exists > 0 then
		
			execute '
						INSERT INTO '||data_source||'_repr_results 
						(
							SELECT 0, '||i||', '||j||', obj_id, traj_id, subtraj_id, ARRAY[('||i||', '||j||', clust_id)::global_clustid], seg_id, seg
							FROM '||data_source||'_c_'||i||'_sc_'||j||'_repr
							WHERE intersects(period('''||ti||''', '''||te||'''), gett(seg)) AND getti(seg) < '''|| te ||''' AND gette(seg) > '''|| ti ||'''
						)
					';
			
			execute '
						INSERT INTO '||data_source||'_originrepr_results 
						(
							SELECT 0, '||i||', '||j||', obj_id, traj_id, subtraj_id, ARRAY[clust_id], seg_id, seg
							FROM '||data_source||'_c_'||i||'_sc_'||j||'_repr
							WHERE intersects(period('''||ti||''', '''||te||'''), gett(seg)) AND getti(seg) < '''|| te ||''' AND gette(seg) > '''|| ti ||'''
						)
					';

		
		end if;
	RAISE NOTICE 'chunk_id ---> %', i;		
	RAISE NOTICE 'sub_chunk_id ---> %', j;		

	end loop;

	--partitions the sub-chunks in equivalence classes 
	eq_class := 0;
	curr_ti = '1970-01-01 02:00:00';--elaxisth hmeromhnia sthn postgres
	prev_te = '1970-01-01 02:00:00';--elaxisth hmeromhnia sthn postgres
	for r in execute '	SELECT obj_id, traj_id, subtraj_id, clust_id[1].clust_id, min(getti(seg)) min_t, max(gette(seg)) max_t
						FROM '||data_source||'_repr_results 
						GROUP BY obj_id, traj_id, subtraj_id, clust_id[1].clust_id
						ORDER BY min(getti(seg))
						' loop

		curr_ti := r.min_t;

		if (curr_ti - prev_te) > tau then
		
			eq_class := eq_class + 1;
			
		end if;
		
		execute 'UPDATE '||data_source||'_repr_results SET equivelance_class = '||eq_class||' WHERE (obj_id, traj_id, clust_id[1].clust_id) = ('||r.obj_id||', '||r.traj_id||', '||r.clust_id||')';

		prev_te := r.max_t;

	end loop;

	--the algorithm pops each equivalence class one-by-one and sorts all representatives w.r.t. time dimension
	execute 'SELECT count(distinct equivelance_class) FROM '||data_source||'_repr_results ' into eq_class;

	for k in 1..eq_class loop
		
		RAISE NOTICE 'eq_class ---> %', k;		
		
		--sweep in time dimension the temporally interleaved representatives and for each pair of sub-trajectories  it only checks whether the two representatives have either the same lifespan (line 8) or one ends when the next is starting (line 11); w.r.t. the tau threshold
		for q in execute 
							'
								SELECT DISTINCT obj_id, traj_id, subtraj_id 
								FROM '||data_source||'_repr_results 

							' loop
							
			for r in execute 	'	
									SELECT rs1.chunk_id rs1_chunk_id, rs1.sub_chunk_id rs1_sub_chunk_id, rs1.obj_id rs1_obj_id, rs1.traj_id rs1_traj_id, rs1.subtraj_id rs1_subtraj_id, rs1.clust_id rs1_clust_id, rs1.min_seg_id rs1_min_seg_id, rs1.max_seg_id rs1_max_seg_id, rs1.min_ti rs1_min_ti, rs1.max_te rs1_max_te, rs2.chunk_id rs2_chunk_id, rs2.sub_chunk_id rs2_sub_chunk_id, rs2.obj_id rs2_obj_id, rs2.traj_id rs2_traj_id, rs2.subtraj_id rs2_subtraj_id, rs2.clust_id rs2_clust_id, rs2.min_seg_id rs2_min_seg_id, rs2.max_seg_id rs2_max_seg_id, rs2.min_ti rs2_min_ti, rs2.max_te rs2_max_te
									FROM 
										(
											SELECT chunk_id, sub_chunk_id, clust_id[1].clust_id clust_id, obj_id, traj_id, subtraj_id, min(seg_id) min_seg_id, max(seg_id) max_seg_id, min(getti(seg)) min_ti, max(gette(seg)) max_te 
											FROM '||data_source||'_repr_results 
											WHERE (obj_id, traj_id, subtraj_id) = ('||q.obj_id||', '||q.traj_id||', '||q.subtraj_id||')
											GROUP BY chunk_id, sub_chunk_id, clust_id[1].clust_id, obj_id, traj_id, subtraj_id
										) rs1 
										INNER JOIN 
										(
											SELECT chunk_id, sub_chunk_id, clust_id[1].clust_id clust_id, obj_id, traj_id, subtraj_id, min(seg_id) min_seg_id, max(seg_id) max_seg_id, min(getti(seg)) min_ti, max(gette(seg)) max_te, segmentst(min(getti(seg)), least(min(getix(getsp(seg))), min(getex(getsp(seg)))), least(min(getiy(getsp(seg))), min(getey(getsp(seg)))), max(gette(seg)), greatest(max(getix(getsp(seg))), max(getex(getsp(seg)))), greatest(max(getiy(getsp(seg))), max(getey(getsp(seg))))) AS seg
											FROM '||data_source||'_repr_results 
											WHERE (obj_id, traj_id, subtraj_id) IN
																(
																	SELECT rs2.obj_id, rs2.traj_id, rs2.subtraj_id
																	FROM
																		(
																			SELECT equivelance_class, chunk_id, sub_chunk_id, obj_id, traj_id, subtraj_id, min(getti(seg)) min_ti, max(gette(seg)) max_te 
																			FROM '||data_source||'_repr_results
																			WHERE (obj_id, traj_id, subtraj_id) = ('||q.obj_id||', '||q.traj_id||', '||q.subtraj_id||')
																			GROUP BY equivelance_class, chunk_id, sub_chunk_id, obj_id, traj_id, subtraj_id
																		) rs1 
																		INNER JOIN '||data_source||'_repr_results rs2
																		ON rs2.seg && period(rs1.min_ti - '''||non_common_lifespan||'''::interval, rs1.max_te + '''||non_common_lifespan||'''::interval)
																	WHERE (rs1.chunk_id, rs1.sub_chunk_id)<>(rs2.chunk_id, rs2.sub_chunk_id) AND (rs1.equivelance_class, rs2.equivelance_class) = ('||k||', '||k||')
																)
											GROUP BY chunk_id, sub_chunk_id, clust_id[1].clust_id, obj_id, traj_id, subtraj_id
										) rs2 
										ON rs2.seg && period(rs1.min_ti - '''||non_common_lifespan||'''::interval, rs1.max_te + '''||non_common_lifespan||'''::interval)
								' loop
				
				if greatest(r.rs1_min_ti, r.rs2_min_ti) - least(r.rs1_min_ti, r.rs2_min_ti) + greatest(r.rs1_max_te, r.rs2_max_te) - least(r.rs1_max_te, r.rs2_max_te) <  non_common_lifespan then
					
					RAISE NOTICE 'check_non_common_lifespan ---> true';		
					
					execute ' 
								SELECT avg(voting)
								FROM
									(
										SELECT rs1_obj_id, rs1_traj_id, rs1_subtraj_id, rs1_seg_id, rs2_obj_id, rs2_traj_id, rs2_subtraj_id, CASE WHEN exp_input <= 745 THEN exp(-exp_input) ELSE 0 END AS voting
										FROM
												
												(
													SELECT rs1.obj_id rs1_obj_id, rs1.traj_id rs1_traj_id, rs1.subtraj_id rs1_subtraj_id, rs1.seg_id rs1_seg_id, rs2.obj_id rs2_obj_id, rs2.traj_id rs2_traj_id, rs2.subtraj_id rs2_subtraj_id, COALESCE(power(min(distance(getsp(rs1.seg), getsp(rs2.seg))/100), 2.0) / (2.0 * power(500, 2.0))) exp_input
													FROM 
														(
															SELECT obj_id, traj_id, subtraj_id, seg_id, seg
															FROM '||data_source||'_repr_results
															WHERE (obj_id, traj_id, subtraj_id) = ('|| r.rs1_obj_id ||', '|| r.rs1_traj_id ||', '|| r.rs1_subtraj_id ||')
														) rs1,
														(
															SELECT obj_id, traj_id, subtraj_id, seg_id, seg
															FROM '||data_source||'_repr_results
															WHERE (obj_id, traj_id, subtraj_id) = ('|| r.rs2_obj_id ||', '|| r.rs2_traj_id ||', '|| r.rs2_subtraj_id ||')
														) rs2
													GROUP BY rs1.obj_id, rs1.traj_id, rs1.subtraj_id, rs1.seg_id, rs2.obj_id, rs2.traj_id, rs2.subtraj_id
													ORDER BY rs1.obj_id, rs1.traj_id, rs1.subtraj_id, rs1.seg_id, rs2.obj_id, rs2.traj_id, rs2.subtraj_id
												)rs
									) rs
								GROUP BY rs.rs1_obj_id, rs.rs1_traj_id, rs.rs1_subtraj_id
							' into avg_voting
							;
					RAISE NOTICE 'Calculate avg(voting)';
					RAISE NOTICE 'avg_voting --> %', avg_voting;		
					
					if avg_voting > similarity_threshold then
						
						RAISE NOTICE 'merge ---> true';		
						
						execute 
								' 	
									DELETE 
									FROM '||data_source||'_repr_results 
									WHERE (obj_id, traj_id, subtraj_id) = ('|| r.rs2_obj_id ||', '|| r.rs2_traj_id ||', '|| r.rs2_subtraj_id ||')
								';
						
						execute 
								' 	
									UPDATE '||data_source||'_repr_results 
									SET  clust_id = array_append(clust_id, ('|| r.rs2_chunk_id ||', '|| r.rs2_sub_chunk_id ||', '|| r.rs2_clust_id ||')::global_clustid)   
									WHERE (obj_id, traj_id, subtraj_id) = ('|| r.rs1_obj_id ||', '|| r.rs1_traj_id ||', '|| r.rs1_subtraj_id ||')
								';
						
					
					end if;
					
				else
					
					RAISE NOTICE 'merge ---> false';		
					
					if r.rs2_min_ti >= r.rs1_max_te	AND (r.rs2_min_ti - r.rs1_max_te) < tau_append  then
						
						execute 
								'
									SELECT distance(geti(getsp(rs2.seg)), gete(getsp(rs1.seg)))
									FROM '||data_source||'_repr_results rs1, '||data_source||'_repr_results rs2
									WHERE (rs1.obj_id, rs1.traj_id, rs1.subtraj_id, rs1.seg_id) = ('|| r.rs1_obj_id ||', '|| r.rs1_traj_id ||', '|| r.rs1_subtraj_id ||', '|| r.rs1_max_seg_id ||') AND (rs2.obj_id, rs2.traj_id, rs2.subtraj_id, rs2.seg_id) = ('|| r.rs2_obj_id ||', '|| r.rs2_traj_id ||', '|| r.rs2_subtraj_id ||', '|| r.rs2_min_seg_id ||')
								' into distance_subtraj;
						
						RAISE NOTICE 'distance_subtraj --> %', distance_subtraj;		
		
						if distance_subtraj < d then
							
							RAISE NOTICE 'distance_subtraj = %', distance_subtraj;			

							RAISE NOTICE 'append ---> true';		
							
							execute ' 
										SELECT count(distinct ds1.obj_id)
										FROM '||data_source||'_c_'|| r.rs1_chunk_id ||'_sc_'|| r.rs1_sub_chunk_id ||' ds1
										WHERE (ds1.clust_id) = ('|| r.rs1_clust_id ||')
									' into ds1_nof_objs_in_cluster;	
											
							execute ' 
										SELECT count(distinct ds2.obj_id)
										FROM '||data_source||'_c_'|| r.rs2_chunk_id ||'_sc_'|| r.rs2_sub_chunk_id ||' ds2
										WHERE (ds2.clust_id) = ('|| r.rs2_clust_id ||')
									' into ds2_nof_objs_in_cluster;
							
							execute ' 
										SELECT count(distinct ds1.obj_id)
										FROM '||data_source||'_c_'|| r.rs1_chunk_id ||'_sc_'|| r.rs1_sub_chunk_id ||' ds1
											INNER JOIN '||data_source||'_c_'|| r.rs2_chunk_id ||'_sc_'|| r.rs2_sub_chunk_id ||' ds2
												ON ds1.obj_id = ds2.obj_id
										WHERE ds1.clust_id = '|| r.rs1_clust_id ||' AND ds2.clust_id = '|| r.rs2_clust_id ||'
									' into common_IDs;
							
							RAISE NOTICE 'ds1_nof_objs_in_cluster: , %', ds1_nof_objs_in_cluster;
							RAISE NOTICE 'ds2_nof_objs_in_cluster: , %', ds2_nof_objs_in_cluster;
							RAISE NOTICE 'common_IDs: , %', common_IDs;
							
							RAISE NOTICE 'common_IDs/(ds1_nof_objs_in_cluster+ds2_nof_objs_in_cluster-common_IDs) = %', common_IDs/(ds1_nof_objs_in_cluster+ds2_nof_objs_in_cluster-common_IDs)::double precision;			
		
							if 	(common_IDs/(ds1_nof_objs_in_cluster+ds2_nof_objs_in_cluster-common_IDs)::double precision) > gamma then
								
								execute 
										' 	
											UPDATE '||data_source||'_repr_results 
											SET  clust_id = array_append(clust_id, ('|| r.rs2_chunk_id ||', '|| r.rs2_sub_chunk_id ||', '|| r.rs2_clust_id ||')::global_clustid)
											WHERE (obj_id, traj_id, subtraj_id) = ('|| r.rs1_obj_id ||', '|| r.rs1_traj_id ||', '|| r.rs1_subtraj_id ||')
										';
										
								execute 
										' 	
											UPDATE '||data_source||'_repr_results 
											SET  clust_id = (
																SELECT DISTINCT clust_id
																FROM '||data_source||'_repr_results 
																WHERE (obj_id, traj_id, subtraj_id) = ('|| r.rs1_obj_id ||', '|| r.rs1_traj_id ||', '|| r.rs1_subtraj_id ||')
															)
											WHERE (obj_id, traj_id, subtraj_id) = ('|| r.rs2_obj_id ||', '|| r.rs2_traj_id ||', '|| r.rs2_subtraj_id ||')
										';
							
								RAISE NOTICE 'append ---> true2';		
							
							end if;
						
						end if;
					
					end if;
					
					RAISE NOTICE 'append ---> false';		

				end if;

			end loop;
		
		end loop;
		
		execute '	
			INSERT INTO '||data_source||'_result_set (
														SELECT chunk_id, sub_chunk_id, obj_id, traj_id, subtraj_id, clust_id, seg_id, seg
														FROM  '||data_source||'_repr_results
														WHERE equivelance_class = '|| k ||'
													)
		';

		execute '	UPDATE '||data_source||'_repr_results
					SET equivelance_class = '|| k ||' + 1
					WHERE (chunk_id, sub_chunk_id, obj_id, traj_id, subtraj_id, clust_id[1].clust_id) in 
													(
														SELECT rs.chunk_id, rs.sub_chunk_id, rs.obj_id, rs.traj_id, rs.subtraj_id, rs.clust_id[1].clust_id
														FROM '||data_source||'_repr_results rs
															INNER JOIN '||data_source||'_sub_chunks_metadata scm
																ON rs.chunk_id = scm.chunk_id
														WHERE rs.equivelance_class = '|| k ||'
														GROUP BY rs.chunk_id, rs.sub_chunk_id, rs.obj_id, rs.traj_id,  rs.subtraj_id, rs.clust_id[1].clust_id
														HAVING max(gette(rs.seg)) + '''|| tau ||''' > max(gete(sub_chunk_period)) AND (max(gette(rs.seg)) + '''|| tau ||''' <= '''|| te ||''')
													)
				';
		
	end loop;
	
	if only_repr is false then
		
		execute 'DROP TABLE IF EXISTS '||data_source||'_result_set_data';

		execute 'CREATE TABLE '||data_source||'_result_set_data(clust_id global_clustid[], chunk_id integer, sub_chunk_id integer, obj_id integer, traj_id integer, subtraj_id integer, seg_id integer, seg segmentst, CONSTRAINT '||data_source||'__result_set_data_pkey PRIMARY KEY (clust_id, obj_id, traj_id, subtraj_id, seg_id))';

	
		for r in execute '
							SELECT DISTINCT clust_id::text as cluster_id, (UNNEST(clust_id)).chunk_id chunk_id, (UNNEST(clust_id)).sub_chunk_id sub_chunk_id, (UNNEST(clust_id)).clust_id clust_id
							FROM '||data_source||'_result_set
						' loop
			
			RAISE NOTICE 'clust_id: %', r.cluster_id;
			
			execute '
						
						INSERT INTO '||data_source||'_result_set_data
						SELECT '''|| r.cluster_id ||'''::global_clustid[], '|| r.chunk_id ||', '|| r.sub_chunk_id ||', obj_id, traj_id, subtraj_id, seg_id, seg
						FROM '||data_source||'_c_'|| r.chunk_id ||'_sc_'|| r.sub_chunk_id ||'
						WHERE clust_id = '|| r.clust_id ||' AND (obj_id, traj_id, subtraj_id) != (
																										SELECT DISTINCT obj_id, traj_id, subtraj_id
																										FROM '||data_source||'_result_set
																										WHERE clust_id = '''|| r.cluster_id ||'''::global_clustid[] AND chunk_id = '|| r.chunk_id ||' AND sub_chunk_id = '|| r.sub_chunk_id ||'
																									)
						
					';

		end loop;
		
	end if;

end

$BODY$

LANGUAGE plpgsql;


--SELECT QUT('imis1week_subset_1', '2009-07-05 00:00:00', '2012-07-06 00:00:00', '00:30:00', 0.7, '2 hours', '2 hours', 500, 0.5);
