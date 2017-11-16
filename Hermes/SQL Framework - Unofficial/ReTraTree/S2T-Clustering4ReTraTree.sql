/*
 * Authors: Marios Vodas (mvodas@gmail.com), Panagiotis Tampakis (panosbp@gmail.com), Stamatis Stefanopoulos (stamostef@yahoo.gr).
 */

/******************************************************************************/
/*
DROP TABLE IF EXISTS S2T_Parameters CASCADE;
CREATE TEMPORARY TABLE S2T_Parameters (
	key text NOT NULL,
	value text NOT NULL,

	PRIMARY KEY (key)
);

INSERT INTO S2T_Parameters(key, value) VALUES ('D', 'imis');

INSERT INTO S2T_Parameters(key, value) VALUES ('NN_method', 'Hermes'); --Hermes, Hermes A, Hermes B, PostGIS A, PostGIS B

INSERT INTO S2T_Parameters(key, value)
	SELECT 'temporal_buffer_size', (tmax - tmin) * 0.01 / 1
	FROM hdatasets_statistics
	WHERE dataset = HDatasetID('imis');
INSERT INTO S2T_Parameters(key, value)
	SELECT 'spatial_buffer_size', round(length(SegmentSP(lx,ly, hx,hy)) * 0.01 / 1)::integer
	FROM hdatasets_statistics
	WHERE dataset = HDatasetID('imis');

INSERT INTO S2T_Parameters(key, value) -- w expresses the minimum number of segments that can define a subtrajectory
	SELECT 'w', round((avg_trajectory_points + 1) * 0.2)::integer
	FROM hdatasets_statistics
	WHERE dataset = HDatasetID('imis');
INSERT INTO S2T_Parameters(key, value) VALUES ('tau', 0.01); -- as tau increases, the number of subtrajectories reduces

INSERT INTO S2T_Parameters(key, value)
	SELECT 'temporal_buffer_size_ssa', value
	FROM S2T_Parameters
	WHERE key = 'temporal_buffer_size';
INSERT INTO S2T_Parameters(key, value)
	SELECT 'spatial_buffer_size_ssa', value
	FROM S2T_Parameters
	WHERE key = 'spatial_buffer_size';
--INSERT INTO S2T_Parameters(key, value) VALUES ('m_start', 1); -- when start/stop is active, at what cluster to start -- currently not implemented
INSERT INTO S2T_Parameters(key, value) VALUES ('M', 10000); -- how many samples, therefore how many clusters
INSERT INTO S2T_Parameters(key, value) VALUES ('eps_ssa', 0.01); -- a positive number near to zero so that we don't put into the sample subtrajectories with small representativeness

INSERT INTO S2T_Parameters(key, value)
	SELECT 'temporal_buffer_size_sca', value
	FROM S2T_Parameters
	WHERE key = 'temporal_buffer_size';
INSERT INTO S2T_Parameters(key, value)
	SELECT 'spatial_buffer_size_sca', value
	FROM S2T_Parameters
	WHERE key = 'spatial_buffer_size';
INSERT INTO S2T_Parameters(key, value) VALUES ('eps_sca', 0.001);

--SELECT S2T_SimplificationMethodT(avg_segment_duration * 10) FROM hdatasets_statistics WHERE dataset = HDatasetID('imis');
--SELECT S2T_SimplificationMethodDP(avg_segment_length * 10) FROM hdatasets_statistics WHERE dataset = HDatasetID('imis');
--SELECT S2T_SimplificationMethodDPP(avg_segment_length * 10) FROM hdatasets_statistics WHERE dataset = HDatasetID('imis');
SELECT S2T_SimplificationMethodNone();

SELECT S2T_VotingMethod('Count'); --Count, Trapezoidal
SELECT S2T_Sigma(length(SegmentSP(lx,ly, hx,hy)) * 0.001) FROM hdatasets_statistics WHERE dataset = HDatasetID('imis');

SELECT S2T_PrepareDataset('imis');
SELECT S2T_Clustering(true);
*/

/******************************************************************************/
/*
SELECT st.clust_id, count(*) AS cnt
FROM imis_subtraj AS st
	INNER JOIN imis_seg AS s
		ON (st.obj_id, st.traj_id, st.subtraj_id) = (s.obj_id, s.traj_id, s.subtraj_id)
GROUP BY st.clust_id
ORDER BY cnt DESC;

COPY (
	SELECT obj_id
	FROM imis_subtraj
	WHERE clust_id IS NOT NULL
		EXCEPT
	(
		SELECT obj_id
		FROM imis_subtraj
		WHERE clust_id IS NULL
			EXCEPT
		SELECT obj_id
		FROM imis_subtraj
		WHERE subtraj_sample
	)
) TO 'C:\PostgreSQL\9.3\x64\data\aminess_demo_objects.txt' WITH CSV HEADER;

COPY (

SELECT KMLDocument(string_agg(tracksFolder, ''))
FROM (
	SELECT obj_id, KMLFolder('MMSI: ' || obj_id,
		string_agg(trackPlacemark, '')) AS tracksFolder
	FROM (
		SELECT obj_id, traj_id, KMLTrack(
            ------------------- Balloon Info -----------------------------------
			'MMSI: ' || obj_id || '<br/>' ||
			count(*) - 1 || ' points sampled between ' || min(getTi(seg)) ||
			' and ' || max(getTe(seg)) || '.<br/>' ||
			'The ship covered a distance of ' ||
			trunc(metres2nm(sum(length(getSp(seg))))::numeric, 1) ||
			' NM with an average speed of ' ||
			trunc(mps2knots(sum(length(getSp(seg))) /
				extract(epoch from max(getTe(seg)) - min(getTi(seg))))::numeric
				, 1) || ' knots.'
            ------------------- End of Balloon Info ----------------------------
			, array_agg(seg ORDER BY getTi(seg) ASC), HDatasetID('imis')
			) AS trackPlacemark
		FROM imis_seg
		GROUP BY obj_id, traj_id
	) AS tracks
	GROUP BY obj_id
) AS folders

) TO 'C:\PostgreSQL\9.3\x64\data\S2T-Input.kml';

COPY (

SELECT KMLDocument(string_agg(tracksFolder, ''))
FROM (
	SELECT clust_id, KMLFolder('Cluster: ' || clust_id,
		string_agg(trackPlacemark, '')) AS tracksFolder
	FROM (
		SELECT st.clust_id, st.obj_id, st.traj_id, st.subtraj_id, KMLTrack(
            ------------------- Balloon Info -----------------------------------
			'MMSI: ' || st.obj_id || '<br/>' ||
			count(*) - 1 || ' points sampled between ' || min(getTi(s.seg)) ||
			' and ' || max(getTe(s.seg)) || '.<br/>' ||
			'The ship covered a distance of ' ||
			trunc(metres2nm(sum(length(getSp(s.seg))))::numeric, 1) ||
			' NM with an average speed of ' ||
			trunc(mps2knots(sum(length(getSp(s.seg))) /
				extract(epoch from max(getTe(s.seg)) - min(getTi(s.seg))))::numeric
				, 1) || ' knots.'
            ------------------- End of Balloon Info ----------------------------
			, array_agg(s.seg ORDER BY getTi(s.seg) ASC), HDatasetID('imis')
			) AS trackPlacemark
		FROM imis_subtraj AS st
			INNER JOIN imis_seg AS s
				ON (st.obj_id, st.traj_id, st.subtraj_id) = (s.obj_id, s.traj_id, s.subtraj_id)
		GROUP BY st.clust_id, st.obj_id, st.traj_id, st.subtraj_id
	) AS tracks
	GROUP BY clust_id
) AS folders

) TO 'C:\PostgreSQL\9.3\x64\data\S2T-Output.kml';
*/

/***********************************************/
DROP TYPE IF EXISTS voting_signal CASCADE;
CREATE TYPE voting_signal AS (
	obj_id integer,
	traj_id integer,
	subtraj_id integer,
	
	subtraj_upper integer[],

	seg_id integer[],
	normalized_voting double precision[],
	normalized_lifespan interval[]
);

/***********************************************/
CREATE OR REPLACE FUNCTION S2T_Preprocessing(data_source text, i integer, n integer) RETURNS void AS $$
declare
n_of_idx integer;

BEGIN
	--Should be a TEMPORARY TABLE
	
	execute '
				CREATE TEMPORARY TABLE IF NOT EXISTS '||data_source||'_c_'||i||'_sc_'||n||'_outliers_seg_sample
					(
						obj_id integer NOT NULL,
						traj_id integer NOT NULL,
						subtraj_id integer NOT NULL,
						clust_id integer NOT NULL,
						seg_id integer NOT NULL,
						seg segmentst NOT NULL,
						CONSTRAINT '||data_source||'_c_'||i||'_sc_'||n||'_outliers_seg_sample_pkey PRIMARY KEY (obj_id, traj_id, subtraj_id, seg_id)
					)
			'; 
			
	execute '	
				SELECT count(*) 
				FROM pg_class 
				WHERE relname = '''||data_source||'_c_'||i||'_sc_'||n||'_outliers_seg_sample_seg_idx''
			' into n_of_idx;
	
	if n_of_idx = 0 then 

		--execute 'CREATE INDEX '||data_source||'_c_'||i||'_sc_'||n||'_sample_idx ON '||data_source||'_c_'||i||'_sc_'||n||'_sample using btree(clust_id)';
		
		execute 'CREATE INDEX '||data_source||'_c_'||i||'_sc_'||n||'_outliers_seg_sample_seg_idx ON '||data_source||'_c_'||i||'_sc_'||n||'_outliers_seg_sample using gist(seg)';
		
	end if;
	
	execute '
				CREATE TEMPORARY TABLE IF NOT EXISTS '||data_source||'_c_'||i||'_sc_'||n||'_outliers_seg_voting
					(
						obj_id integer NOT NULL, 
						traj_id integer NOT NULL, 
						subtraj_id integer NOT NULL,
						subtraj_upper integer,
						seg_id integer NOT NULL, 
						clust_id integer, 
						seg segmentst, 
						normalized_voting double precision,
						normalized_lifespan interval,
						CONSTRAINT '||data_source||'_c_'||i||'_sc_'||n||'_outliers_seg_voting_pkey PRIMARY KEY (obj_id, traj_id, subtraj_id, seg_id)
					)
			'; 
	
	execute '	
				SELECT count(*) 
				FROM pg_class 
				WHERE relname = '''||data_source||'_c_'||i||'_sc_'||n||'_outliers_seg_voting_seg_idx''
			' into n_of_idx;
	
	if n_of_idx = 0 then 

		--execute 'CREATE INDEX '||data_source||'_c_'||i||'_sc_'||n||'_sample_idx ON '||data_source||'_c_'||i||'_sc_'||n||'_sample using btree(clust_id)';
		
		execute 'CREATE INDEX '||data_source||'_c_'||i||'_sc_'||n||'_outliers_seg_voting_seg_idx ON '||data_source||'_c_'||i||'_sc_'||n||'_outliers_seg_voting using gist(seg)';
		
	end if;
	
	
	--Perform S2T Clustering to the outliers set
	
	-- create the subtraj table for the current chunk and sub-chunk if it doesnt already exist
	execute '
				CREATE TEMPORARY TABLE IF NOT EXISTS '||data_source||'_c_'||i||'_sc_'||n||'_outliers_subtraj
					(
						obj_id integer NOT NULL,
						traj_id integer NOT NULL,
						subtraj_id integer NOT NULL,
						subtraj_upper integer,
						sr_gain_zero double precision NOT NULL,
						first_seg_id integer NOT NULL,
						NrSegments integer NOT NULL,
						subtraj_sample boolean NOT NULL DEFAULT false,
						clust_id integer,
						sca_voting double precision,
						CONSTRAINT '||data_source||'_c_'||i||'_sc_'||n||'_outliers_subtraj_idx PRIMARY KEY (obj_id, traj_id, subtraj_id, subtraj_upper) DEFERRABLE INITIALLY IMMEDIATE
					)
			';

	--check if the appropriate indexes exist
	execute '	
		SELECT count(*) 
		FROM pg_class 
		WHERE relname = '''||data_source||'_c_'||i||'_sc_'||n||'_outliers_subtraj_idx''
	' into n_of_idx;
	
	--if they dont exist then create them
	if n_of_idx = 0 then 
	
		execute 'CREATE INDEX '||data_source||'_c_'||i||'_sc_'||n||'_outliers_subtraj2_idx ON '||data_source||'_c_'||i||'_sc_'||n||'_outliers_subtraj using btree(clust_id)';
		execute 'CREATE INDEX '||data_source||'_c_'||i||'_sc_'||n||'_outliers_subtraj3_idx ON '||data_source||'_c_'||i||'_sc_'||n||'_outliers_subtraj using btree(sr_gain_zero DESC)';
	
	end if;
	
END;
$$ LANGUAGE plpgsql STRICT;

/***********************************************/
CREATE OR REPLACE FUNCTION S2T_Postprocessing(data_source text, i integer, n integer) RETURNS void AS $$
declare
max_cluster_id integer;

BEGIN

	execute ' ANALYZE '||data_source||'_c_'||i||'_sc_'||n||'_outliers_seg_sample';
	execute ' ANALYZE '||data_source||'_c_'||i||'_sc_'||n||'_outliers_seg_voting';
	execute ' ANALYZE '||data_source||'_c_'||i||'_sc_'||n||'_outliers_subtraj';
	
	execute '
				SELECT coalesce(max(clust_id), 0)
				FROM '||data_source||'_c_'||i||'_sc_'||n||'_repr
			' into max_cluster_id;
			
	execute '	
				UPDATE '||data_source||'_c_'||i||'_sc_'||n||'_outliers_subtraj r
				SET subtraj_id = greatest(r1.subtraj_max_subtraj_id, r1.sc_max_subtraj_id) + r.subtraj_id,
					clust_id = 	'|| max_cluster_id ||' + r.clust_id
				FROM 
					(
						SELECT r1.obj_id, r1.traj_id, max(r1.subtraj_id) subtraj_max_subtraj_id, coalesce(max(r2.subtraj_id), 0) sc_max_subtraj_id 
						FROM '||data_source||'_c_'||i||'_sc_'||n||'_outliers_subtraj r1
							LEFT JOIN '||data_source||'_c_'||i||'_sc_'||n||' r2
								ON (r1.obj_id, r1.traj_id) = (r2.obj_id, r2.traj_id)
						GROUP BY r1.obj_id, r1.traj_id
					) r1
				WHERE (r.obj_id, r.traj_id) = (r1.obj_id, r1.traj_id)
			';
	
	execute '	
				UPDATE '||data_source||'_c_'||i||'_sc_'||n||'_outliers_seg_sample r
				SET subtraj_id = r1.subtraj_id, clust_id = 	r1.clust_id
				FROM '||data_source||'_c_'||i||'_sc_'||n||'_outliers_subtraj r1 
				WHERE (r.obj_id, r.traj_id) = (r1.obj_id, r1.traj_id) AND (r.seg_id BETWEEN r1.first_seg_id AND (r1.first_seg_id + r1.NrSegments - 1))
			';
	
	execute 'INSERT INTO '||data_source||'_c_'||i||'_sc_'||n||'_outliers_seg(obj_id, traj_id, subtraj_id, seg_id, seg)
				SELECT obj_id, traj_id, subtraj_id, seg_id, seg FROM '||data_source||'_c_'||i||'_sc_'||n||'_outliers_seg_sample';
	
	execute ' ANALYZE '||data_source||'_c_'||i||'_sc_'||n||'_outliers_seg';
	
	execute '	
				UPDATE '||data_source||'_c_'||i||'_sc_'||n||'_outliers_seg_voting r
				SET subtraj_id = r1.subtraj_id, clust_id = 	r1.clust_id
				FROM '||data_source||'_c_'||i||'_sc_'||n||'_outliers_subtraj r1 
				WHERE (r.obj_id, r.traj_id) = (r1.obj_id, r1.traj_id) AND (r.seg_id BETWEEN r1.first_seg_id AND (r1.first_seg_id + r1.NrSegments - 1))
			';
			
	execute '	
				UPDATE '||data_source||'_c_'||i||'_sc_'||n||'_outliers_seg r
				SET subtraj_id = r1.subtraj_id
				FROM '||data_source||'_c_'||i||'_sc_'||n||'_outliers_subtraj r1 
				WHERE (r.obj_id, r.traj_id) = (r1.obj_id, r1.traj_id) AND (r.seg_id BETWEEN r1.first_seg_id AND (r1.first_seg_id + r1.NrSegments - 1))
			';
END;
$$ LANGUAGE plpgsql STRICT;

/***********************************************/
CREATE OR REPLACE FUNCTION S2T_Clustering(verbose boolean DEFAULT false, only_voting boolean DEFAULT false) RETURNS void AS $$
#yield (None, None)

import time
#from plpy import spiexceptions

try:
	GETparam_Plan = plpy.prepare("SELECT value FROM S2T_Parameters WHERE key = $1;", ["text"])
	SETparam_Plan = plpy.prepare("UPDATE S2T_Parameters SET value = $2 WHERE key = $1;", ["text", "text"])

	rv = plpy.execute(GETparam_Plan, ["data_source"])
	data_source = rv[0]["value"]

	rv = plpy.execute(GETparam_Plan, ["D"])
	D = rv[0]["value"]

	rv = plpy.execute(GETparam_Plan, ["NN_method"])
	NN_method = rv[0]["value"]

	rv = plpy.execute(GETparam_Plan, ["temporal_buffer_size"])
	temporal_buffer_size = rv[0]["value"]
	rv = plpy.execute(GETparam_Plan, ["spatial_buffer_size"])
	spatial_buffer_size = int(rv[0]["value"])

	rv = plpy.execute(GETparam_Plan, ["w"])
	w = int(rv[0]["value"])
	rv = plpy.execute(GETparam_Plan, ["tau"])
	tau = float(rv[0]["value"])

	rv = plpy.execute(GETparam_Plan, ["temporal_buffer_size_ssa"])
	temporal_buffer_size_ssa = rv[0]["value"]
	rv = plpy.execute(GETparam_Plan, ["spatial_buffer_size_ssa"])
	spatial_buffer_size_ssa = int(rv[0]["value"])
	rv = plpy.execute(GETparam_Plan, ["M"])
	M = int(rv[0]["value"])
	rv = plpy.execute(GETparam_Plan, ["eps_ssa"])
	eps_ssa = float(rv[0]["value"])

	rv = plpy.execute(GETparam_Plan, ["temporal_buffer_size_sca"])
	temporal_buffer_size_sca = rv[0]["value"]
	rv = plpy.execute(GETparam_Plan, ["spatial_buffer_size_sca"])
	spatial_buffer_size_sca = int(rv[0]["value"])
	rv = plpy.execute(GETparam_Plan, ["eps_sca"])
	eps_sca = float(rv[0]["value"])

	#plpy.execute("DROP TABLE IF EXISTS " + plpy.quote_ident(D + "_" + NN_method + "_stats") + ";")
	#plpy.execute("CREATE TABLE " + plpy.quote_ident(D + "_" + NN_method + "_stats") + "(id bigserial, algorithm_instance text, instance_duration double precision);")

	#stats_Plan = plpy.prepare("INSERT INTO " + plpy.quote_ident(D + "_" + NN_method + "_stats") + "(algorithm_instance, instance_duration) VALUES ($1, $2);", ["text", "double precision"])

	plpy.execute("SET enable_seqscan = off;")

	if NN_method == "PostGIS B":
		t1 = time.time()
		plpy.execute("UPDATE " + plpy.quote_ident(D + "_traj") + " AS t SET mbb_diagonal_pgis = s.mbb_diagonal FROM (SELECT obj_id, traj_id, ST_MakeLine(ST_MakePoint(min(ST_XMin(seg_pgis)) - " + str(spatial_buffer_size) + ", min(ST_YMin(seg_pgis)) - " + str(spatial_buffer_size) + ", min(ST_ZMin(seg_pgis)) - extract(epoch FROM '" + temporal_buffer_size + "'::interval)), ST_MakePoint(max(ST_XMax(seg_pgis)) + " + str(spatial_buffer_size) + ", max(ST_YMax(seg_pgis)) + " + str(spatial_buffer_size) + ", max(ST_ZMax(seg_pgis)) + extract(epoch FROM '" + temporal_buffer_size + "'::interval))) AS mbb_diagonal FROM ONLY " + plpy.quote_ident(D + "_seg") + " GROUP BY obj_id, traj_id) AS s WHERE (t.obj_id, t.traj_id) = (s.obj_id, s.traj_id);")
		t2 = time.time()

		if verbose:
			plpy.info("traj_mbb = " + str(round(t2 - t1)) + " sec")
	elif NN_method == "Hermes B":
		t1 = time.time()
		plpy.execute("UPDATE " + plpy.quote_ident(D + "_traj") + " AS t SET mbb = s.mbb FROM (SELECT obj_id, traj_id, buffer(BoxST(min(getTi(seg)), max(getTe(seg)), min(getLx(getSp(seg)::BoxSP)), min(getLy(getSp(seg)::BoxSP)), max(getHx(getSp(seg)::BoxSP)), max(getHy(getSp(seg)::BoxSP))), '" + temporal_buffer_size + "', " + str(spatial_buffer_size) + ", " + str(spatial_buffer_size) + ") AS mbb FROM ONLY " + plpy.quote_ident(D + "_seg") + " GROUP BY obj_id, traj_id) AS s WHERE (t.obj_id, t.traj_id) = (s.obj_id, s.traj_id);")
		t2 = time.time()

		if verbose:
			plpy.info("traj_mbb = " + str(round(t2 - t1)) + " sec")

	plpy.execute("SELECT S2T_TemporalBufferSize('" + temporal_buffer_size + "');")
	plpy.execute("SELECT S2T_SpatialBufferSize(" + str(spatial_buffer_size) + ");")

	GVA_Plan = plpy.prepare("SELECT obj_id, traj_id, subtraj_id, subtraj_upper, seg_id, normalized_voting, normalized_lifespan FROM S2T_GVA($1, $2, $3, $4, $5);", ["integer", "integer", "integer", "text", "text"])
	TSA_Plan = plpy.prepare("SELECT obj_id, traj_id, subtraj_id, subtraj_upper, seg_id, normalized_voting, normalized_lifespan FROM S2T_TSA(($1, $2, $3, $4, $5, $6, $7)::voting_signal, $8, $9);", ["integer", "integer", "integer", "integer[]", "integer[]", "double precision[]", "interval[]", "integer", "double precision"])
	vsrelt_subtraj_Plan = plpy.prepare("INSERT INTO " + plpy.quote_ident(D + "_subtraj") + "(obj_id, traj_id, subtraj_id, subtraj_upper, sr_gain_zero, first_seg_id, NrSegments) SELECT obj_id, traj_id, (SELECT max_subtraj_id FROM " + plpy.quote_ident(data_source + "_max_subtraj_id") + " WHERE (obj_id, traj_id) = ($1, $2)) + subtraj_upper, subtraj_upper, sum(normalized_voting * extract(epoch from normalized_lifespan)), min(seg_id), count(*) FROM S2T_vsrelt(($1, $2, $3, $4, $5, $6, $7)::voting_signal) GROUP BY obj_id, traj_id, subtraj_id, subtraj_upper ORDER BY obj_id ASC, traj_id ASC, subtraj_id, subtraj_upper ASC;", ["integer", "integer", "integer", "integer[]", "integer[]", "double precision[]", "interval[]"])
	vsrelt_seg_voting_Plan = plpy.prepare("INSERT INTO " + plpy.quote_ident(D + "_seg_voting") + "(obj_id, traj_id, seg_id, subtraj_id, subtraj_upper,  normalized_voting, normalized_lifespan) SELECT obj_id, traj_id, seg_id, (SELECT max_subtraj_id FROM " + plpy.quote_ident(data_source + "_max_subtraj_id") + " WHERE (obj_id, traj_id) = ($1, $2)) + subtraj_upper, subtraj_upper, normalized_voting, normalized_lifespan FROM S2T_vsrelt(($1, $2, $3, $4, $5, $6, $7)::voting_signal);", ["integer", "integer", "integer", "integer[]", "integer[]", "double precision[]", "interval[]"])
	#fix_subtraj_id_Plan_1 = plpy.prepare("UPDATE " + plpy.quote_ident(D + "_subtraj") + " SET subtraj_id = ((SELECT max_subtraj_id FROM " + plpy.quote_ident(data_source + "_max_subtraj_id") + " WHERE (obj_id, traj_id) = ($1, $2)) + subtraj_upper) WHERE (obj_id, traj_id) = ($1, $2);", ["integer", "integer"])
	#fix_subtraj_id_Plan_2 = plpy.prepare("UPDATE " + plpy.quote_ident(D + "_seg_voting") + " SET subtraj_id = ((SELECT max_subtraj_id FROM " + plpy.quote_ident(data_source + "_max_subtraj_id") + " WHERE (obj_id, traj_id) = ($1, $2)) + subtraj_upper) WHERE (obj_id, traj_id) = ($1, $2);", ["integer", "integer"])
	update_max_subtraj_id_Plan = plpy.prepare("UPDATE " + plpy.quote_ident(data_source + "_max_subtraj_id") + "  SET max_subtraj_id = (SELECT max(subtraj_id) FROM " + plpy.quote_ident(D + "_subtraj") + " WHERE (obj_id, traj_id) = ($1, $2)) WHERE (obj_id, traj_id) = ($1, $2);", ["integer", "integer"])

	rv = plpy.execute("SELECT DISTINCT obj_id, traj_id, subtraj_id FROM ONLY " + plpy.quote_ident(D + "_seg") + " ORDER BY obj_id, traj_id, subtraj_id;")
	NrTrajectories = len(rv)
	duration_GVA_sum = 0
	duration_TSA_sum = 0
	duration_vsrelt_subtraj_sum = 0
	duration_vsrelt_seg_voting_sum = 0
	for i, v in enumerate(rv):

		with plpy.subtransaction():
			t1 = time.time()
			rv2 = plpy.execute(GVA_Plan, [v["obj_id"], v["traj_id"], v["subtraj_id"], D, data_source])
			t2 = time.time()
			duration_GVA = t2 - t1

			t1 = time.time()
			rv2 = plpy.execute(TSA_Plan, [rv2[0]["obj_id"], rv2[0]["traj_id"], rv2[0]["subtraj_id"], rv2[0]["subtraj_upper"], rv2[0]["seg_id"], rv2[0]["normalized_voting"], rv2[0]["normalized_lifespan"], w, tau])
			t2 = time.time()
			duration_TSA = t2 - t1

			t1 = time.time()
			plpy.execute(vsrelt_subtraj_Plan, [rv2[0]["obj_id"], rv2[0]["traj_id"], rv2[0]["subtraj_id"], rv2[0]["subtraj_upper"], rv2[0]["seg_id"], rv2[0]["normalized_voting"], rv2[0]["normalized_lifespan"]])
			t2 = time.time()
			duration_vsrelt_subtraj = t2 - t1
			
			
			t1 = time.time()
			plpy.execute(vsrelt_seg_voting_Plan, [rv2[0]["obj_id"], rv2[0]["traj_id"], rv2[0]["subtraj_id"], rv2[0]["subtraj_upper"], rv2[0]["seg_id"], rv2[0]["normalized_voting"], rv2[0]["normalized_lifespan"]])
			t2 = time.time()
			duration_vsrelt_seg_voting = t2 - t1
			
			#plpy.execute(fix_subtraj_id_Plan_1, [rv2[0]["obj_id"], rv2[0]["traj_id"]])
			
			#plpy.execute(fix_subtraj_id_Plan_2, [rv2[0]["obj_id"], rv2[0]["traj_id"]])

			plpy.execute(update_max_subtraj_id_Plan, [rv2[0]["obj_id"], rv2[0]["traj_id"]])
			
			duration_GVA_sum = duration_GVA_sum + duration_GVA
			duration_TSA_sum = duration_TSA_sum + duration_TSA
			duration_vsrelt_subtraj_sum = duration_vsrelt_subtraj_sum + duration_vsrelt_subtraj
			duration_vsrelt_seg_voting_sum = duration_vsrelt_seg_voting_sum + duration_vsrelt_seg_voting

			duration_all = duration_GVA + duration_TSA + duration_vsrelt_subtraj + duration_vsrelt_seg_voting

	#		yield ("GVA", duration_GVA)
	#		yield ("TSA", duration_TSA)
		#plpy.execute(stats_Plan, ["GVA", duration_GVA])
		#plpy.execute(stats_Plan, ["TSA", duration_TSA])
		#plpy.execute(stats_Plan, ["vsrelt_subtraj", duration_vsrelt_subtraj])
		#plpy.execute(stats_Plan, ["vsrelt_seg_voting", duration_vsrelt_seg_voting])
		
		if verbose:
			plpy.info(str(round((i + 1) * 100 / NrTrajectories, 1)) + "%,	" + str(i + 1) + "	/	" + str(NrTrajectories) + ",	" + str(round(duration_all)) + " sec	[GVA = " + str(round(duration_GVA)) + " sec	>>	" + str(round(duration_GVA_sum / (i + 1))) + " sec,	TSA = " + str(round(duration_TSA)) + " sec	>>	" + str(round(duration_TSA_sum / (i + 1))) + " sec,	vsrelt_subtraj = " + str(round(duration_vsrelt_subtraj)) + " sec	>>	" + str(round(duration_vsrelt_subtraj_sum / (i + 1))) + " sec,	vsrelt_seg_voting = " + str(round(duration_vsrelt_seg_voting)) + " sec	>>	")

	if only_voting:
		return

	plpy.execute("SELECT S2T_TemporalBufferSize('" + temporal_buffer_size_ssa + "');")
	plpy.execute("SELECT S2T_SpatialBufferSize(" + str(spatial_buffer_size_ssa) + ");")

	isSample_Plan = plpy.prepare("SELECT subtraj_sample FROM " + plpy.quote_ident(D + "_subtraj") + " WHERE (obj_id, traj_id, subtraj_id) = ($1, $2, $3);", ["integer", "integer", "integer"])
	SRGain_Plan = plpy.prepare("SELECT S2T_SRGain($1, $2, $3, $4, $5) AS sr_gain;", ["integer", "integer", "integer", "text", "text"])
	SampleSimilarity_Plan = plpy.prepare("SELECT S2T_SampleSimilarity($1, $2, $3, $4, $5) AS similarity;", ["integer", "integer", "integer", "text", "text"])

	UpdateSampling_Plan = plpy.prepare("UPDATE " + plpy.quote_ident(D + "_subtraj") + " SET subtraj_sample = true, clust_id = $4 WHERE (obj_id, traj_id, subtraj_id) = ($1, $2, $3);", ["integer", "integer", "integer", "integer"])
	InsertSampling_Plan = plpy.prepare("INSERT INTO " + plpy.quote_ident(D + "_seg_sample") + " (obj_id, traj_id, subtraj_id, clust_id, seg_id, seg) SELECT obj_id, traj_id, $3, $4, seg_id, seg FROM ONLY " + plpy.quote_ident(D + "_seg") + " WHERE (obj_id, traj_id) = ($1, $2) AND seg_id IN (SELECT seg_id FROM " + plpy.quote_ident(D + "_seg_voting") + " WHERE (obj_id, traj_id, subtraj_id) = ($1, $2, $3));", ["integer", "integer", "integer", "integer"])
	DeleteSampling_Plan = plpy.prepare("DELETE FROM ONLY " + plpy.quote_ident(D + "_seg") + " WHERE (obj_id, traj_id) = ($1, $2) AND seg_id IN (SELECT seg_id FROM " + plpy.quote_ident(D + "_seg_voting") + " WHERE (obj_id, traj_id, subtraj_id) = ($1, $2, $3));", ["integer", "integer", "integer"])

	rv = plpy.execute("SELECT obj_id, traj_id, clust_id, subtraj_id FROM " + plpy.quote_ident(D + "_subtraj") + " ORDER BY sr_gain_zero DESC LIMIT 1;");
	plpy.execute(UpdateSampling_Plan, [rv[0]["obj_id"], rv[0]["traj_id"], rv[0]["subtraj_id"], 1])
	plpy.execute(InsertSampling_Plan, [rv[0]["obj_id"], rv[0]["traj_id"], rv[0]["subtraj_id"], 1])
	plpy.execute(DeleteSampling_Plan, [rv[0]["obj_id"], rv[0]["traj_id"], rv[0]["subtraj_id"]])

	rv = plpy.execute("SELECT obj_id, traj_id, subtraj_id, clust_id, sr_gain_zero FROM " + plpy.quote_ident(D + "_subtraj") + " ORDER BY sr_gain_zero DESC;")
	NrSubtrajectories = len(rv)
	SRGain_duration_values = []
	SampleSimilarity_duration_values = []
	for m in range(2, M + 1):
		SRGain_duration_values = []
		SampleSimilarity_duration_values = []

		with plpy.subtransaction():
			sr_max_gain = float('-Infinity')
			found_sample = False

			for i, v in enumerate(rv):
				sr_gain = float('-Infinity')
				duration_SRGain = "-"
				duration_SampleSimilarity = "-"

				t1 = time.time()
				rv2 = plpy.execute(isSample_Plan, [v["obj_id"], v["traj_id"], v["subtraj_id"]])
				t2 = time.time()
				duration_isSample = t2 - t1
				isSample = rv2[0]["subtraj_sample"]
				if not isSample:
					t1 = time.time()
					rv2 = plpy.execute(SRGain_Plan, [v["obj_id"], v["traj_id"], v["subtraj_id"], D, data_source])
					t2 = time.time()
					duration_SRGain = t2 - t1
					SRGain_duration_values.append(duration_SRGain)

					sr_gain = rv2[0]["sr_gain"]
					if sr_gain is not None and sr_gain > sr_max_gain:
						sr_max_gain = sr_gain
						
						t1 = time.time()
						rv2 = plpy.execute(SampleSimilarity_Plan, [v["obj_id"], v["traj_id"], v["subtraj_id"], D, data_source])
						t2 = time.time()
						duration_SampleSimilarity = t2 - t1
						SampleSimilarity_duration_values.append(duration_SampleSimilarity)

						similarity = rv2[0]["similarity"]
						if similarity is not None and similarity > 0.75:
							found_sample = False
						else:
							u_obj = v["obj_id"]
							u_traj = v["traj_id"]
							u_subtraj = v["subtraj_id"]
							found_sample = True
						
	#				if verbose:
	#					plpy.info(str(m) + ",	" + str(round((i + 1) * 100 / NrSubtrajectories, 1)) + "%,	" + str(i + 1) + "	/	" + str(NrSubtrajectories) + ",	isSample = " + str(round(duration_isSample)) + " sec,	" + "SRGain = " + str(duration_SRGain) + " sec,	" + "SampleSimilarity = " + str(duration_SampleSimilarity) + " sec	[sr_max_gain = " + str(sr_max_gain) + ",	sr_gain = " + str(sr_gain) + ",	sr_gain_zero = " + str(v["sr_gain_zero"]) + "]")

				if sr_max_gain > v["sr_gain_zero"]:
					break
				
			if found_sample:
				t1 = time.time()
				plpy.execute(UpdateSampling_Plan, [u_obj, u_traj, u_subtraj, m])
				t2 = time.time()
				duration_UpdateSampling = t2 - t1

				t1 = time.time()
				plpy.execute(InsertSampling_Plan, [u_obj, u_traj, u_subtraj, m])
				t2 = time.time()
				duration_InsertSampling = t2 - t1

				t1 = time.time()
				plpy.execute(DeleteSampling_Plan, [u_obj, u_traj, u_subtraj])
				t2 = time.time()
				duration_DeleteSampling = t2 - t1

				if verbose:
					plpy.info("[u_obj = " + str(u_obj) + ",	u_traj = " + str(u_traj) + ",	u_subtraj = " + str(u_subtraj) + "]" + ",	UpdateSampling = " + str(round(duration_UpdateSampling)) + " sec,	InsertSampling = " + str(round(duration_InsertSampling)) + " sec,	DeleteSampling = " + str(round(duration_DeleteSampling)) + " sec")

			if sr_max_gain <= eps_ssa:
				if verbose:
					plpy.info("[m = " + str(m) + "	/	" + str(M) + " - Last]")

				#for d in SRGain_duration_values:
				#	plpy.execute(stats_Plan, ["SRGain", d])
				#for d in SampleSimilarity_duration_values:
				#	plpy.execute(stats_Plan, ["SampleSimilarity", d])

				break

			if verbose:
				plpy.info("[m = " + str(m) + "	/	" + str(M) + "]")

		#for d in SRGain_duration_values:
		#	plpy.execute(stats_Plan, ["SRGain", d])
		#for d in SampleSimilarity_duration_values:
		#	plpy.execute(stats_Plan, ["SampleSimilarity", d])

	#for d in SRGain_duration_values:
	#	plpy.execute(stats_Plan, ["SRGain", d])
	#for d in SampleSimilarity_duration_values:
	#	plpy.execute(stats_Plan, ["SampleSimilarity", d])

	plpy.execute("SELECT S2T_TemporalBufferSize('" + temporal_buffer_size_sca + "');")
	plpy.execute("SELECT S2T_SpatialBufferSize(" + str(spatial_buffer_size_sca) + ");")

	ClusterCandidates_Plan = plpy.prepare("SELECT obj_id, traj_id, subtraj_id, avg_voting FROM S2T_ClusterCandidates($1, $2, $3, $4, $5);", ["integer", "integer", "integer", "text", "text"])
	PrevSCAVoting_Plan = plpy.prepare("SELECT sca_voting FROM " + plpy.quote_ident(D + "_subtraj") + " WHERE (obj_id, traj_id, subtraj_id) = ($1, $2, $3);", ["integer", "integer", "integer"])
	SetCluster_Plan = plpy.prepare("UPDATE " + plpy.quote_ident(D + "_subtraj") + " SET clust_id = $4, sca_voting = $5 WHERE (obj_id, traj_id, subtraj_id) = ($1, $2, $3);", ["integer", "integer", "integer", "integer", "double precision"])

	rv = plpy.execute("SELECT obj_id, traj_id, subtraj_id, clust_id FROM " + plpy.quote_ident(D + "_subtraj") + " WHERE subtraj_sample = true;")
	NrSubtrajectories = len(rv)
	for i, v in enumerate(rv):
		with plpy.subtransaction():
			t1 = time.time()
			rv2 = plpy.execute(ClusterCandidates_Plan, [v["obj_id"], v["traj_id"], v["subtraj_id"], D, data_source])
			t2 = time.time()
			duration_ClusterCandidates = t2 - t1

			for i2, v2 in enumerate(rv2):
				if v2["avg_voting"] > eps_sca:
					rv3 = plpy.execute(PrevSCAVoting_Plan, [v2["obj_id"], v2["traj_id"], v2["subtraj_id"]])

					if (rv3[0]["sca_voting"] is None) or (v2["avg_voting"] > rv3[0]["sca_voting"]):
						plpy.execute(SetCluster_Plan, [v2["obj_id"], v2["traj_id"], v2["subtraj_id"], v["clust_id"], v2["avg_voting"]])

			if verbose:
				plpy.info(str(round((i + 1) * 100 / NrSubtrajectories, 1)) + "%,	" + str(i + 1) + "	/	" + str(NrSubtrajectories) + ",	[" + str(v["obj_id"]) + ",	" + str(v["traj_id"]) + ",	" + str(v["subtraj_id"]) + "]")

		#plpy.execute(stats_Plan, ["ClusterCandidates", duration_ClusterCandidates])

	plpy.execute("SET enable_seqscan = on;")

	if verbose:
		plpy.info("S2T-Clustering end!")
except Exception as ex:
	import traceback
	plpy.notice(traceback.format_exc())
$$ LANGUAGE plpython3u STRICT;

/***********************************************/
CREATE OR REPLACE FUNCTION S2T_vsrelt(voting_signal) RETURNS TABLE(obj_id integer, traj_id integer, subtraj_id integer, subtraj_upper integer, seg_id integer, normalized_voting double precision, normalized_lifespan interval) AS $$
	SELECT
		$1.obj_id, $1.traj_id,
		$1.subtraj_id, j,
		$1.seg_id[i], $1.normalized_voting[i], $1.normalized_lifespan[i]
	FROM generate_series(array_lower($1.seg_id, 1), array_upper($1.seg_id, 1)) AS i, generate_series(array_lower($1.subtraj_upper, 1), array_upper($1.subtraj_upper, 1)) AS j
	WHERE COALESCE($1.subtraj_upper[j - 1], 0) < i AND i <= $1.subtraj_upper[j];
$$ LANGUAGE sql STRICT;

/***********************************************/
CREATE OR REPLACE FUNCTION S2T_GVA(obj_id integer, traj_id integer, subtraj_id integer, D text, data_source text) RETURNS voting_signal AS $$
DECLARE
	vs voting_signal;

	NN_method text;
	core_script text;

	temporal_buffer_size interval;
	spatial_buffer_size integer;
BEGIN
	SELECT value INTO NN_method FROM S2T_Parameters WHERE key = 'NN_method';

	IF NN_method = 'PostGIS A' OR NN_method = 'PostGIS B' OR NN_method = 'Hermes A' OR NN_method = 'Hermes B' THEN
		SELECT value INTO temporal_buffer_size FROM S2T_Parameters WHERE key = 'temporal_buffer_size';
		SELECT value INTO spatial_buffer_size FROM S2T_Parameters WHERE key = 'spatial_buffer_size';

		IF NN_method = 'PostGIS A' THEN
			core_script :=
				'
				SELECT odb1.obj_id AS ek_obj_id, odb1.traj_id AS ek_traj_id, odb1.seg_id AS ek_seg_id, min(ST_ZMax(odb1.seg_pgis) - ST_ZMin(odb1.seg_pgis)) AS ek_seg_lspan, count(odb2.seg_pgis) AS voting
				FROM
						(
							SELECT obj_id, traj_id, seg_id, seg_pgis FROM ONLY ' || quote_ident(D || '_seg')::regclass || ' WHERE (obj_id, traj_id) = ($1, $2)
						) AS odb1
					LEFT JOIN
						ONLY ' || quote_ident(D || '_seg')::regclass || ' AS odb2
					ON
						odb2.seg_pgis
							&&&
						ST_MakeLine(ST_MakePoint(ST_XMin(odb1.seg_pgis) - $4, ST_YMin(odb1.seg_pgis) - $4, ST_ZMin(odb1.seg_pgis) - extract(epoch FROM $3)), ST_MakePoint(ST_XMax(odb1.seg_pgis) + $4, ST_YMax(odb1.seg_pgis) + $4, ST_ZMax(odb1.seg_pgis) + extract(epoch FROM $3)))
					AND
						((ST_XMin(odb1.seg_pgis) <= ST_XMin(odb2.seg_pgis) AND ST_XMin(odb2.seg_pgis) <= ST_XMax(odb1.seg_pgis)) OR (ST_XMin(odb2.seg_pgis) <= ST_XMin(odb1.seg_pgis) AND ST_XMin(odb1.seg_pgis) <= ST_XMax(odb2.seg_pgis)))
					AND
						((ST_YMin(odb1.seg_pgis) <= ST_YMin(odb2.seg_pgis) AND ST_YMin(odb2.seg_pgis) <= ST_YMax(odb1.seg_pgis)) OR (ST_YMin(odb2.seg_pgis) <= ST_YMin(odb1.seg_pgis) AND ST_YMin(odb1.seg_pgis) <= ST_YMax(odb2.seg_pgis)))
					AND
						((ST_ZMin(odb1.seg_pgis) <= ST_ZMin(odb2.seg_pgis) AND ST_ZMin(odb2.seg_pgis) <= ST_ZMax(odb1.seg_pgis)) OR (ST_ZMin(odb2.seg_pgis) <= ST_ZMin(odb1.seg_pgis) AND ST_ZMin(odb1.seg_pgis) <= ST_ZMax(odb2.seg_pgis)))
				GROUP BY odb1.obj_id, odb1.traj_id, odb1.seg_id
				';
		ELSIF NN_method = 'PostGIS B' THEN
			core_script :=
				'
				SELECT odb1.obj_id AS ek_obj_id, odb1.traj_id AS ek_traj_id, odb1.seg_id AS ek_seg_id, min(ST_ZMax(odb1.seg_pgis) - ST_ZMin(odb1.seg_pgis)) AS ek_seg_lspan, count(odb2.seg_pgis) AS voting
				FROM
						(
							SELECT obj_id, traj_id, seg_id, seg_pgis FROM ONLY ' || quote_ident(D || '_seg')::regclass || ' WHERE (obj_id, traj_id) = ($1, $2)
						) AS odb1
					LEFT JOIN
						(
							SELECT seg_pgis
							FROM ONLY ' || quote_ident(D || '_seg')::regclass || '
							WHERE seg_pgis &&& (SELECT mbb_diagonal_pgis FROM ' || quote_ident(D || '_traj')::regclass || ' WHERE (obj_id, traj_id) = ($1, $2))
						) AS odb2
					ON
						ST_MakeLine(ST_MakePoint(ST_XMin(odb2.seg_pgis), ST_YMin(odb2.seg_pgis), ST_ZMin(odb2.seg_pgis)), ST_MakePoint(ST_XMax(odb2.seg_pgis), ST_YMax(odb2.seg_pgis), ST_ZMax(odb2.seg_pgis)))
							&&&
						ST_MakeLine(ST_MakePoint(ST_XMin(odb1.seg_pgis) - $4, ST_YMin(odb1.seg_pgis) - $4, ST_ZMin(odb1.seg_pgis) - extract(epoch FROM $3)), ST_MakePoint(ST_XMax(odb1.seg_pgis) + $4, ST_YMax(odb1.seg_pgis) + $4, ST_ZMax(odb1.seg_pgis) + extract(epoch FROM $3)))
					AND
						((ST_XMin(odb1.seg_pgis) <= ST_XMin(odb2.seg_pgis) AND ST_XMin(odb2.seg_pgis) <= ST_XMax(odb1.seg_pgis)) OR (ST_XMin(odb2.seg_pgis) <= ST_XMin(odb1.seg_pgis) AND ST_XMin(odb1.seg_pgis) <= ST_XMax(odb2.seg_pgis)))
					AND
						((ST_YMin(odb1.seg_pgis) <= ST_YMin(odb2.seg_pgis) AND ST_YMin(odb2.seg_pgis) <= ST_YMax(odb1.seg_pgis)) OR (ST_YMin(odb2.seg_pgis) <= ST_YMin(odb1.seg_pgis) AND ST_YMin(odb1.seg_pgis) <= ST_YMax(odb2.seg_pgis)))
					AND
						((ST_ZMin(odb1.seg_pgis) <= ST_ZMin(odb2.seg_pgis) AND ST_ZMin(odb2.seg_pgis) <= ST_ZMax(odb1.seg_pgis)) OR (ST_ZMin(odb2.seg_pgis) <= ST_ZMin(odb1.seg_pgis) AND ST_ZMin(odb1.seg_pgis) <= ST_ZMax(odb2.seg_pgis)))
				GROUP BY odb1.obj_id, odb1.traj_id, odb1.seg_id
				';
		ELSIF NN_method = 'Hermes A' THEN
			core_script :=
				'
					SELECT odb1.obj_id AS ek_obj_id, odb1.traj_id AS ek_traj_id, odb1.subtraj_id AS ek_subtraj_id, odb1.seg_id AS ek_seg_id, odb1.ek_seg_lspan ek_seg_lspan, COALESCE(sum(distance), 0) AS voting, count(distance) count_dist
					FROM
						(	
							SELECT odb1.obj_id, odb1.traj_id, odb1.subtraj_id, odb1.seg_id, min(extract(epoch from duration(getT(odb1.seg)))) AS ek_seg_lspan, min(Trapezoidal(odb1.seg, odb2.seg)) AS distance
							FROM
								(
									SELECT obj_id, traj_id, subtraj_id, seg_id, seg FROM ONLY ' || quote_ident(D || '_seg')::regclass || ' WHERE (obj_id, traj_id, subtraj_id) = ($1, $2, $3)
								) AS odb1
							LEFT JOIN
								(
									SELECT obj_id, traj_id, subtraj_id, seg_id, seg
									FROM ONLY ' || quote_ident(D || '_seg')::regclass || '
									WHERE seg &&& (SELECT buffer(trajectory_agg(seg ORDER BY getTi(seg) ASC)::BoxST, $4, $5, $5) FROM ' || quote_ident(D || '_seg')::regclass || ' WHERE (obj_id, traj_id, subtraj_id) = ($1, $2, $3))
								) AS odb2
							ON
								odb2.seg
									&&
								buffer(odb1.seg::BoxST, $4, $5, $5) AND odb1.obj_id <> odb2.obj_id
							GROUP BY odb1.obj_id, odb1.traj_id, odb1.subtraj_id, odb1.seg_id, odb2.obj_id, odb2.traj_id, odb2.subtraj_id
					) odb1
				GROUP BY odb1.obj_id, odb1.traj_id, odb1.subtraj_id, odb1.seg_id, odb1.ek_seg_lspan
				';
		ELSE
			core_script :=
				'
				SELECT odb1.obj_id AS ek_obj_id, odb1.traj_id AS ek_traj_id, odb1.seg_id AS ek_seg_id, min(extract(epoch from duration(getT(odb1.seg)))) AS ek_seg_lspan, count(odb2.seg) AS voting
				FROM
						(
							SELECT obj_id, traj_id, seg_id, seg FROM ONLY ' || quote_ident(D || '_seg')::regclass || ' WHERE (obj_id, traj_id) = ($1, $2)
						) AS odb1
					LEFT JOIN
						(
							SELECT seg
							FROM ONLY ' || quote_ident(D || '_seg')::regclass || '
							WHERE seg &&& (SELECT mbb FROM ' || quote_ident(D || '_traj')::regclass || ' WHERE (obj_id, traj_id) = ($1, $2))
						) AS odb2
					ON
						intersects(odb2.seg::BoxST, buffer(odb1.seg::BoxST, $3, $4, $4))
--						intersects(odb2.seg::BoxST, BoxST(getTi(odb1.seg) - $3, getTe(odb1.seg) + $3, getLx(getSp(odb1.seg)::BoxSP) - $4, getLy(getSp(odb1.seg)::BoxSP) - $4, getHx(getSp(odb1.seg)::BoxSP) + $4, getHy(getSp(odb1.seg)::BoxSP) + $4))
				GROUP BY odb1.obj_id, odb1.traj_id, odb1.seg_id
				';
		END IF;

		EXECUTE
		'
			SELECT tmp.ek_obj_id, tmp.ek_traj_id, tmp.ek_subtraj_id, NULL, array_agg(tmp.ek_seg_id ORDER BY tmp.ek_seg_id ASC), array_agg(tmp.normalized_voting ORDER BY tmp.ek_seg_id ASC), array_agg(tmp.normalized_lifespan ORDER BY tmp.ek_seg_id ASC)
			FROM
				(
					SELECT tmp.ek_obj_id, tmp.ek_traj_id, tmp.ek_subtraj_id, tmp.ek_seg_id, tmp.count_dist - tmp.voting::double precision / dd.max_voting AS normalized_voting, ''1 second''::interval * (CASE WHEN (sum(tmp.ek_seg_lspan) OVER (PARTITION BY tmp.ek_obj_id, tmp.ek_traj_id)) > 0 THEN tmp.ek_seg_lspan / sum(tmp.ek_seg_lspan) OVER (PARTITION BY tmp.ek_obj_id, tmp.ek_traj_id) ELSE 0 END) AS normalized_lifespan
					FROM 
					(
						SELECT length(SegmentSP(lx,ly, hx,hy)) max_voting FROM hdatasets_statistics WHERE dataset = HDatasetID(''' || data_source || ''')
					) dd,
					(' || core_script || ') AS tmp
				) AS tmp
			GROUP BY tmp.ek_obj_id, tmp.ek_traj_id, tmp.ek_subtraj_id;
		'
		INTO vs
		USING obj_id, traj_id, subtraj_id, temporal_buffer_size, spatial_buffer_size, 2 * power(S2T_Sigma(), 2);
	ELSE
		EXECUTE 'SELECT 1 FROM ONLY ' || quote_ident(D || '_seg')::regclass || ' WHERE seg &&& (SELECT trajectory_agg(seg ORDER BY getTi(seg) ASC) FROM ONLY ' || quote_ident(D || '_seg')::regclass || ' WHERE (obj_id, traj_id) = ($1, $2));' USING obj_id, traj_id;

		SELECT obj_id, traj_id, NULL, array_agg(ordinality ORDER BY ordinality ASC), array_agg(normalized_voting ORDER BY ordinality ASC), array_agg(normalized_lifespan ORDER BY ordinality ASC)
		INTO vs
		FROM S2T_VotingSignal() WITH ORDINALITY;
	END IF;

	RETURN vs;

END;
$$ LANGUAGE plpgsql STRICT;

/***********************************************/
CREATE OR REPLACE FUNCTION S2T_TSA(vs voting_signal, w integer, tau double precision) RETURNS voting_signal AS $$
DECLARE
	nr_s integer;

	d double precision[];

	subtraj_upper integer[];
	LPk integer;
	prev_subtraj_upper integer;

	mean_1 double precision;
	mean_2 double precision;
	variance_1 double precision;
	variance_2 double precision;
	ten_sixteen double precision := power(10, -16);

	d_max double precision;
BEGIN
	nr_s := array_length(vs.seg_id, 1);

	FOR n IN (w + 1)..(nr_s - w) LOOP
		mean_1 := (SELECT sum(vs.normalized_voting[i]) FROM generate_series(n - w, n - 1) AS i) / w;
		mean_2 := (SELECT sum(vs.normalized_voting[i]) FROM generate_series(n, n + w - 1) AS i) / w;

		variance_1 := (SELECT sum(power(vs.normalized_voting[i] - mean_1, 2)) FROM generate_series(n - w, n - 1) AS i) / w;
		variance_2 := (SELECT sum(power(vs.normalized_voting[i] - mean_2, 2)) FROM generate_series(n, n + w - 1) AS i) / w;

		IF variance_1 = 0 THEN
			variance_1 := ten_sixteen;
		END IF;
		IF variance_2 = 0 THEN
			variance_2 := ten_sixteen;
		END IF;

		d[n] := power(mean_1 - mean_2, 2) * (1 / variance_1 + 1 / variance_2);
	END LOOP;

	LPk := 1;
	prev_subtraj_upper := 0;

	FOR n IN (w + 1)..(nr_s - w) LOOP
		SELECT max(d[i]) INTO d_max FROM generate_series(n - w, n + w) AS i;

		IF d[n] > tau AND d[n] >= d_max AND (n - 1) - prev_subtraj_upper >= w THEN
			subtraj_upper[LPk] := n - 1;

			prev_subtraj_upper := n - 1;
			LPk := LPk + 1;
		END IF;
	END LOOP;

	IF subtraj_upper[array_upper(subtraj_upper, 1)] < nr_s THEN
		subtraj_upper[array_upper(subtraj_upper, 1) + 1] := nr_s;
	END IF;

	IF COALESCE(array_length(subtraj_upper, 1), 0) = 0 THEN
		subtraj_upper[1] := nr_s;
	END IF;

	vs.subtraj_upper := subtraj_upper;

	RETURN vs;
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

/***********************************************/
CREATE OR REPLACE FUNCTION S2T_SRGain(obj_id integer, traj_id integer, subtraj_id integer, D text, data_source text) RETURNS double precision AS $$
DECLARE
	sr_gain double precision;
	core_script text;

BEGIN
	
	
	core_script :=
		'
			
		SELECT tmp.ek_obj_id, tmp.ek_traj_id, tmp.ek_subtraj_id, tmp.ek_seg_id, tmp.count_dist - tmp.voting::double precision / dd.max_voting AS normalized_voting
		FROM
		(
			SELECT length(SegmentSP(lx,ly, hx,hy)) max_voting FROM hdatasets_statistics WHERE dataset = HDatasetID(''' || data_source || ''')
		) dd,
		(
			SELECT odb1.c_obj_id ek_obj_id, odb1.c_traj_id ek_traj_id, odb1.c_subtraj_id ek_subtraj_id, odb1.c_seg_id ek_seg_id, COALESCE(sum(distance), 0) AS voting, count(distance) count_dist
			FROM
			(
					SELECT s.c_obj_id c_obj_id, s.c_traj_id c_traj_id, sv.subtraj_id c_subtraj_id, s.c_seg_id c_seg_id, s.s_obj_id s_obj_id, s.s_traj_id s_traj_id, sv.subtraj_id s_subtraj_id,  min(Trapezoidal(s.c_seg, s.s_seg)) AS distance
					FROM
						(
							SELECT odb1.obj_id c_obj_id, odb1.traj_id c_traj_id, odb1.seg_id c_seg_id, odb2.obj_id s_obj_id, odb2.traj_id s_traj_id, odb1.seg c_seg, odb2.seg s_seg
							FROM
								(
									SELECT s.obj_id, s.traj_id, s.seg_id, s.seg 
									FROM ONLY ' || quote_ident(D || '_seg')::regclass || ' s
									WHERE (s.obj_id, s.traj_id) = ($1, $2) AND seg_id IN (
																SELECT seg_id 
																FROM ' || quote_ident(D || '_seg_voting')::regclass || '
																WHERE (obj_id, traj_id, subtraj_id) = ($1, $2, $3)
															)
								) AS odb1
							LEFT JOIN
								(
									SELECT s.obj_id, s.traj_id, s.seg_id, s.seg
									FROM ONLY ' || quote_ident(D || '_seg_sample')::regclass || ' s
									WHERE seg &&& (SELECT buffer(trajectory_agg(seg ORDER BY getTi(seg) ASC)::BoxST, $4, $5, $5) FROM ' || quote_ident(D || '_seg')::regclass || ' WHERE (obj_id, traj_id) = ($1, $2) AND seg_id IN (SELECT seg_id FROM ' || quote_ident(D || '_seg_voting')::regclass || ' WHERE (obj_id, traj_id, subtraj_id) = ($1, $2, $3))) 
								) AS odb2
							ON
								odb2.seg && buffer(odb1.seg::BoxST, $4, $5, $5)
						) s 
					INNER JOIN
						' || quote_ident(D || '_seg_voting')::regclass || ' sv 
					ON (s.c_obj_id, s.c_traj_id, s.c_seg_id) = (sv.obj_id, sv.traj_id, sv.seg_id)
					GROUP BY s.c_obj_id, s.c_traj_id, sv.subtraj_id, s.c_seg_id, s.s_obj_id, s.s_traj_id, sv.subtraj_id 
			) odb1
			GROUP BY odb1.c_obj_id, odb1.c_traj_id, odb1.c_subtraj_id, odb1.c_seg_id
		) tmp
			
		';
		
	EXECUTE
	'
		SELECT sum(tcdb.normalized_voting * extract(epoch FROM tcdb.normalized_lifespan) * (1 - (tmp.normalized_voting)))
		FROM
				ONLY ' || quote_ident(D || '_seg_voting')::regclass || ' AS tcdb
			INNER JOIN
				(' || core_script || ') AS tmp
			ON (tcdb.obj_id, tcdb.traj_id, tcdb.seg_id) = ($1, $2, tmp.ek_seg_id);
	'
	INTO sr_gain
	USING obj_id, traj_id, subtraj_id, S2T_TemporalBufferSize(), S2T_SpatialBufferSize(), 2 * power(S2T_Sigma(), 2);

	RETURN sr_gain;
END;
$$ LANGUAGE plpgsql STRICT;

/***********************************************/
CREATE OR REPLACE FUNCTION S2T_SampleSimilarity(obj_id integer, traj_id integer, subtraj_id integer, D text, data_source text) RETURNS double precision AS $$
DECLARE
	similarity double precision;
BEGIN

	EXECUTE
	'
		SELECT COALESCE(round(2*sum_voting::numeric / (candidate.NrSegments + N.NrSegments), 15)::double precision, 0) avg_voting
		FROM 
			(
				SELECT NrSegments FROM ' || quote_ident(D || '_subtraj')::regclass || ' WHERE (obj_id, traj_id, subtraj_id) = ($1, $2, $3) 
			) candidate,
			(
				SELECT s_obj_id, s_traj_id, s_subtraj_id, o_obj_id, o_traj_id, o_subtraj_id, count(distance) - COALESCE(sum(distance)/dd.max_voting, 0) AS sum_voting
				FROM
					(
						SELECT length(SegmentSP(lx,ly, hx,hy)) max_voting FROM hdatasets_statistics WHERE dataset = HDatasetID(''' || data_source || ''')
					) dd,
					(
						SELECT s.obj_id s_obj_id, s.traj_id s_traj_id, s.subtraj_id s_subtraj_id, s.seg_id s_seg_id, o.obj_id o_obj_id, o.traj_id o_traj_id, o.subtraj_id o_subtraj_id,  min(Trapezoidal(o.seg, s.seg)) AS distance, count(o.seg)
						FROM
							(
								SELECT obj_id, traj_id, $3 AS subtraj_id, seg_id, seg FROM ONLY ' || quote_ident(D || '_seg')::regclass || ' WHERE (obj_id, traj_id) = ($1, $2) AND seg_id IN (SELECT seg_id FROM ' || quote_ident(D || '_seg_voting')::regclass || ' WHERE (obj_id, traj_id, subtraj_id) = ($1, $2, $3))
							) AS s
						INNER JOIN
							(
								SELECT ss.obj_id, ss.traj_id, sv.subtraj_id, ss.seg_id, ss.seg FROM ' || quote_ident(D || '_seg_sample')::regclass || ' ss INNER JOIN ' || quote_ident(D || '_seg_voting')::regclass || ' sv ON  (ss.obj_id, ss.traj_id,  ss.seg_id)=(sv.obj_id, sv.traj_id,  sv.seg_id)
								WHERE ss.seg &&& (SELECT buffer(trajectory_agg(seg ORDER BY getTi(seg) ASC)::BoxST, $4, $5, $5) FROM ' || quote_ident(D || '_seg')::regclass || ' WHERE (obj_id, traj_id) = ($1, $2) AND seg_id IN (SELECT seg_id FROM ' || quote_ident(D || '_seg_voting')::regclass || ' WHERE (obj_id, traj_id, subtraj_id) = ($1, $2, $3))) 
							) AS o
							
						ON
							o.seg && buffer(s.seg::BoxST, $4, $5, $5)
						GROUP BY s.obj_id, s.traj_id, s.subtraj_id, s.seg_id, o.obj_id, o.traj_id, o.subtraj_id
						ORDER BY s.obj_id, s.traj_id, s.subtraj_id, s.seg_id, o.obj_id, o.traj_id, o.subtraj_id

					) AS tmp1
				--WHERE tmp1.distance > 0
				GROUP BY s_obj_id, s_traj_id, s_subtraj_id, o_obj_id, o_traj_id, o_subtraj_id, dd.max_voting
			) AS J
		INNER JOIN
			' || quote_ident(D || '_subtraj')::regclass || ' AS N
		ON (J.o_obj_id, J.o_traj_id, J.o_subtraj_id) = (N.obj_id, N.traj_id, N.subtraj_id)
		ORDER BY avg_voting DESC
		LIMIT 1;

	'
	INTO similarity
	USING obj_id, traj_id, subtraj_id, S2T_TemporalBufferSize(), S2T_SpatialBufferSize(), 2 * power(S2T_Sigma(), 2);


	RETURN similarity;
END;
$$ LANGUAGE plpgsql STRICT;

/***********************************************/
CREATE OR REPLACE FUNCTION S2T_ClusterCandidates(obj_id_in integer, traj_id_in integer, subtraj_id_in integer, D text, data_source text)
RETURNS TABLE (obj_id integer, traj_id integer, subtraj_id integer, avg_voting double precision) AS $$
DECLARE
	voting_method text;
BEGIN
	voting_method := S2T_VotingMethod();

	PERFORM S2T_VotingMethodNone();

	IF voting_method = 'Trapezoidal' THEN
		
		--SELECT J.obj_id, J.traj_id, J.subtraj_id, round(2*sum_voting::numeric / (repr.NrSegments + N.NrSegments), 15)::double precision
		
		RETURN QUERY EXECUTE
		'
			SELECT J.obj_id, J.traj_id, J.subtraj_id, round(2*sum_voting::numeric / (repr.NrSegments + N.NrSegments), 15)::double precision
			FROM
					(SELECT NrSegments FROM ' || quote_ident(D || '_subtraj')::regclass || ' WHERE (obj_id, traj_id, subtraj_id) = ($1, $2, $3)) repr,					
				(
					SELECT tmp1.obj_id, tmp1.traj_id, tmp2.subtraj_id, count(distance) - COALESCE(sum(distance)/dd.max_voting, 0) sum_voting
					FROM
						(
							SELECT length(SegmentSP(lx,ly, hx,hy)) max_voting FROM hdatasets_statistics WHERE dataset = HDatasetID(''' || data_source || ''')
						) dd,
						(
							SELECT obj_id, traj_id, seg_id, min(Trapezoidal(o.seg, s.seg)) AS distance
							FROM
								(
									SELECT seg FROM ' || quote_ident(D || '_seg_sample')::regclass || ' WHERE (obj_id, traj_id) = ($1, $2) AND seg_id IN (SELECT seg_id FROM ' || quote_ident(D || '_seg_voting')::regclass || ' WHERE (obj_id, traj_id, subtraj_id) = ($1, $2, $3))
								) AS s
							INNER JOIN
								(
									SELECT obj_id, traj_id, seg_id, seg
									FROM ONLY ' || quote_ident(D || '_seg')::regclass || '
									WHERE seg &&& (SELECT buffer(trajectory_agg(seg ORDER BY getTi(seg) ASC)::BoxST, $4, $5, $5) FROM ' || quote_ident(D || '_seg_sample')::regclass || ' WHERE (obj_id, traj_id) = ($1, $2) AND seg_id IN (SELECT seg_id FROM ' || quote_ident(D || '_seg_voting')::regclass || ' WHERE (obj_id, traj_id, subtraj_id) = ($1, $2, $3)))
								) AS o
							ON
								o.seg && buffer(s.seg::BoxST, $4, $5, $5)
							GROUP BY obj_id, traj_id, seg_id
						) AS tmp1
					INNER JOIN
						' || quote_ident(D || '_seg_voting')::regclass || ' AS tmp2
					ON
						(tmp1.obj_id, tmp1.traj_id, tmp1.seg_id) = (tmp2.obj_id, tmp2.traj_id, tmp2.seg_id)
					--WHERE tmp1.distance > 0
					GROUP BY tmp1.obj_id, tmp1.traj_id, tmp2.subtraj_id, dd.max_voting
				) AS J
			INNER JOIN
				' || quote_ident(D || '_subtraj')::regclass || ' AS N
			ON (J.obj_id, J.traj_id, J.subtraj_id) = (N.obj_id, N.traj_id, N.subtraj_id);
		'
		USING obj_id_in, traj_id_in, subtraj_id_in, S2T_TemporalBufferSize(), S2T_SpatialBufferSize(), 2 * power(S2T_Sigma(), 2);
	ELSE
		RETURN QUERY EXECUTE
		'
			SELECT J.obj_id, J.traj_id, J.subtraj_id, round(2*voting / (repr.NrSegments + N.NrSegments), 15)::double precision
			FROM
					(SELECT NrSegments FROM ' || quote_ident(D || '_subtraj')::regclass || ' WHERE (obj_id, traj_id, subtraj_id) = ($1, $2, $3)) repr,					
				(
					SELECT tmp1.obj_id, tmp1.traj_id, tmp2.subtraj_id, sum(tmp1.count_seg)::numeric AS voting
					FROM
						(
							SELECT DISTINCT obj_id, traj_id, seg_id, count(s.seg) count_seg
							FROM
								(
									SELECT seg FROM ' || quote_ident(D || '_seg_sample')::regclass || ' WHERE (obj_id, traj_id) = ($1, $2) AND seg_id IN (SELECT seg_id FROM ' || quote_ident(D || '_seg_voting')::regclass || ' WHERE (obj_id, traj_id, subtraj_id) = ($1, $2, $3))
								) AS s
							INNER JOIN
								(
									SELECT obj_id, traj_id, seg_id, seg
									FROM ONLY ' || quote_ident(D || '_seg')::regclass || '
									WHERE seg &&& (SELECT buffer(trajectory_agg(seg ORDER BY getTi(seg) ASC)::BoxST, $4, $5, $5) FROM ' || quote_ident(D || '_seg_sample')::regclass || ' WHERE (obj_id, traj_id) = ($1, $2) AND seg_id IN (SELECT seg_id FROM ' || quote_ident(D || '_seg_voting')::regclass || ' WHERE (obj_id, traj_id, subtraj_id) = ($1, $2, $3)))
								) AS o
							ON
								o.seg &&& buffer(s.seg::BoxST, $4, $5, $5)
							GROUP BY obj_id, traj_id, seg_id
						) AS tmp1
					INNER JOIN
						' || quote_ident(D || '_seg_voting')::regclass || ' AS tmp2
					ON
						(tmp1.obj_id, tmp1.traj_id, tmp1.seg_id) = (tmp2.obj_id, tmp2.traj_id, tmp2.seg_id)
					GROUP BY tmp1.obj_id, tmp1.traj_id, tmp2.subtraj_id
				) AS J
			INNER JOIN
				' || quote_ident(D || '_subtraj')::regclass || ' AS N
			ON (J.obj_id, J.traj_id, J.subtraj_id) = (N.obj_id, N.traj_id, N.subtraj_id);
		'
		USING obj_id_in, traj_id_in, subtraj_id_in, S2T_TemporalBufferSize(), S2T_SpatialBufferSize();
	END IF;

	PERFORM S2T_VotingMethod(voting_method);

	RETURN;
END;
$$ LANGUAGE plpgsql STRICT;
