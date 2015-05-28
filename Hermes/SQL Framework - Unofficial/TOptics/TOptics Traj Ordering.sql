/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
/*
900 trajectories from milan are clustered in 12 minutes approximately.
for milan_rev with 97030 trajectories total query runtime: 4452327 ms.

*/

/*
DROP TABLE IF EXISTS TOptics_Traj_NN_vars CASCADE;
CREATE TEMPORARY TABLE TOptics_Traj_NN_vars (
	key text NOT NULL,
	value text NOT NULL,

	PRIMARY KEY (key)
);

INSERT INTO TOptics_Traj_NN_vars(key, value) VALUES ('temporal_threshold', '30 minutes');

--INSERT INTO TOptics_Traj_NN_vars(key, value) VALUES ('eps_sp', 250);
--INSERT INTO TOptics_Traj_NN_vars(key, value) VALUES ('eps_t', '5 minutes');
--INSERT INTO TOptics_Traj_NN_vars(key, value) VALUES ('prc', 0.6);

SELECT TOptics_Traj('milan_traj', 'he_TOptics_Traj_NN_Euclidean', 15);
*/

CREATE FUNCTION TOptics_Traj_storage() RETURNS void AS $$
BEGIN
	DROP TABLE IF EXISTS TOptics_Traj_vars CASCADE;
	DROP TABLE IF EXISTS TOptics_Traj_ordering CASCADE;
	DROP TABLE IF EXISTS TOptics_Traj_seeds CASCADE;
	DROP TABLE IF EXISTS TOptics_Traj_neighbors CASCADE;
	DROP TABLE IF EXISTS TOptics_Traj_db_ids CASCADE;

	DROP TABLE IF EXISTS TOptics_Traj_clusters CASCADE;

	CREATE TEMPORARY TABLE TOptics_Traj_vars (
		key text NOT NULL,
		value text NOT NULL,

		PRIMARY KEY (key)
	);

	CREATE TEMPORARY TABLE TOptics_Traj_ordering (
		id serial NOT NULL,
		obj_id integer NOT NULL,
		traj_id integer NOT NULL,
		reachability_distance double precision,
		core_distance double precision,

		UNIQUE (obj_id, traj_id),
		PRIMARY KEY (id)
	);

	CREATE TEMPORARY TABLE TOptics_Traj_seeds (
		obj_id integer NOT NULL,
		traj_id integer NOT NULL,
		reachability_distance double precision,

		PRIMARY KEY (obj_id, traj_id)
	);
	CREATE INDEX ON TOptics_Traj_seeds (reachability_distance);

	CREATE TEMPORARY TABLE TOptics_Traj_neighbors (
		obj_id integer NOT NULL,
		traj_id integer NOT NULL,
		distance double precision NOT NULL,

		PRIMARY KEY (obj_id, traj_id)
	);
	CREATE INDEX ON TOptics_Traj_neighbors (distance);

	CREATE TEMPORARY TABLE TOptics_Traj_db_ids (
		obj_id integer NOT NULL,
		traj_id integer NOT NULL,

		PRIMARY KEY (obj_id, traj_id)
	);
END;
$$ LANGUAGE plpgsql;
COMMENT ON FUNCTION TOptics_Traj_storage() IS
'Creates the necessary temporary tables that TOptics algorithm uses.
HDOC
TOptics_Traj_vars
TOptics_Traj_ordering
TOptics_Traj_clusters
TOptics_Traj_seeds
TOptics_Traj_neighbors
TOptics_Traj_db_ids';

CREATE FUNCTION TOptics_Traj_cleanUp() RETURNS void AS $$
BEGIN
	DROP TABLE IF EXISTS TOptics_Traj_db_ids CASCADE;
	DROP TABLE IF EXISTS TOptics_Traj_neighbors CASCADE;
	DROP TABLE IF EXISTS TOptics_Traj_seeds CASCADE;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION TOptics_Traj_neighbors(DB text, NN_method text, p_obj integer, p_traj integer) RETURNS void AS $$
BEGIN
	TRUNCATE TABLE TOptics_Traj_neighbors;

	EXECUTE 'SELECT ' || NN_method || '(''' || DB || '''::text, ' || p_obj || '::integer, ' || p_traj || '::integer);';
END;
$$ LANGUAGE plpgsql STRICT;

CREATE FUNCTION TOptics_Traj(DB text, NN_method text, min_trajs integer, ksi double precision DEFAULT -1.0) RETURNS void AS $$
DECLARE
	p_obj integer;
	p_traj integer;

	o record;

	obj_id_temp integer;
	traj_id_temp integer;

	core_distance_temp double precision;
	reachability_distance_temp double precision;

	mxid integer;
	urd double precision;
	ucd double precision;
BEGIN
	PERFORM TOptics_Traj_storage();

	EXECUTE 'INSERT INTO TOptics_Traj_db_ids(obj_id, traj_id) SELECT DISTINCT obj_id, traj_id FROM ' || quote_ident(DB) || ';';

	LOOP
		SELECT obj_id, traj_id INTO p_obj, p_traj FROM TOptics_Traj_db_ids LIMIT 1;
		EXIT WHEN NOT FOUND;

		PERFORM TOptics_Traj_neighbors(DB, NN_method, p_obj, p_traj);

		SELECT distance INTO core_distance_temp FROM TOptics_Traj_neighbors ORDER BY distance ASC LIMIT 1 OFFSET min_trajs - 1;
		IF NOT FOUND THEN
			core_distance_temp := NULL;
		END IF;

		INSERT INTO TOptics_Traj_ordering(obj_id, traj_id, core_distance) VALUES (p_obj, p_traj, core_distance_temp);
		DELETE FROM TOptics_Traj_db_ids WHERE (obj_id, traj_id) = (p_obj, p_traj);

		TRUNCATE TABLE TOptics_Traj_seeds;
		IF core_distance_temp IS NOT NULL THEN
			FOR o IN SELECT obj_id, traj_id, distance FROM TOptics_Traj_neighbors WHERE (obj_id, traj_id) NOT IN (SELECT obj_id, traj_id FROM TOptics_Traj_ordering) LOOP
				IF core_distance_temp >= o.distance THEN
					reachability_distance_temp := core_distance_temp;
				ELSE
					reachability_distance_temp := o.distance;
				END IF;

				SELECT obj_id, traj_id INTO obj_id_temp, traj_id_temp FROM TOptics_Traj_seeds WHERE (obj_id, traj_id) = (o.obj_id, o.traj_id);
				IF NOT FOUND THEN
					INSERT INTO TOptics_Traj_seeds(obj_id, traj_id, reachability_distance) VALUES (o.obj_id, o.traj_id, reachability_distance_temp);
				ELSE
					UPDATE TOptics_Traj_seeds
					SET reachability_distance = reachability_distance_temp
					WHERE (obj_id, traj_id) = (o.obj_id, o.traj_id) AND reachability_distance > reachability_distance_temp;
				END IF;
			END LOOP;

			LOOP
				SELECT obj_id, traj_id, reachability_distance INTO p_obj, p_traj, reachability_distance_temp FROM TOptics_Traj_seeds ORDER BY reachability_distance ASC;
				EXIT WHEN NOT FOUND;

				DELETE FROM TOptics_Traj_seeds WHERE (obj_id, traj_id) = (p_obj, p_traj);

				PERFORM TOptics_Traj_neighbors(DB, NN_method, p_obj, p_traj);

				SELECT distance INTO core_distance_temp FROM TOptics_Traj_neighbors ORDER BY distance ASC LIMIT 1 OFFSET min_trajs - 1;
				IF NOT FOUND THEN
					core_distance_temp := NULL;
				END IF;

				INSERT INTO TOptics_Traj_ordering(obj_id, traj_id, core_distance, reachability_distance) VALUES (p_obj, p_traj, core_distance_temp, reachability_distance_temp);
				DELETE FROM TOptics_Traj_db_ids WHERE (obj_id, traj_id) = (p_obj, p_traj);

				IF core_distance_temp IS NOT NULL THEN
					FOR o IN SELECT obj_id, traj_id, distance FROM TOptics_Traj_neighbors WHERE (obj_id, traj_id) NOT IN (SELECT obj_id, traj_id FROM TOptics_Traj_ordering) LOOP
						IF core_distance_temp >= o.distance THEN
							reachability_distance_temp := core_distance_temp;
						ELSE
							reachability_distance_temp := o.distance;
						END IF;

						SELECT obj_id, traj_id INTO obj_id_temp, traj_id_temp FROM TOptics_Traj_seeds WHERE (obj_id, traj_id) = (o.obj_id, o.traj_id);
						IF NOT FOUND THEN
							INSERT INTO TOptics_Traj_seeds(obj_id, traj_id, reachability_distance) VALUES (o.obj_id, o.traj_id, reachability_distance_temp);
						ELSE
							UPDATE TOptics_Traj_seeds
							SET reachability_distance = reachability_distance_temp
							WHERE (obj_id, traj_id) = (o.obj_id, o.traj_id) AND reachability_distance > reachability_distance_temp;
						END IF;
					END LOOP;
				END IF;
			END LOOP;
		END IF;
	END LOOP;

	PERFORM TOptics_Traj_cleanUp();

	INSERT INTO TOptics_Traj_vars(key, value) VALUES ('DB', DB);
	INSERT INTO TOptics_Traj_vars(key, value) VALUES ('NN_method', NN_method);
	INSERT INTO TOptics_Traj_vars(key, value) VALUES ('min_trajs', min_trajs);

	SELECT max(id) INTO mxid FROM TOptics_Traj_ordering;
	IF mxid IS NULL THEN
		INSERT INTO TOptics_Traj_vars(key, value) VALUES ('mxid', '0');
	ELSE
		INSERT INTO TOptics_Traj_vars(key, value) VALUES ('mxid', mxid);
	END IF;

	SELECT max(reachability_distance) * 2 INTO urd FROM TOptics_Traj_ordering;
	IF urd IS NULL THEN
		INSERT INTO TOptics_Traj_vars(key, value) VALUES ('urd', '0.0');
	ELSE
		INSERT INTO TOptics_Traj_vars(key, value) VALUES ('urd', urd);
	END IF;

	SELECT max(core_distance) * 2 INTO ucd FROM TOptics_Traj_ordering;
	IF ucd IS NULL THEN
		INSERT INTO TOptics_Traj_vars(key, value) VALUES ('ucd', '0.0');
	ELSE
		INSERT INTO TOptics_Traj_vars(key, value) VALUES ('ucd', ucd);
	END IF;

	IF 0.0 < ksi AND ksi < 1.0 THEN
		PERFORM TOptics_Traj_extractClusters(ksi);
	END IF;
END;
$$ LANGUAGE plpgsql STRICT;
