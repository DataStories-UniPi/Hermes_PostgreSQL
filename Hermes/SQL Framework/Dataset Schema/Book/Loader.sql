/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief The file implements the loading function of datasets
 *
 * @see @ref dataset_ais
 *
 */

/******************************************************************************/
--How to update where cursor is when cursor is opened on a dynamic SQL query:
--EXECUTE 'UPDATE ' || quote_ident(dataset_name || '_input_data')::regclass || ' SET var = val WHERE CURRENT OF ' || quote_ident(c::text) || ';';
/******************************************************************************/

/** @brief The function loads dataset in the database from a file
 *
 *	@param[in] dataset_name the name of the database
 *	@param[in] csv_file the name of the file that the data resides 
 *	@param[in] keep_input_data boolean value showing if the input data will remain
 *
 */
CREATE FUNCTION HDatasetsLoad(dataset_name text, csv_file text DEFAULT '', keep_input_data boolean DEFAULT false) RETURNS TABLE(operation text) AS $$
DECLARE
	dataset_id integer;
	dataset_LRP PointLL;
	dataset_SRID integer;
	dataset_segment_storage boolean;
	dataset_trajectory_storage boolean;
	dataset_subtrajectory_storage boolean;
	dataset_semantics_enabled boolean;

	query_1 text;
	query_2 text;

	row_exists boolean;
	rcnt integer;

	curr_obj record;
	curr_traj record;

	c refcursor;

	traj_old Trajectory;
	traj Trajectory;
	traj_length_max integer;
	traj_cache_id integer;

	last_seg_id integer;

	subtraj_length_max integer;
	subtraj_cache_id integer;

	traj_exists boolean;
	seg_exists boolean;

	first_t timestamp;

	prev_subtraj_id integer;
	prev_t timestamp;
	prev_x integer;
	prev_y integer;
	prev_episode text;
	prev_tags text;

	curr_row record;
	curr_x integer;
	curr_y integer;

BEGIN
	SELECT id, LRP, SRID, segment_storage, trajectory_storage, subtrajectory_storage, semantics_enabled
		INTO dataset_id, dataset_LRP, dataset_SRID, dataset_segment_storage, dataset_trajectory_storage, dataset_subtrajectory_storage, dataset_semantics_enabled
	FROM HDatasets WHERE name = dataset_name;
	IF NOT FOUND THEN
		RETURN;
	END IF;
	
	IF csv_file <> '' THEN
		EXECUTE 'DROP TABLE IF EXISTS ' || quote_ident(dataset_name || '_input_data') || ' CASCADE;';
		
		query_1 := 'CREATE UNLOGGED TABLE ' || quote_ident(dataset_name || '_input_data') ||
			'(
				obj_id integer NOT NULL,
				traj_id integer NOT NULL';
		query_2 := '(obj_id, traj_id';

		IF dataset_subtrajectory_storage THEN
			query_1 := query_1 || ',subtraj_id integer NOT NULL';
			query_2 := query_2 || ', subtraj_id';
		END IF;

		query_1 := query_1 || ',t timestamp NOT NULL,
			lon double precision NOT NULL,
			lat double precision NOT NULL';
		query_2 := query_2 || ', t, lon, lat';

		IF dataset_semantics_enabled THEN
			query_1 := query_1 || ',episode text,
				tags text';
			query_2 := query_2 || ', episode, tags';
		END IF;

		query_1 := query_1 || ');';
		query_2 := query_2 || ')';

		EXECUTE query_1;
		
		EXECUTE
			'
				COPY ' || quote_ident(dataset_name || '_input_data')::regclass || query_2 || '
				FROM ''' || csv_file || '''
				WITH CSV HEADER;
			';

		operation := 'COPY TO TABLE';
		RETURN NEXT;
		
		EXECUTE 'CREATE INDEX ON ' || quote_ident(dataset_name || '_input_data')::regclass || ' (obj_id, traj_id) WITH (FILLFACTOR = 100);';
	END IF;

	IF dataset_trajectory_storage THEN
		EXECUTE 'SELECT max(nr_points) FROM (SELECT obj_id, traj_id, count(*) AS nr_points FROM ' || quote_ident(dataset_name || '_input_data')::regclass || ' GROUP BY obj_id, traj_id) AS tmp;' INTO traj_length_max;

		traj_cache_id := TrajCache_Allocate(traj_length_max + 1);
	END IF;

	IF dataset_subtrajectory_storage THEN
		EXECUTE 'SELECT max(nr_points) FROM (SELECT obj_id, traj_id, subtraj_id, count(*) AS nr_points FROM ' || quote_ident(dataset_name || '_input_data')::regclass || ' GROUP BY obj_id, traj_id, subtraj_id) AS tmp;' INTO subtraj_length_max;

		subtraj_cache_id := TrajCache_Allocate(subtraj_length_max + 1);
	END IF;

	query_1 := 't, lon, lat';
	IF dataset_subtrajectory_storage THEN
		query_1 := query_1 || ', subtraj_id';

		IF dataset_semantics_enabled THEN
			query_1 := query_1 || ', episode, tags';
		END IF;
	END IF;

	FOR curr_obj IN EXECUTE 'SELECT DISTINCT obj_id FROM ' || quote_ident(dataset_name || '_input_data')::regclass || ';' LOOP
		EXECUTE 'SELECT exists(SELECT 1 FROM ' || quote_ident(dataset_name || '_obj')::regclass || ' WHERE obj_id = $1 LIMIT 1);' INTO row_exists USING curr_obj.obj_id;

		IF NOT row_exists THEN
			EXECUTE 'INSERT INTO ' || quote_ident(dataset_name || '_obj')::regclass || '(obj_id) VALUES ($1);' USING curr_obj.obj_id;
		END IF;

		FOR curr_traj IN EXECUTE 'SELECT DISTINCT traj_id FROM ' || quote_ident(dataset_name || '_input_data')::regclass || ' WHERE obj_id = $1;' USING curr_obj.obj_id LOOP
			OPEN c FOR EXECUTE 'SELECT DISTINCT ON (t) ' || query_1 || ' FROM ' || quote_ident(dataset_name || '_input_data')::regclass || ' WHERE (obj_id, traj_id) = ($1, $2) ORDER BY t ASC;' USING curr_obj.obj_id, curr_traj.traj_id;

			FETCH c INTO curr_row;

			DECLARE
				xy PointSP;
			BEGIN
				IF dataset_LRP IS NOT NULL THEN
					xy := PointXY(PointLL(curr_row.lon, curr_row.lat), dataset_LRP)::PointSP;
				ELSE
					xy := PointSPFromGeom(ST_Transform(ST_SetSRID(ST_MakePoint(curr_row.lon, curr_row.lat), 4326), dataset_SRID));
				END IF;

				curr_x := getX(xy);
				curr_y := getY(xy);
			END;

			IF dataset_trajectory_storage THEN
				PERFORM TrajCache_ResetIndex(traj_cache_id);

				EXECUTE 'SELECT traj FROM ' || quote_ident(dataset_name || '_traj')::regclass || ' WHERE (obj_id, traj_id) = ($1, $2);' INTO traj_old USING curr_obj.obj_id, curr_traj.traj_id;
				GET DIAGNOSTICS rcnt = ROW_COUNT;

				IF rcnt > 0 THEN
					PERFORM TrajCache_Append(traj_cache_id, lastPoint(traj_old));

					traj_exists := true;
				ELSE
					traj_exists := false;
				END IF;

				PERFORM TrajCache_Append(traj_cache_id, PointST(curr_row.t, curr_x, curr_y));
			ELSE
				EXECUTE 'SELECT exists(SELECT 1 FROM ' || quote_ident(dataset_name || '_traj')::regclass || ' WHERE (obj_id, traj_id) = ($1, $2) LIMIT 1);' INTO traj_exists USING curr_obj.obj_id, curr_traj.traj_id;
			END IF;

			IF NOT traj_exists THEN
				EXECUTE 'INSERT INTO ' || quote_ident(dataset_name || '_traj')::regclass || '(obj_id, traj_id) VALUES ($1, $2);' USING curr_obj.obj_id, curr_traj.traj_id;
			END IF;

			IF dataset_segment_storage THEN
				seg_exists := false;

				EXECUTE
					'SELECT seg_id, getTe(seg), getEx(seg), getEy(seg) FROM ' || quote_ident(dataset_name || '_seg')::regclass || ' WHERE (obj_id, traj_id) = ($1, $2) ORDER BY seg_id DESC, getTe(seg) DESC LIMIT 1;'
				INTO last_seg_id, prev_t, prev_x, prev_y USING curr_obj.obj_id, curr_traj.traj_id;
				GET DIAGNOSTICS rcnt = ROW_COUNT;

				IF rcnt > 0 THEN
					seg_exists := true;
				ELSE
					last_seg_id := 0;

					EXECUTE
						'SELECT t, x, y FROM ' || quote_ident(dataset_name || '_seg_lobby')::regclass || ' WHERE (obj_id, traj_id) = ($1, $2);'
					INTO prev_t, prev_x, prev_y USING curr_obj.obj_id, curr_traj.traj_id;
					GET DIAGNOSTICS rcnt = ROW_COUNT;

					IF rcnt > 0 THEN
						EXECUTE
							'DELETE FROM ' || quote_ident(dataset_name || '_seg_lobby')::regclass || ' WHERE (obj_id, traj_id) = ($1, $2);'
						USING curr_obj.obj_id, curr_traj.traj_id;

						seg_exists := true;
					END IF;
				END IF;

				IF seg_exists THEN
					EXECUTE 'INSERT INTO ' || quote_ident(dataset_name || '_seg')::regclass || '(obj_id, traj_id, seg_id, seg) VALUES ($1, $2, $3, $4);' USING curr_obj.obj_id, curr_traj.traj_id, last_seg_id + 1, SegmentST(prev_t, prev_x, prev_y, curr_row.t, curr_x, curr_y);
					last_seg_id := last_seg_id + 1;
				ELSE
					first_t := curr_row.t;
				END IF;				
			END IF;

			IF dataset_subtrajectory_storage THEN
				PERFORM TrajCache_ResetIndex(subtraj_cache_id);
				
				DECLARE
					lastPoint PointST;
				BEGIN
					EXECUTE 'SELECT lastPoint(subtraj) FROM ' || quote_ident(dataset_name || '_subtraj')::regclass || ' WHERE (obj_id, traj_id) = ($1, $2) ORDER BY subtraj_id DESC LIMIT 1;' INTO lastPoint USING curr_obj.obj_id, curr_traj.traj_id;
					GET DIAGNOSTICS rcnt = ROW_COUNT;
					
					IF rcnt > 0 THEN
						PERFORM TrajCache_Append(subtraj_cache_id, lastPoint);
					END IF;
				END;
				
				PERFORM TrajCache_Append(subtraj_cache_id, PointST(curr_row.t, curr_x, curr_y));

				prev_subtraj_id := curr_row.subtraj_id;
				IF dataset_semantics_enabled THEN
					prev_episode := curr_row.episode;
					prev_tags := curr_row.tags;
				END IF;
			END IF;

			prev_t := curr_row.t;
			prev_x := curr_x;
			prev_y := curr_y;

			FETCH c INTO curr_row;
			WHILE FOUND LOOP
				DECLARE
					xy PointSP;
				BEGIN
					IF dataset_LRP IS NOT NULL THEN
						xy := PointXY(PointLL(curr_row.lon, curr_row.lat), dataset_LRP)::PointSP;
					ELSE
						xy := PointSPFromGeom(ST_Transform(ST_SetSRID(ST_MakePoint(curr_row.lon, curr_row.lat), 4326), dataset_SRID));
						--This query is significantly slower than the previous one: SELECT ST_X(geom), ST_Y(geom) INTO curr_x, curr_y FROM (SELECT ST_Transform(ST_SetSRID(ST_MakePoint(curr_row.lon, curr_row.lat), 4326), dataset_SRID) AS geom) AS tmp;
						--This query is twice slower than the one above: SELECT ST_X(ST_Transform(ST_SetSRID(ST_MakePoint(curr_row.lon, curr_row.lat), 4326), dataset_SRID)), ST_Y(ST_Transform(ST_SetSRID(ST_MakePoint(curr_row.lon, curr_row.lat), 4326), dataset_SRID)) INTO curr_x, curr_y;
					END IF;

					curr_x := getX(xy);
					curr_y := getY(xy);
				END;

				IF dataset_trajectory_storage THEN
					PERFORM TrajCache_Append(traj_cache_id, PointST(curr_row.t, curr_x, curr_y));
				END IF;

				IF dataset_segment_storage THEN
					EXECUTE 'INSERT INTO ' || quote_ident(dataset_name || '_seg')::regclass || '(obj_id, traj_id, seg_id, seg) VALUES ($1, $2, $3, $4);' USING curr_obj.obj_id, curr_traj.traj_id, last_seg_id + 1, SegmentST(prev_t, prev_x, prev_y, curr_row.t, curr_x, curr_y);

					last_seg_id := last_seg_id + 1;
				END IF;

				IF dataset_subtrajectory_storage THEN
					IF prev_subtraj_id <> curr_row.subtraj_id THEN
						IF dataset_semantics_enabled THEN
							EXECUTE 'INSERT INTO ' || quote_ident(dataset_name || '_subtraj')::regclass || '(obj_id, traj_id, subtraj_id, subtraj, episode, tags) VALUES ($1, $2, $3, $4, $5, $6);' USING curr_obj.obj_id, curr_traj.traj_id, prev_subtraj_id, TrajCache2Trajectory(subtraj_cache_id), prev_episode, prev_tags;
							
							prev_episode := curr_row.episode;
							prev_tags := curr_row.tags;
						ELSE
							EXECUTE 'INSERT INTO ' || quote_ident(dataset_name || '_subtraj')::regclass || '(obj_id, traj_id, subtraj_id, subtraj) VALUES ($1, $2, $3, $4);' USING curr_obj.obj_id, curr_traj.traj_id, prev_subtraj_id, TrajCache2Trajectory(subtraj_cache_id);
						END IF;

						PERFORM TrajCache_ResetIndex(subtraj_cache_id);
						PERFORM TrajCache_Append(subtraj_cache_id, PointST(prev_t, prev_x, prev_y));
					END IF;

					PERFORM TrajCache_Append(subtraj_cache_id, PointST(curr_row.t, curr_x, curr_y));

					prev_subtraj_id := curr_row.subtraj_id;
				END IF;

				prev_t := curr_row.t;
				prev_x := curr_x;
				prev_y := curr_y;

				FETCH c INTO curr_row;
			END LOOP;

			CLOSE c;

			IF dataset_trajectory_storage THEN
				traj := TrajCache2Trajectory(traj_cache_id);

				IF traj_old IS NOT NULL THEN
					traj := expand(traj_old, traj);
				END IF;

				EXECUTE 'UPDATE ' || quote_ident(dataset_name || '_traj')::regclass || ' SET traj = $3 WHERE (obj_id, traj_id) = ($1, $2);' USING curr_obj.obj_id, curr_traj.traj_id, traj;
			END IF;

			IF dataset_segment_storage THEN
				IF NOT seg_exists AND first_t = prev_t THEN
					EXECUTE
						'INSERT INTO ' || quote_ident(dataset_name || '_seg_lobby')::regclass || '(obj_id, traj_id, t, x, y) VALUES ($1, $2, $3, $4, $5);'
					USING curr_obj.obj_id, curr_traj.traj_id, prev_t, prev_x, prev_y;
				END IF;
			END IF;

			IF dataset_subtrajectory_storage THEN
				IF dataset_semantics_enabled THEN
					EXECUTE 'INSERT INTO ' || quote_ident(dataset_name || '_subtraj')::regclass || '(obj_id, traj_id, subtraj_id, subtraj, episode, tags) VALUES ($1, $2, $3, $4, $5, $6);' USING curr_obj.obj_id, curr_traj.traj_id, prev_subtraj_id, TrajCache2Trajectory(subtraj_cache_id), prev_episode, prev_tags;
				ELSE
					EXECUTE 'INSERT INTO ' || quote_ident(dataset_name || '_subtraj')::regclass || '(obj_id, traj_id, subtraj_id, subtraj) VALUES ($1, $2, $3, $4);' USING curr_obj.obj_id, curr_traj.traj_id, prev_subtraj_id, TrajCache2Trajectory(subtraj_cache_id);
				END IF;
			END IF;
		END LOOP;
	END LOOP;

	IF dataset_trajectory_storage OR dataset_subtrajectory_storage THEN
		PERFORM TrajCache_Clean();
	END IF;

	IF dataset_trajectory_storage THEN
		operation := 'INSERT NEW TRAJ';
		RETURN NEXT;

		operation := 'UPDATE TRAJ';
		RETURN NEXT;
	END IF;

	IF dataset_segment_storage THEN
		operation := 'SEARCH LAST SEG';
		RETURN NEXT;

		operation := 'INSERT NEW SEG';
		RETURN NEXT;
	END IF;

	IF dataset_subtrajectory_storage THEN
		operation := 'SEARCH LAST SUBTRAJ';
		RETURN NEXT;

		operation := 'INSERT NEW SUBTRAJ';
		RETURN NEXT;
	END IF;

	operation := 'OVERALL PROCESSING';
	RETURN NEXT;

	IF NOT keep_input_data THEN
		EXECUTE 'DROP TABLE ' || quote_ident(dataset_name || '_input_data')::regclass || ';';
	END IF;

	RETURN;
END;
$$ LANGUAGE plpgsql STRICT;
