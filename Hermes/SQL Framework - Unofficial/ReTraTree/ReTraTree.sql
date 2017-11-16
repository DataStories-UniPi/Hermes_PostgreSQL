/*
 * Authors: Panagiotis Tampakis (panosbp@gmail.com).
 */

CREATE OR REPLACE FUNCTION ReTraTree(data_source character varying, chunk_lifespan interval, sub_chunk_temporal_tolerance interval, max_size_of_outlier_set bigint, non_common_lifespan interval, similarity_threshold double precision, min_size_of_subtraj integer)
RETURNS void AS

$BODY$

declare

n_of_chunks integer;
n_of_subchunks integer;
n_of_idx integer;
n_of_to_insert integer;
n_of_segs_in_chunk  integer;
chunk_exists boolean;
i integer;
j record;
k record;
n integer;
r integer;
max_data_source_t timestamp without time zone;
min_data_source_t timestamp without time zone;
chunk_ti timestamp without time zone;
chunk_te timestamp without time zone;
sub_chunk_ti timestamp without time zone;
sub_chunk_te timestamp without time zone;
assigned_to_subchunk boolean;
assigned_to_cluster boolean; 
candidate_cluster_id integer;
similarity double precision;
j_obj_id integer;
j_traj_id integer;
j_subtraj_id integer;
j_subtraj_ti timestamp without time zone;
j_subtraj_te timestamp without time zone;
if_tbl_exists integer;
max_subtraj_id integer;
max_cluster_id integer;
relsize integer;
ti timestamp without time zone;
te timestamp without time zone;
n_of_loop integer;
original_datasource character varying;

n_of_outliers_in_subchunk integer;

n_of_subtraj_in_chunk integer;

n_of_segs_in_subchunk integer;

begin
	
	n_of_loop := -1;	
	
	original_datasource :=	data_source;
	
	execute 'DROP TABLE IF EXISTS '||original_datasource||'_temp';
	
	execute '
				CREATE /*TEMPORARY*/ TABLE IF NOT EXISTS '||original_datasource||'_temp
				(
				  obj_id integer NOT NULL,
				  traj_id integer NOT NULL,
				  subtraj_id integer NOT NULL,
				  seg_id integer NOT NULL,
				  seg segmentst NOT NULL,
				  CONSTRAINT '||original_datasource||'_temp_pkey PRIMARY KEY (obj_id, traj_id, subtraj_id, seg_id)
				)
			';
	--CREATE TABLE that holds information about the 2nd level data driven temporal partitioning called sub-chunks
	execute '
				CREATE /*TEMPORARY*/ TABLE IF NOT EXISTS '||original_datasource||'_sub_chunks_metadata
				(
					chunk_id integer, 
					sub_chunk_id integer, 
					sub_chunk_period period,
					max_clust_id integer DEFAULT 0,
					CONSTRAINT '||original_datasource||'_sub_chunks_metadata_pkey PRIMARY KEY (chunk_id, sub_chunk_id)
				)
			';
	
	execute '
				DROP TABLE IF EXISTS '||original_datasource||'_max_subtraj_id
			';
	
	execute '
				CREATE /*TEMPORARY*/ TABLE IF NOT EXISTS '||original_datasource||'_max_subtraj_id
				(
					obj_id integer, 
					traj_id integer, 
					max_subtraj_id integer DEFAULT 1, 
					CONSTRAINT '||original_datasource||'_max_subtraj_id_pkey PRIMARY KEY (obj_id, traj_id)
				)
			';
	execute '
				TRUNCATE  '||original_datasource||'_max_subtraj_id 
				
			';
	execute '
				INSERT INTO '||original_datasource||'_max_subtraj_id (obj_id, traj_id) (SELECT obj_id, traj_id FROM '||original_datasource||'_traj)
				
			';
	
	execute '
				CREATE TABLE IF NOT EXISTS '||original_datasource||'_outliers_archive_seg 
				(
					obj_id integer NOT NULL, 
					traj_id integer NOT NULL,
					subtraj_id integer NOT NULL, 
					seg_id integer NOT NULL, 
					seg segmentst, 
					normalized_voting double precision,
					normalized_lifespan interval,
					CONSTRAINT '||original_datasource||'_outliers_archive_seg_pkey 
					PRIMARY KEY (obj_id, traj_id, subtraj_id, seg_id)
				)';
				
	execute '	
				SELECT count(*) 
				FROM pg_class 
				WHERE relname = '''||original_datasource||'_outliers_archive_seg_idx''
			' into n_of_idx;
	
	--if they dont exist then create them
	if n_of_idx = 0 then 
	
		execute 'CREATE INDEX '||original_datasource||'_outliers_archive_seg_idx ON '||original_datasource||'_outliers_archive_seg using gist(seg)';
	
	end if;
	
	--SELECT the minimum and maximum time of the dataset in order to calculate the number of chunks (given a chunk lifespan) and assign it to the min_data_source_t variable
	execute '	
				SELECT min(getti(seg)), max(gette(seg))  
				FROM '||original_datasource||'_seg
			' into min_data_source_t, max_data_source_t;

	--calculate the number of chunks, given a chunk lifespan
	n_of_chunks := ceil(extract(epoch FROM(max_data_source_t - min_data_source_t))/extract(epoch FROM(chunk_lifespan)));
	
	loop 
		
		n_of_loop := n_of_loop + 1;
		
		RAISE NOTICE 'n_of_loop ---> %', n_of_loop;	
		
		if n_of_loop > 0 then
		
			data_source := original_datasource||'_'|| n_of_loop;
			
		end if;
		
		if n_of_loop > 1 then
		
			execute 'DROP TABLE IF EXISTS '||original_datasource||'_'|| n_of_loop-1||'_seg';
			
		end if;
		
		execute '	
					SELECT count(*) 
					FROM pg_tables 
					WHERE tablename = '''||data_source||'_seg''
				' into if_tbl_exists;
		
		if if_tbl_exists > 0 then
		
			execute '
					SELECT count(*)
					FROM '||data_source||'_seg
					' into n_of_to_insert;
						
		else 
			
			n_of_to_insert := 0;
		
		end if;
		
		if n_of_to_insert > 0 then
		
			if n_of_loop > 0 then
			
				execute 'CREATE INDEX '|| data_source ||'_seg_idx ON '|| data_source ||'_seg USING gist(seg);';
				
			end if;
			
			--for every chunk
			for i in 1..n_of_chunks loop
			
				RAISE NOTICE 'chunk ---> %', i;		
				
					--calculate the minimum and the maxinum time of the chunk
					chunk_ti := min_data_source_t + chunk_lifespan * (i-1);
					chunk_te := min_data_source_t + chunk_lifespan * (i);
				
				execute '
							SELECT count(*) 
							FROM '||data_source||'_seg
							WHERE getti(seg) >= '''|| chunk_ti ||''' AND gette(seg) <= '''|| chunk_te ||''' AND getti(seg) < '''|| chunk_te ||''' AND gette(seg) > '''|| chunk_ti ||'''				
						' into n_of_segs_in_chunk;
							
				RAISE NOTICE 'n_of_segs_in_chunk ---> %', n_of_segs_in_chunk;		
				
				if n_of_segs_in_chunk > 0 then
				
					n_of_subtraj_in_chunk = 0;
					
					for j in execute '	
										SELECT obj_id, traj_id, subtraj_id, min(getti(seg)) sub_traj_ti, max(gette(seg)) sub_traj_te 
										FROM
											(
												SELECT obj_id, traj_id, subtraj_id, seg_id, seg
												FROM '||data_source||'_seg
												WHERE intersects(period('''||chunk_ti||''', '''||chunk_te||'''), gett(seg)) AND getti(seg) < '''|| chunk_te ||''' AND gette(seg) > '''|| chunk_ti ||'''
												ORDER BY obj_id, traj_id, subtraj_id, seg_id
											) r1
										GROUP BY obj_id, traj_id, subtraj_id
									' loop

						assigned_to_subchunk := FALSE;
						assigned_to_cluster := FALSE;
						j_obj_id := j.obj_id;
						j_traj_id := j.traj_id;
						j_subtraj_id := j.subtraj_id;
						j_subtraj_ti := j.sub_traj_ti;
						j_subtraj_te := j.sub_traj_te;
						
						n_of_subtraj_in_chunk := n_of_subtraj_in_chunk + 1;
						RAISE NOTICE 'n_of_subtraj_in_chunk ---> %', n_of_subtraj_in_chunk;		

						--calculate number of subchunks for the current chunk and assign it to the n_of_subchunks variable
						execute '	
									SELECT count(sub_chunk_id) 
									FROM '||original_datasource||'_sub_chunks_metadata 
									WHERE chunk_id = '||i||'
								' into n_of_subchunks;
						
						--for every sub-chunk of the current chunk
						for n in 1..n_of_subchunks loop
							
							RAISE NOTICE 'n_of_subchunks ---> %', n_of_subchunks;		
							
							--calculate the minimum and maximum time of the current sub-chunk and assign it to the sub_chunk_ti, sub_chunk_te variables
							execute '	
										SELECT geti(sub_chunk_period), gete(sub_chunk_period) 
										FROM '||original_datasource||'_sub_chunks_metadata 
										WHERE (chunk_id, sub_chunk_id) = ('||i||', '||n||')
									' into sub_chunk_ti, sub_chunk_te;
							
							RAISE NOTICE 'sub_chunk_ti ---> %', sub_chunk_ti;
							RAISE NOTICE 'sub_chunk_te ---> %', sub_chunk_te;
							
							--if the current sub-trajectory "fits" to an already existing sub-chunk with respect to the sub_chunk_temporal_tolerance
							if (abs(extract(epoch FROM(j_subtraj_ti - sub_chunk_ti))) < extract(epoch FROM(sub_chunk_temporal_tolerance))/2) and (abs(extract(epoch FROM(j_subtraj_te - sub_chunk_te))) < extract(epoch FROM(sub_chunk_temporal_tolerance))/2) then 
								
								-- set the assigned_to_subchunk variable TRUE
								assigned_to_subchunk := TRUE;
								
								--then check if there exists a representative table for the current sub-schunk
								execute '	
											SELECT count(*) 
											FROM pg_tables 
											WHERE tablename = '''||original_datasource||'_c_'||i||'_sc_'||n||'_repr''
										' into if_tbl_exists;
								
								--if there exists a representative table for the current sub-schunk
								if if_tbl_exists > 0 then
								
									similarity := 0;
									
									--find which representative is more similar with the current sub-trajectory and their similarity and assign them to the candidate_cluster_id and similarity variables respectively
									execute  '	
												SELECT clust_id, avg(voting)
												FROM
													(	
														SELECT obj_id, traj_id, subtraj_id, clust_id, seg, CASE WHEN exp_input <= 745 THEN exp(-exp_input) ELSE 0 END AS voting
														FROM
															(
																SELECT repr.obj_id, repr.traj_id, repr.subtraj_id, repr.seg_id, repr.clust_id, repr.seg, COALESCE(power(min(distance(getsp(repr.seg), getsp(ch.seg))), 2.0) / (2.0 * power(1000, 2.0))) exp_input
																FROM 
																	(
																		SELECT obj_id, traj_id, subtraj_id, seg_id, seg 
																		FROM '||data_source||'_seg
																		WHERE (obj_id, traj_id, subtraj_id) = ('||j_obj_id||', '||j_traj_id||', '||j_subtraj_id||') AND intersects(period('''||sub_chunk_ti||''', '''||sub_chunk_te||'''), gett(seg)) AND getti(seg) < '''|| sub_chunk_te ||''' AND gette(seg) > '''|| sub_chunk_ti ||'''
																	) ch
																	INNER JOIN '||original_datasource||'_c_'||i||'_sc_'||n||'_repr repr
																		ON (repr.obj_id, repr.traj_id, repr.subtraj_id) <> ('||j_obj_id||', '||j_traj_id||', '||j_subtraj_id||')
																GROUP BY repr.obj_id, repr.traj_id, repr.subtraj_id, repr.seg_id, repr.clust_id
															) nested_tbl
													) nested_tbl
												GROUP BY obj_id, traj_id, subtraj_id, clust_id
												HAVING ( max(max(gette(seg)), '''||j_subtraj_te||'''::timestamp without time zone) - min(max(gette(seg)), '''||j_subtraj_te||'''::timestamp without time zone) ) + ( max(min(getti(seg)), '''||j_subtraj_ti||'''::timestamp without time zone) - min(min(getti(seg)), '''||j_subtraj_ti||'''::timestamp without time zone) ) < '''||non_common_lifespan||'''::interval
												ORDER BY avg(voting) DESC
												LIMIT 1
											' into candidate_cluster_id, similarity;
												
									--if their similarity is above a similarity_threshold
									if (similarity > similarity_threshold) and (similarity > 0) then
									
							
										--then assign the current sub-trajectory to the candidate_cluster_id
											
										execute '	
													INSERT INTO '||original_datasource||'_c_'||i||'_sc_'||n||'(obj_id, traj_id, subtraj_id, seg_id, clust_id, seg) 
													(
														SELECT obj_id, traj_id, subtraj_id, seg_id, '||candidate_cluster_id||', seg
														FROM '||data_source||'_seg
														WHERE (obj_id, traj_id, subtraj_id) = ('||j_obj_id||', '||j_traj_id||', '||j_subtraj_id||') AND intersects(period('''||sub_chunk_ti||''', '''||sub_chunk_te||'''), gett(seg)) AND getti(seg) < '''|| sub_chunk_te ||''' AND gette(seg) > '''|| sub_chunk_ti ||'''
														ORDER BY obj_id, traj_id, subtraj_id, seg_id
													)
												';
													
										assigned_to_cluster := TRUE;
									
									end if;
											
								end if;
								
								--if a representative table does not exist or the current sub-trajectory could not be assigned to any of the existing clusters
								if assigned_to_cluster = FALSE then
									
									-- then create the outliers table for the current chunk and sub-chunk if it doesnt already exist
									execute '
												CREATE TABLE IF NOT EXISTS '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg 
												(
													obj_id integer NOT NULL, 
													traj_id integer NOT NULL,
													subtraj_id integer NOT NULL, 
													seg_id integer NOT NULL, 
													seg segmentst, 
													normalized_voting double precision,
													normalized_lifespan interval,
													CONSTRAINT '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg_pkey 
													PRIMARY KEY (obj_id, traj_id, subtraj_id, seg_id)
												)';

										
									execute '
											INSERT INTO '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg(obj_id, traj_id, subtraj_id, seg_id, seg) 
													(
														SELECT obj_id, traj_id, subtraj_id, seg_id, seg
														FROM '||data_source||'_seg r
														WHERE (obj_id, traj_id, subtraj_id) = ('||j_obj_id||', '||j_traj_id||', '||j_subtraj_id||') AND intersects(period('''||sub_chunk_ti||''', '''||sub_chunk_te||'''), gett(seg)) AND getti(seg) < '''|| sub_chunk_te ||''' AND gette(seg) > '''|| sub_chunk_ti ||'''
														ORDER BY obj_id, traj_id, subtraj_id, seg_id
													)
											';
												
									execute '	
												SELECT pg_relation_size(oid) 
												FROM pg_class
												WHERE relname = '''||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg''
											' into relsize;
									
									execute '	
												SELECT count(*) 
												FROM 
													(
														SELECT distinct obj_id, traj_id, subtraj_id
														FROM '||data_source||'_c_'||i||'_sc_'||n||'_outliers_seg
													) n_of_subtraj
											' into n_of_outliers_in_subchunk;
											
									RAISE NOTICE 'n_of_outliers_in_subchunk = %', n_of_outliers_in_subchunk;
									
									--if the number of outliers exceeds the max_n_of_outliers_in_subchunk threshold
									if n_of_outliers_in_subchunk >= max_size_of_outlier_set then

										
										--check if the appropriate indexes exist
										execute '	
													SELECT count(*) 
													FROM pg_class 
													WHERE relname = '''||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg_idx''
												' into n_of_idx;
										
										--if they dont exist then create them
										if n_of_idx = 0 then 
										
											execute 'CREATE INDEX '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg_idx ON '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg using gist(seg)';
										
										end if;

									
										execute '
													CREATE TABLE IF NOT EXISTS '||original_datasource||'_c_'||i||'_sc_'||n||'
														(
															obj_id integer NOT NULL, 
															traj_id integer NOT NULL, 
															subtraj_id integer NOT NULL, 
															seg_id integer NOT NULL, 
															clust_id integer NOT NULL, 
															seg segmentst, 
															CONSTRAINT '||original_datasource||'_c_'||i||'_sc_'||n||'_pkey PRIMARY KEY (obj_id, traj_id, subtraj_id, seg_id)
														)
												';
										
										execute '	
													SELECT count(*) 
													FROM pg_class 
													WHERE relname = '''||original_datasource||'_c_'||i||'_sc_'||n||'_seg_idx''
												' into n_of_idx;
										
										if n_of_idx = 0 then 
										
											execute 'CREATE INDEX '||original_datasource||'_c_'||i||'_sc_'||n||'_seg_idx ON '||original_datasource||'_c_'||i||'_sc_'||n||' using btree(clust_id)';
											
										end if;
										
										execute '
													CREATE /*TEMPORARY*/ TABLE IF NOT EXISTS '||original_datasource||'_c_'||i||'_sc_'||n||'_repr
														(
															obj_id integer NOT NULL, 
															traj_id integer NOT NULL, 
															subtraj_id integer NOT NULL, 
															seg_id integer NOT NULL, 
															clust_id integer NOT NULL, 
															seg segmentst,
															CONSTRAINT '||original_datasource||'_c_'||i||'_sc_'||n||'_repr_pkey PRIMARY KEY (obj_id, traj_id, subtraj_id, seg_id, clust_id)
														)
												'; 

										SET enable_seqscan = off;

										DROP TABLE IF EXISTS S2T_Parameters CASCADE;
										CREATE TEMPORARY TABLE S2T_Parameters (
											key text NOT NULL,
											value text NOT NULL,

											PRIMARY KEY (key)
										);

										INSERT INTO S2T_Parameters(key, value) VALUES ('data_source', ''||original_datasource||'');
										INSERT INTO S2T_Parameters(key, value) VALUES ('D', ''||original_datasource||'_c_'||i||'_sc_'||n||'_outliers');

										INSERT INTO S2T_Parameters(key, value) VALUES ('NN_method', 'Hermes A'); --Hermes, Hermes A, Hermes B, PostGIS A, PostGIS B

										INSERT INTO S2T_Parameters(key, value) VALUES ('temporal_buffer_size', '0 seconds');

										execute '
													INSERT INTO S2T_Parameters(key, value) 
													SELECT ''spatial_buffer_size'', round(sqrt(-2*(length(SegmentSP(lx,ly, hx,hy)) * 0.001)^2*ln(0.001)))::integer
													FROM hdatasets_statistics
													WHERE dataset = HDatasetID('''||original_datasource||''')
												';
										

										execute '
													INSERT INTO S2T_Parameters(key, value) -- w expresses the minimum number of segments that can define a subtrajectory
													SELECT ''w'', round((avg_trajectory_points + 1) * 0.2)::integer
													FROM hdatasets_statistics
													WHERE dataset = HDatasetID('''||original_datasource||''')
												';
										
										
										INSERT INTO S2T_Parameters(key, value) VALUES ('tau', 0.01); -- as tau increases, the number of subtrajectories reduces

										INSERT INTO S2T_Parameters(key, value)
											SELECT 'temporal_buffer_size_ssa', value
											FROM S2T_Parameters
											WHERE key = 'temporal_buffer_size';
										INSERT INTO S2T_Parameters(key, value)
											SELECT 'spatial_buffer_size_ssa', value
											FROM S2T_Parameters
											WHERE key = 'spatial_buffer_size';
										
										INSERT INTO S2T_Parameters(key, value) VALUES ('M', 1000); -- how many samples, therefore how many clusters
										INSERT INTO S2T_Parameters(key, value) VALUES ('eps_ssa', 0.01); -- a positive number near to zero so that we don't put into the sample subtrajectories with small representativeness

										INSERT INTO S2T_Parameters(key, value)
											SELECT 'temporal_buffer_size_sca', value
											FROM S2T_Parameters
											WHERE key = 'temporal_buffer_size';
										INSERT INTO S2T_Parameters(key, value)
											SELECT 'spatial_buffer_size_sca', value
											FROM S2T_Parameters
											WHERE key = 'spatial_buffer_size';
										
										INSERT INTO S2T_Parameters(key, value) VALUES ('eps_sca', 0.85);


										PERFORM S2T_VotingMethod('Trapezoidal'); --Count, Trapezoidal
										PERFORM S2T_Sigma(length(SegmentSP(lx,ly, hx,hy)) * 0.01) FROM hdatasets_statistics WHERE dataset = HDatasetID('''||original_datasource||''');
										PERFORM S2T_Preprocessing(original_datasource, i, n);
											
										
										PERFORM S2T_Clustering();

										
										--PERFORM S2T_Postprocessing(data_source, i, n);
										execute 'ANALYZE '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg';

										execute 'ANALYZE '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg_sample';
										execute 'ANALYZE '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg_voting';
										execute 'ANALYZE '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_subtraj';
														--insert the representatives of the current chunk and sub-chunk to the appropriate table AND insert only those representatives that are different FROM the already existing representatives
										
										execute 'SELECT max_clust_id FROM '||original_datasource||'_sub_chunks_metadata WHERE chunk_id = '|| i ||' AND sub_chunk_id = '|| n ||'' into max_cluster_id;
										
										execute '	
													UPDATE '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_subtraj r
													SET clust_id = '|| max_cluster_id ||' + r.clust_id
													WHERE clust_id IS NOT NULL
													
												';
										
										execute '	
													UPDATE '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg_voting r
													SET clust_id = '|| max_cluster_id ||' + r.clust_id
													WHERE clust_id IS NOT NULL
												';
										
										execute '	
													UPDATE '||original_datasource||'_sub_chunks_metadata 
													SET max_clust_id = r1.max_clust_id 
													FROM
														(
															SELECT max(clust_id) max_clust_id
															FROM '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_subtraj
														) r1
													WHERE chunk_id = '|| i ||' AND sub_chunk_id = '|| n ||'
												';
												
										execute '	
													UPDATE '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg r
													SET subtraj_id = r1.subtraj_id
													FROM '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_subtraj r1 
													WHERE (r.obj_id, r.traj_id) = (r1.obj_id, r1.traj_id) AND (r.seg_id BETWEEN r1.first_seg_id AND (r1.first_seg_id + r1.NrSegments - 1)) AND r1.subtraj_sample = FALSE
												';
										
										execute '	
													UPDATE '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg_sample r
													SET subtraj_id = r1.subtraj_id, clust_id = 	r1.clust_id
													FROM '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_subtraj r1 
													WHERE (r.obj_id, r.traj_id) = (r1.obj_id, r1.traj_id) AND (r.seg_id BETWEEN r1.first_seg_id AND (r1.first_seg_id + r1.NrSegments - 1)) AND r1.subtraj_sample = TRUE
												';
										
										execute '
													INSERT INTO '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg(obj_id, traj_id, subtraj_id, seg_id, seg)
													SELECT obj_id, traj_id, subtraj_id, seg_id, seg FROM '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg_sample';
										
										execute ' ANALYZE '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg';
										
												
										
										
										for k in execute 	'
																SELECT r1.obj_id, r1.traj_id, r1.subtraj_id, r1.clust_id, min(getti(r1.seg)) sub_traj_ti, max(gette(r1.seg)) sub_traj_te
																FROM '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg_sample r1
																GROUP BY r1.obj_id, r1.traj_id, r1.subtraj_id, r1.clust_id
															' loop
													
											similarity = 0;

											execute  '	
												SELECT clust_id, avg(voting)
												FROM
													(	
														SELECT obj_id, traj_id, subtraj_id, clust_id, seg, CASE WHEN exp_input <= 745 THEN exp(-exp_input) ELSE 0 END AS voting
														FROM
															(
																SELECT repr.obj_id, repr.traj_id, repr.subtraj_id, repr.seg_id, repr.clust_id, repr.seg, COALESCE(power(min(distance(getsp(repr.seg), getsp(new_repr.seg))), 2.0) / (2.0 * power(1000, 2.0))) exp_input
																FROM 
																	'||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg_sample new_repr 
																			INNER JOIN '||original_datasource||'_c_'||i||'_sc_'||n||'_repr repr
																				ON (new_repr.obj_id, new_repr.traj_id, new_repr.subtraj_id) = (repr.obj_id, repr.traj_id, repr.subtraj_id)
																WHERE (new_repr.obj_id, new_repr.traj_id, new_repr.subtraj_id) = ('||k.obj_id||', '||k.traj_id||', '||k.subtraj_id||')
																GROUP BY repr.obj_id, repr.traj_id, repr.subtraj_id, repr.clust_id, repr.seg_id, repr.seg
															) nested_tbl
													) nested_tbl
												GROUP BY obj_id, traj_id, subtraj_id, clust_id
												HAVING  (max(max(gette(seg)), '''||k.sub_traj_te||''') - min(max(gette(seg)), '''||k.sub_traj_te||''') ) + (max(min(getti(seg)), '''||k.sub_traj_ti||''') - min(min(getti(seg)), '''||k.sub_traj_ti||''')) < '''||non_common_lifespan||'''
												ORDER BY avg(voting) DESC
												LIMIT 1
											' into candidate_cluster_id, similarity;

											if similarity IS NULL then
										
												similarity := 0;

											end if;
											
											RAISE NOTICE 'similarity ---> %', similarity;	
	
											if (similarity < similarity_threshold) then
											
												--add representative in representative set
												execute '	
															INSERT INTO '||original_datasource||'_c_'||i||'_sc_'||n||'_repr(obj_id, traj_id, subtraj_id, seg_id, clust_id, seg)
															(
																SELECT r1.obj_id, r1.traj_id, r1.subtraj_id, r1.seg_id, r1.clust_id, r1.seg
																FROM '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg_sample r1
																WHERE (r1.obj_id, r1.traj_id, r1.subtraj_id) = ('||k.obj_id||', '||k.traj_id||', '||k.subtraj_id||')
															)
														';
												
																								
												execute 'SELECT COUNT(*) FROM 	'||original_datasource||'_c_'||i||'_sc_'||n||'' INTO n_of_segs_in_subchunk;
												
												RAISE NOTICE 'n_of_segs_in_subchunk 1 ---> %', n_of_segs_in_subchunk;		

												
												execute '	
															INSERT INTO '||original_datasource||'_c_'||i||'_sc_'||n||'(obj_id, traj_id, subtraj_id, seg_id, clust_id, seg)
															(
																SELECT r1.obj_id, r1.traj_id, r1.subtraj_id, r1.seg_id, r1.clust_id, r1.seg
																FROM '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg_sample r1
																WHERE (r1.obj_id, r1.traj_id, r1.subtraj_id) = ('||k.obj_id||', '||k.traj_id||', '||k.subtraj_id||')
															)
														';
												
																								
												execute 'SELECT COUNT(*) FROM 	'||original_datasource||'_c_'||i||'_sc_'||n||'' INTO n_of_segs_in_subchunk;
												
												RAISE NOTICE 'n_of_segs_in_subchunk 2 ---> %', n_of_segs_in_subchunk;		
												
												execute '	
															INSERT INTO '||original_datasource||'_c_'||i||'_sc_'||n||'(obj_id, traj_id, subtraj_id, seg_id, clust_id, seg)
															(
																SELECT r1.obj_id, r1.traj_id, r1.subtraj_id, r1.seg_id, r2.clust_id, r1.seg
																FROM '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg r1
																	INNER JOIN '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_subtraj r2
																		ON (r1.obj_id, r1.traj_id) = (r2.obj_id, r2.traj_id) AND (r1.seg_id BETWEEN r2.first_seg_id AND (r2.first_seg_id + r2.NrSegments - 1))
																WHERE r2.clust_id = '||k.clust_id||' AND r2.subtraj_sample = FALSE
															)
														';
												execute 'SELECT COUNT(*) FROM 	'||original_datasource||'_c_'||i||'_sc_'||n||'' INTO n_of_segs_in_subchunk;
												
												RAISE NOTICE 'n_of_segs_in_subchunk 3 ---> %', n_of_segs_in_subchunk;	
												
											else			
														
												--add representative and subtrajectories that belong to the current cluster in subchunk set
												execute '	
															INSERT INTO '||original_datasource||'_c_'||i||'_sc_'||n||'(obj_id, traj_id, subtraj_id, seg_id, clust_id, seg)
															(
																SELECT r1.obj_id, r1.traj_id, r1.subtraj_id, r1.seg_id, '|| candidate_cluster_id ||', r1.seg
																FROM '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg r1
																	INNER JOIN '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_subtraj r2
																		ON (r1.obj_id, r1.traj_id) = (r2.obj_id, r2.traj_id) AND (r1.seg_id BETWEEN r2.first_seg_id AND (r2.first_seg_id + r2.NrSegments - 1))
																WHERE r2.clust_id = '||k.clust_id||'
															)
														';
														
												execute 'SELECT COUNT(*) FROM 	'||original_datasource||'_c_'||i||'_sc_'||n||'' INTO n_of_segs_in_subchunk;
												
												RAISE NOTICE 'n_of_segs_in_subchunk 4 ---> %', n_of_segs_in_subchunk;	

											end if;		

										end loop;
										--remove not clustered trajectories FROM the outliers set that have less than min_size_of_subtraj segments
										--re-drop the remaining not clustered outliers from the top of the structure only if there is the chance to end up in a different subchunk

										--delete trajectories that belong to the current cluster from subchunk outliers set
										execute '	
													DELETE 
													FROM '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg
													WHERE (obj_id, traj_id, subtraj_id) IN (
																								SELECT obj_id, traj_id, subtraj_id 
																								FROM '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_subtraj 
																								WHERE clust_id IS NOT NULL
																							)
												';
											
										execute '	
													INSERT INTO  '||original_datasource||'_outliers_archive_seg
													SELECT * FROM '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg 
													WHERE (obj_id, traj_id, subtraj_id) IN (
																								SELECT obj_id, traj_id, subtraj_id 
																								FROM '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_subtraj 
																								WHERE NrSegments <= '||min_size_of_subtraj||'
																							)
												';
										
										execute '	
													DELETE 
													FROM '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg 
													WHERE (obj_id, traj_id, subtraj_id) IN (
																								SELECT obj_id, traj_id, subtraj_id 
																								FROM '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_subtraj 
																								WHERE NrSegments <= '||min_size_of_subtraj||'
																							)
												';
												
										execute '
													CREATE /*TEMPORARY*/ TABLE IF NOT EXISTS '||original_datasource||'_'||n_of_loop+1||'_seg
													(
													  obj_id integer NOT NULL,
													  traj_id integer NOT NULL,
													  subtraj_id integer NOT NULL,
													  seg_id integer NOT NULL,
													  seg segmentst NOT NULL,
													  CONSTRAINT '||original_datasource||'_'||n_of_loop+1||'_seg_pkey PRIMARY KEY (obj_id, traj_id, subtraj_id, seg_id)
													)
												';
										
										execute '	INSERT INTO  '||original_datasource||'_'|| n_of_loop + 1 ||'_seg
													SELECT obj_id, traj_id, subtraj_id, seg_id, seg
													FROM '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg
													WHERE (obj_id, traj_id, subtraj_id) IN (
																					SELECT obj_id, traj_id, subtraj_id
																					FROM '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg
																					GROUP BY obj_id, traj_id, subtraj_id
																					HAVING NOT ((abs(extract(epoch FROM(min(getti(seg))::timestamp without time zone - '''|| sub_chunk_ti ||'''::timestamp without time zone))) <= extract(epoch FROM('''|| sub_chunk_temporal_tolerance ||'''::interval))/2) AND (abs(extract(epoch FROM(max(gette(seg)::timestamp without time zone) - '''|| sub_chunk_te ||'''::timestamp without time zone))) <= extract(epoch FROM('''|| sub_chunk_temporal_tolerance ||'''::interval))/2))
																					)
												';
										
										
										execute '	INSERT INTO '||original_datasource||'_temp
													SELECT obj_id, traj_id, subtraj_id, seg_id, seg
													FROM '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg
													WHERE (obj_id, traj_id, subtraj_id) IN (
																					SELECT obj_id, traj_id, subtraj_id
																					FROM '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg
																					GROUP BY obj_id, traj_id, subtraj_id
																					HAVING ((abs(extract(epoch FROM(min(getti(seg))::timestamp without time zone - '''|| sub_chunk_ti ||'''::timestamp without time zone))) <= extract(epoch FROM('''|| sub_chunk_temporal_tolerance ||'''::interval))/2) AND (abs(extract(epoch FROM(max(gette(seg)::timestamp without time zone) - '''|| sub_chunk_te ||'''::timestamp without time zone))) <= extract(epoch FROM('''|| sub_chunk_temporal_tolerance ||'''::interval))/2))
																						)
												';
										
										execute 'TRUNCATE '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg';
										execute 'TRUNCATE '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg_voting';
										execute 'TRUNCATE '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_subtraj';
										execute 'TRUNCATE '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg_sample';
										
										execute '	INSERT INTO  '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg
													SELECT *
													FROM '||original_datasource||'_temp
												';
										
										execute 'TRUNCATE '||original_datasource||'_temp';
									
									
									end if;
									
								end if;
								
								EXIT;
								
							end if;
							
						end loop;
						
					
						--if there is not a matching sub-chunk for the current sub-trajectory and is not assigned to a sub-chunk add it into the outlier set
						if assigned_to_subchunk = FALSE then 
							
							--a new subchunk is created by inserting a new record to the sub-chunks metadata table
							execute '	
										INSERT INTO '||original_datasource||'_sub_chunks_metadata(chunk_id, sub_chunk_id, sub_chunk_period) 
										VALUES ('||i||', '||n_of_subchunks+1||', period('''||j_subtraj_ti||'''::timestamp without time zone, '''||j_subtraj_te||'''::timestamp without time zone))
									';
									
							--which is initialized with an outliers set O including the unmatched sub-trajectory, by creating the appropriate table, indexes and inserting the unmatched trajectory to that table
							execute '
										CREATE TABLE '||original_datasource||'_c_'||i||'_sc_'||n_of_subchunks+1||'_outliers_seg
										(
											obj_id integer NOT NULL, 
											traj_id integer NOT NULL,
											subtraj_id integer NOT NULL, 
											seg_id integer NOT NULL, 
											seg segmentst, 
											normalized_voting double precision,
											normalized_lifespan interval,
											CONSTRAINT '||original_datasource||'_c_'||i||'_sc_'||n_of_subchunks+1||'_outliers_seg_pkey PRIMARY KEY (obj_id, traj_id, subtraj_id, seg_id)
										)
									';
									
							execute '	
										INSERT INTO '||original_datasource||'_c_'||i||'_sc_'||n_of_subchunks+1||'_outliers_seg(obj_id, traj_id, subtraj_id, seg_id, seg) 
										(
											SELECT obj_id, traj_id, subtraj_id, seg_id, seg
											FROM '||data_source||'_seg
											WHERE (obj_id, traj_id, subtraj_id) = ('||j_obj_id||', '||j_traj_id||', '||j_subtraj_id||') AND intersects(period('''||chunk_ti||''', '''||chunk_te||'''), gett(seg)) AND getti(seg) < '''|| chunk_te ||''' AND gette(seg) > '''|| chunk_ti ||'''
											ORDER BY obj_id, traj_id, subtraj_id, seg_id
										)
									';
							--UPDATE to_insert flag to FALSE for every segment that was insert into the chunk except the ones that are  getti(seg) < chunk_te < gette(seg) because we need them for the next chunk
							
						
						end if;
						
					end loop;
					
				end if;
			
			end loop;
		else

			for i in 1..n_of_chunks loop
			
				execute '	
							SELECT count(sub_chunk_id) 
							FROM '||original_datasource||'_sub_chunks_metadata 
							WHERE chunk_id = '||i||'
						' into n_of_subchunks;
			
				for n in 1..n_of_subchunks loop
				
					RAISE NOTICE 'chunk ---> %', i;	
					RAISE NOTICE 'subchunk ---> %', n;		

					execute '	
								SELECT count(*) 
								FROM pg_tables 
								WHERE tablename = '''||original_datasource||'_c_'||i||'_sc_'||n||'_repr''
							' into if_tbl_exists;
					if if_tbl_exists = 0 then
				
						--check if the appropriate indexes exist
						execute '	
									SELECT count(*) 
									FROM pg_class 
									WHERE relname = '''||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg_idx''
								' into n_of_idx;
						
						--if they dont exist then create them
						if n_of_idx = 0 then 
						
							execute 'CREATE INDEX '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg_idx ON '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg using gist(seg)';
						
						end if;

					
						execute '
									CREATE TABLE IF NOT EXISTS '||original_datasource||'_c_'||i||'_sc_'||n||'
										(
											obj_id integer NOT NULL, 
											traj_id integer NOT NULL, 
											subtraj_id integer NOT NULL, 
											seg_id integer NOT NULL, 
											clust_id integer NOT NULL, 
											seg segmentst, 
											CONSTRAINT '||original_datasource||'_c_'||i||'_sc_'||n||'_pkey PRIMARY KEY (obj_id, traj_id, subtraj_id, seg_id)
										)
								';
						
						execute '	
									SELECT count(*) 
									FROM pg_class 
									WHERE relname = '''||original_datasource||'_c_'||i||'_sc_'||n||'_seg_idx''
								' into n_of_idx;
						
						if n_of_idx = 0 then 
						
							execute 'CREATE INDEX '||original_datasource||'_c_'||i||'_sc_'||n||'_seg_idx ON '||original_datasource||'_c_'||i||'_sc_'||n||' using btree(clust_id)';
							
						end if;
						
						execute '
									CREATE /*TEMPORARY*/ TABLE IF NOT EXISTS '||original_datasource||'_c_'||i||'_sc_'||n||'_repr
										(
											obj_id integer NOT NULL, 
											traj_id integer NOT NULL, 
											subtraj_id integer NOT NULL, 
											seg_id integer NOT NULL, 
											clust_id integer NOT NULL, 
											seg segmentst,
											CONSTRAINT '||original_datasource||'_c_'||i||'_sc_'||n||'_repr_pkey PRIMARY KEY (obj_id, traj_id, subtraj_id, seg_id, clust_id)
										)
								'; 

						SET enable_seqscan = off;

						DROP TABLE IF EXISTS S2T_Parameters CASCADE;
						CREATE TEMPORARY TABLE S2T_Parameters (
							key text NOT NULL,
							value text NOT NULL,

							PRIMARY KEY (key)
						);

						INSERT INTO S2T_Parameters(key, value) VALUES ('data_source', ''||original_datasource||'');
						INSERT INTO S2T_Parameters(key, value) VALUES ('D', ''||original_datasource||'_c_'||i||'_sc_'||n||'_outliers');

						INSERT INTO S2T_Parameters(key, value) VALUES ('NN_method', 'Hermes A'); --Hermes, Hermes A, Hermes B, PostGIS A, PostGIS B

						INSERT INTO S2T_Parameters(key, value) VALUES ('temporal_buffer_size', '0 seconds');

						execute '
									INSERT INTO S2T_Parameters(key, value) 
									SELECT ''spatial_buffer_size'', round(sqrt(-2*(length(SegmentSP(lx,ly, hx,hy)) * 0.001)^2*ln(0.001)))::integer
									FROM hdatasets_statistics
									WHERE dataset = HDatasetID('''||original_datasource||''')
								';
						

						execute '
									INSERT INTO S2T_Parameters(key, value) -- w expresses the minimum number of segments that can define a subtrajectory
									SELECT ''w'', round((avg_trajectory_points + 1) * 0.2)::integer
									FROM hdatasets_statistics
									WHERE dataset = HDatasetID('''||original_datasource||''')
								';

						INSERT INTO S2T_Parameters(key, value) VALUES ('tau', 0.01); -- as tau increases, the number of subtrajectories reduces

						INSERT INTO S2T_Parameters(key, value)
							SELECT 'temporal_buffer_size_ssa', value
							FROM S2T_Parameters
							WHERE key = 'temporal_buffer_size';
						INSERT INTO S2T_Parameters(key, value)
							SELECT 'spatial_buffer_size_ssa', value
							FROM S2T_Parameters
							WHERE key = 'spatial_buffer_size';
						
						INSERT INTO S2T_Parameters(key, value) VALUES ('M', 1000); -- how many samples, therefore how many clusters
						INSERT INTO S2T_Parameters(key, value) VALUES ('eps_ssa', 0.01); -- a positive number near to zero so that we don't put into the sample subtrajectories with small representativeness

						INSERT INTO S2T_Parameters(key, value)
							SELECT 'temporal_buffer_size_sca', value
							FROM S2T_Parameters
							WHERE key = 'temporal_buffer_size';
						INSERT INTO S2T_Parameters(key, value)
							SELECT 'spatial_buffer_size_sca', value
							FROM S2T_Parameters
							WHERE key = 'spatial_buffer_size';
						
						INSERT INTO S2T_Parameters(key, value) VALUES ('eps_sca', 0.85);


						PERFORM S2T_VotingMethod('Trapezoidal'); --Count, Trapezoidal
						PERFORM S2T_Sigma(length(SegmentSP(lx,ly, hx,hy)) * 0.01) FROM hdatasets_statistics WHERE dataset = HDatasetID('''||original_datasource||''');
						PERFORM S2T_Preprocessing(original_datasource, i, n);
							
						
						PERFORM S2T_Clustering();

						execute 'ANALYZE '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg';

						execute 'ANALYZE '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg_sample';
						execute 'ANALYZE '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg_voting';
						execute 'ANALYZE '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_subtraj';
						--insert the representatives of the current chunk and sub-chunk to the appropriate table AND insert only those representatives that are different FROM the already existing representatives
						
						execute 'SELECT max_clust_id FROM '||original_datasource||'_sub_chunks_metadata WHERE chunk_id = '|| i ||' AND sub_chunk_id = '|| n ||'' into max_cluster_id;
						
						execute '	
									UPDATE '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_subtraj r
									SET clust_id = '|| max_cluster_id ||' + r.clust_id
									WHERE clust_id IS NOT NULL
									
								';
						
						execute '	
									UPDATE '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg_voting r
									SET clust_id = '|| max_cluster_id ||' + r.clust_id
									WHERE clust_id IS NOT NULL
								';
						
						execute '	
									UPDATE '||original_datasource||'_sub_chunks_metadata 
									SET max_clust_id = r1.max_clust_id 
									FROM
										(
											SELECT max(clust_id) max_clust_id
											FROM '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_subtraj
										) r1
									WHERE chunk_id = '|| i ||' AND sub_chunk_id = '|| n ||'
								';
								
						execute '	
									UPDATE '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg r
									SET subtraj_id = r1.subtraj_id
									FROM '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_subtraj r1 
									WHERE (r.obj_id, r.traj_id) = (r1.obj_id, r1.traj_id) AND (r.seg_id BETWEEN r1.first_seg_id AND (r1.first_seg_id + r1.NrSegments - 1)) AND r1.subtraj_sample = FALSE
								';
						
						execute '	
									UPDATE '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg_sample r
									SET subtraj_id = r1.subtraj_id, clust_id = 	r1.clust_id
									FROM '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_subtraj r1 
									WHERE (r.obj_id, r.traj_id) = (r1.obj_id, r1.traj_id) AND (r.seg_id BETWEEN r1.first_seg_id AND (r1.first_seg_id + r1.NrSegments - 1)) AND r1.subtraj_sample = TRUE
								';
						
						execute '
									INSERT INTO '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg(obj_id, traj_id, subtraj_id, seg_id, seg)
									SELECT obj_id, traj_id, subtraj_id, seg_id, seg FROM '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg_sample';
						
						execute ' ANALYZE '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg';
						
								
						
						
						for k in execute 	'
												SELECT r1.obj_id, r1.traj_id, r1.subtraj_id, r1.clust_id, min(getti(r1.seg)) sub_traj_ti, max(gette(r1.seg)) sub_traj_te
												FROM '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg_sample r1
												GROUP BY r1.obj_id, r1.traj_id, r1.subtraj_id, r1.clust_id
											' loop
									
							similarity = 0;

							execute  '	
								SELECT clust_id, avg(voting)
								FROM
									(	
										SELECT obj_id, traj_id, subtraj_id, clust_id, seg, CASE WHEN exp_input <= 745 THEN exp(-exp_input) ELSE 0 END AS voting
										FROM
											(
												SELECT repr.obj_id, repr.traj_id, repr.subtraj_id, repr.seg_id, repr.clust_id, repr.seg, COALESCE(power(min(distance(getsp(repr.seg), getsp(new_repr.seg))), 2.0) / (2.0 * power(1000, 2.0))) exp_input
												FROM 
													'||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg_sample new_repr 
															INNER JOIN '||original_datasource||'_c_'||i||'_sc_'||n||'_repr repr
																ON (new_repr.obj_id, new_repr.traj_id, new_repr.subtraj_id) = (repr.obj_id, repr.traj_id, repr.subtraj_id)
												WHERE (new_repr.obj_id, new_repr.traj_id, new_repr.subtraj_id) = ('||k.obj_id||', '||k.traj_id||', '||k.subtraj_id||')
												GROUP BY repr.obj_id, repr.traj_id, repr.subtraj_id, repr.clust_id, repr.seg_id, repr.seg
											) nested_tbl
									) nested_tbl
								GROUP BY obj_id, traj_id, subtraj_id, clust_id
								HAVING  (max(max(gette(seg)), '''||k.sub_traj_te||''') - min(max(gette(seg)), '''||k.sub_traj_te||''') ) + (max(min(getti(seg)), '''||k.sub_traj_ti||''') - min(min(getti(seg)), '''||k.sub_traj_ti||''')) < '''||non_common_lifespan||'''
								ORDER BY avg(voting) DESC
								LIMIT 1
							' into candidate_cluster_id, similarity;

							if similarity IS NULL then
						
								similarity := 0;

							end if;
									
							if (similarity < similarity_threshold) then
							
								--add representative in representative set
								execute '	
											INSERT INTO '||original_datasource||'_c_'||i||'_sc_'||n||'_repr(obj_id, traj_id, subtraj_id, seg_id, clust_id, seg)
											(
												SELECT r1.obj_id, r1.traj_id, r1.subtraj_id, r1.seg_id, r1.clust_id, r1.seg
												FROM '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg_sample r1
												WHERE (r1.obj_id, r1.traj_id, r1.subtraj_id) = ('||k.obj_id||', '||k.traj_id||', '||k.subtraj_id||')
											)
										';
										
								execute '	
											INSERT INTO '||original_datasource||'_c_'||i||'_sc_'||n||'(obj_id, traj_id, subtraj_id, seg_id, clust_id, seg)
											(
												SELECT r1.obj_id, r1.traj_id, r1.subtraj_id, r1.seg_id, r1.clust_id, r1.seg
												FROM '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg_sample r1
												WHERE (r1.obj_id, r1.traj_id, r1.subtraj_id) = ('||k.obj_id||', '||k.traj_id||', '||k.subtraj_id||')
											)
										';
								
								execute '	
											INSERT INTO '||original_datasource||'_c_'||i||'_sc_'||n||'(obj_id, traj_id, subtraj_id, seg_id, clust_id, seg)
											(
												SELECT r1.obj_id, r1.traj_id, r1.subtraj_id, r1.seg_id, r2.clust_id, r1.seg
												FROM '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg r1
													INNER JOIN '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg_voting r2
														ON (r1.obj_id, r1.traj_id, r1.subtraj_id, r1.seg_id) = (r2.obj_id, r2.traj_id, r2.subtraj_id, r2.seg_id)
												WHERE r2.clust_id = '||k.clust_id||'
											)
										';
								
							else			
										
								--add representative and subtrajectories that belong to the current cluster in subchunk set
								execute '	
											INSERT INTO '||original_datasource||'_c_'||i||'_sc_'||n||'(obj_id, traj_id, subtraj_id, seg_id, clust_id, seg)
											(
												SELECT r1.obj_id, r1.traj_id, r1.subtraj_id, r1.seg_id, '|| candidate_cluster_id ||', r1.seg
												FROM '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg r1
													INNER JOIN '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg_voting r2
														ON (r1.obj_id, r1.traj_id, r1.subtraj_id, r1.seg_id) = (r2.obj_id, r2.traj_id, r2.subtraj_id, r2.seg_id)
												WHERE r2.clust_id = '||k.clust_id||'
											)
										';
							end if;		

						end loop;

						--delete trajectories that belong to the current cluster from subchunk outliers set
						execute '	
									DELETE 
									FROM '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg
									WHERE (obj_id, traj_id, subtraj_id) IN (
																				SELECT obj_id, traj_id, subtraj_id 
																				FROM '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_subtraj 
																				WHERE clust_id IS NOT NULL
																			)
								';
							
						execute '	
									INSERT INTO  '||original_datasource||'_outliers_archive_seg
									SELECT * FROM '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg 
									WHERE (obj_id, traj_id, subtraj_id) IN (
																			SELECT obj_id, traj_id, subtraj_id 
																			FROM '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg 
																			GROUP BY obj_id, traj_id, subtraj_id 
																			HAVING count(seg_id) <= '||min_size_of_subtraj||'
																			)
								';
						
						execute '	
									DELETE 
									FROM '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg 
									WHERE (obj_id, traj_id, subtraj_id) IN (
																				SELECT obj_id, traj_id, subtraj_id 
																				FROM '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_subtraj 
																				WHERE NrSegments <= '||min_size_of_subtraj||'
																			)
								';
						
						execute 'TRUNCATE '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg_voting';
						execute 'TRUNCATE '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_subtraj';
						execute 'TRUNCATE '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg_sample';
						execute 'ANALYZE '||original_datasource||'_c_'||i||'_sc_'||n||'_outliers_seg';
						
					end if;
					
				end loop;	
			
			end loop;
			
			EXIT;
			
		end if;
	
	end loop;
	
end

$BODY$

LANGUAGE plpgsql;

--ReTraTree(data_source character varying, chunk_lifespan interval, sub_chunk_temporal_tolerance interval, max_n_of_outliers_in_subchunk integer, non_common_lifespan interval, similarity_threshold double precision, min_size_of_subtraj integer)

--SELECT ReTraTree('milan_subset_1_seg', '24 hours', '240 minutes', 20, '120 minutes', 0.5, 16)
--SELECT ReTraTree('milan_subset_1_seg', '24 hours', '24 hours', 100, '240 minutes', 0.5, 16)
--SELECT ReTraTree('hermoupolis_toy_seg', '60 seconds', '2 seconds', 10, '1 seconds', 0.5, 16)


--select drop_ReTraTree('milan_subset_1_seg');
--update milan_subset_1_seg set to_insert = true, subtraj_id = 0
--(obj_id, traj_id, subtraj_id, seg_id)=(5, 2, 0, 34)


--SELECT ReTraTree('pkdd_synthetic_400_50_seg', '20 seconds', '2 seconds', 20, '10 second', 0, 16)

--select drop_ReTraTree('pkdd_synthetic_400_50_seg');
/*
SELECT obj_id, traj_id, subtraj_id,clust_id, avg(exponent), ( max(max(gette(seg)), '2014-03-24 00:00:44') - min(max(gette(seg)), '2014-03-24 00:00:44') ) + ( max(min(getti(seg)), '2014-03-24 00:00:01') - min(min(getti(seg)), '2014-03-24 00:00:01') )
FROM
		(
			SELECT repr.obj_id, repr.traj_id, repr.subtraj_id, repr.seg_id, repr.clust_id, repr.seg, exp(-(power(min(distance(getsp(repr.seg), getsp(new_repr.seg))), 2.0) / (2.0 * power(1000, 2.0)))) exponent
			FROM pkdd_synthetic_400_50_seg_chunk_1_subchunk_1_sample new_repr, pkdd_synthetic_400_50_seg_chunk_1_subchunk_1_sample repr
			WHERE (new_repr.obj_id, new_repr.traj_id, new_repr.subtraj_id) = (1, 1, 1)
			GROUP BY repr.obj_id, repr.traj_id, repr.subtraj_id, repr.clust_id, repr.seg_id, repr.seg
		) nested_tbl
GROUP BY obj_id, traj_id, subtraj_id, clust_id
HAVING  ( max(max(gette(seg)), '2014-03-24 00:00:44') - min(max(gette(seg)), '2014-03-24 00:00:44') ) + ( max(min(getti(seg)), '2014-03-24 00:00:01') - min(min(getti(seg)), '2014-03-24 00:00:01') ) < '10 second'
ORDER BY avg(exponent) DESC
LIMIT 1
*/
