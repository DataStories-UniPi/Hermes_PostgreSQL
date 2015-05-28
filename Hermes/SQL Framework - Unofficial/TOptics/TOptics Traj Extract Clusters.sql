/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/*
Select TOptics_Traj_extractClusters(0.2);
*/ 
/******************************************************************************/
CREATE FUNCTION TOptics_Traj_r(p integer)
RETURNS double precision AS $$
DECLARE
	ret double precision;
BEGIN
	SELECT reachability_distance INTO ret FROM TOptics_Traj_ordering WHERE id = p;
	IF ret IS NULL THEN
		RETURN (SELECT value FROM TOptics_Traj_vars WHERE key = 'urd');
	ELSE
		RETURN ret;
	END IF;
END;
$$ LANGUAGE plpgsql STRICT;

CREATE FUNCTION TOptics_Traj_upTraj(p integer, ksi double precision)
RETURNS boolean AS $$
BEGIN
	RETURN TOptics_Traj_r(p) <= TOptics_Traj_r(p + 1) * (1.0 - ksi);
END;
$$ LANGUAGE plpgsql STRICT;

CREATE FUNCTION TOptics_Traj_downTraj(p integer, ksi double precision)
RETURNS boolean AS $$
BEGIN
	RETURN TOptics_Traj_r(p) * (1.0 - ksi) <= TOptics_Traj_r(p + 1);
END;
$$ LANGUAGE plpgsql STRICT;

CREATE FUNCTION TOptics_Traj_upArea(s integer, e integer, ksi double precision)
RETURNS smallint AS $$
DECLARE
	cnu integer := 0;
BEGIN
	IF NOT TOptics_Traj_upTraj(e, ksi) OR NOT TOptics_Traj_upTraj(s, ksi) THEN
		RETURN -1;
	END IF;

	FOR i IN (s + 1)..e LOOP
		IF TOptics_Traj_r(i) < TOptics_Traj_r(i - 1) THEN
			RETURN -2;
		END IF;
	END LOOP;

	FOR i IN (s + 1)..(e - 1) LOOP
		IF TOptics_Traj_upTraj(i, ksi) THEN
			cnu := 0;
		ELSE
			cnu := cnu + 1;
		END IF;

		IF cnu > (SELECT value::integer FROM TOptics_Traj_vars WHERE key = 'min_trajs') THEN
			RETURN -3;
		END IF;
	END LOOP;

	RETURN 0;
END;
$$ LANGUAGE plpgsql STRICT;

CREATE FUNCTION TOptics_Traj_downArea(s integer, e integer, ksi double precision)
RETURNS smallint AS $$
DECLARE
	cnd integer := 0;
BEGIN
	IF NOT TOptics_Traj_downTraj(e, ksi) OR NOT TOptics_Traj_downTraj(s, ksi) THEN
		RETURN -1;
	END IF;

	FOR i IN (s + 1)..e LOOP
		IF TOptics_Traj_r(i) > TOptics_Traj_r(i - 1) THEN
			RETURN -2;
		END IF;
	END LOOP;

	FOR i IN (s + 1)..(e - 1) LOOP
		IF TOptics_Traj_downTraj(i, ksi) THEN
			cnd := 0;
		ELSE
			cnd := cnd + 1;
		END IF;

		IF cnd > (SELECT value::integer FROM TOptics_Traj_vars WHERE key = 'min_trajs') THEN
			RETURN -3;
		END IF;
	END LOOP;

	RETURN 0;
END;
$$ LANGUAGE plpgsql STRICT;

CREATE FUNCTION TOptics_Traj_findUpArea(s integer, ksi double precision)
RETURNS integer AS $$
DECLARE
	e integer := NULL;
	flag smallint;
BEGIN
	IF NOT TOptics_Traj_upTraj(s, ksi) THEN
		RETURN NULL;
	END IF;

	FOR i IN (s + 1)..(SELECT value FROM TOptics_Traj_vars WHERE key = 'mxid') LOOP
		flag := TOptics_Traj_upArea(s, i, ksi);
		IF flag = 0 THEN
			e := i;
		ELSIF flag = -2 THEN
			EXIT;
		ELSIF flag = -3 THEN
			EXIT;
		END IF;
	END LOOP;

	RETURN e;
END;
$$ LANGUAGE plpgsql STRICT;

CREATE FUNCTION TOptics_Traj_findDownArea(s integer, ksi double precision)
RETURNS integer AS $$
DECLARE
	e integer := NULL;
	flag smallint;
BEGIN
	IF NOT TOptics_Traj_downTraj(s, ksi) THEN
		RETURN NULL;
	END IF;

	FOR i IN (s + 1)..(SELECT value FROM TOptics_Traj_vars WHERE key = 'mxid') LOOP
		flag := TOptics_Traj_downArea(s, i, ksi);
		IF flag = 0 THEN
			e := i;
		ELSIF flag = -2 THEN
			EXIT;
		ELSIF flag = -3 THEN
			EXIT;
		END IF;
	END LOOP;

	RETURN e;
END;
$$ LANGUAGE plpgsql STRICT;

CREATE FUNCTION TOptics_Traj_findClusterSE(sD integer, eD integer, sU integer, eU integer, ksi double precision, OUT s integer, OUT e integer) AS $$
DECLARE
	sD_r double precision;

	reach_end integer;
	reach_end_r double precision;
BEGIN
	sD_r := TOptics_Traj_r(sD);

	reach_end := eU + 1;
	reach_end_r := TOptics_Traj_r(reach_end);

	IF sD_r * (1.0 - ksi) >= reach_end_r THEN
		s := eD;
		FOR i IN REVERSE (eD - 1)..sD LOOP
			IF TOptics_Traj_r(i) > reach_end_r THEN
				EXIT;
			END IF;

			s := i;
		END LOOP;

		e := eU;
	ELSIF reach_end_r * (1.0 - ksi) >= sD_r THEN
		s := sD;

		e := eU;
		FOR i IN REVERSE (eU - 1)..sU LOOP
			IF TOptics_Traj_r(i) < sD_r THEN
				EXIT;
			END IF;

			e := i;
		END LOOP;
	ELSE
		s := sD;
		e := eU;
	END IF;
END;
$$ LANGUAGE plpgsql STRICT;

CREATE OR REPLACE FUNCTION TOptics_Traj_extractClusters(ksi double precision) RETURNS void AS $$
DECLARE
	indx integer := 1;
	indx_r double precision;

	mib_global double precision := 0.0;

	mxid integer;

	D RECORD;

	e_tmp integer;

	sU integer;
	eU integer;

	sC integer;
	eC integer;

	clust_id_tmp integer := 1;
BEGIN
	DROP TABLE IF EXISTS TOptics_Traj_clusters CASCADE;
	DROP TABLE IF EXISTS TOptics_Traj_steep_down_areas CASCADE;

	CREATE TEMPORARY TABLE TOptics_Traj_steep_down_areas
	(
		id serial NOT NULL,
		s integer NOT NULL,
		e integer NOT NULL,
		s_r double precision NOT NULL,
		e_r double precision NOT NULL,
		mib double precision NOT NULL,
		PRIMARY KEY (id),
		FOREIGN KEY (s) REFERENCES TOptics_Traj_ordering(id),
		FOREIGN KEY (e) REFERENCES TOptics_Traj_ordering(id)
	);

	CREATE TEMPORARY TABLE TOptics_Traj_clusters
	(
		o_id serial NOT NULL,
		clust_id integer NOT NULL,
		PRIMARY KEY (o_id, clust_id),
		FOREIGN KEY (o_id) REFERENCES TOptics_Traj_ordering(id)
	);
	CREATE INDEX ON TOptics_Traj_clusters (clust_id);

	SELECT value INTO mxid FROM TOptics_Traj_vars WHERE key = 'mxid';
	indx_r := TOptics_Traj_r(indx);

	WHILE indx < mxid LOOP
		mib_global := max(mib_global, indx_r);

		e_tmp := TOptics_Traj_findDownArea(indx, ksi);
		IF e_tmp IS NOT NULL THEN
			UPDATE TOptics_Traj_steep_down_areas
			SET mib = max(e_r, indx_r);

			DELETE FROM TOptics_Traj_steep_down_areas WHERE s_r * (1.0 - ksi) < mib_global;

			INSERT INTO TOptics_Traj_steep_down_areas(s, e, s_r, e_r, mib) VALUES (indx, e_tmp, indx_r, TOptics_Traj_r(e_tmp), 0.0);

			indx := e_tmp + 1;
			indx_r := TOptics_Traj_r(indx);
			mib_global := indx_r;
		ELSE
			e_tmp := TOptics_Traj_findUpArea(indx, ksi);
			IF e_tmp IS NOT NULL THEN
				UPDATE TOptics_Traj_steep_down_areas
				SET mib = max(e_r, indx_r);

				DELETE FROM TOptics_Traj_steep_down_areas WHERE s_r * (1.0 - ksi) < mib_global;

				sU := indx;
				eU := e_tmp;

				indx := e_tmp + 1;
				indx_r := TOptics_Traj_r(indx);
				mib_global := indx_r;

				FOR D IN SELECT s, e, mib FROM TOptics_Traj_steep_down_areas LOOP
					IF D.e >= sU THEN
						CONTINUE;
					END IF;

					IF TOptics_Traj_r(eU) * (1.0 - ksi) < D.mib THEN
						CONTINUE;
					END IF;

					SELECT s, e INTO sC, eC FROM TOptics_Traj_findClusterSE(D.s, D.e, sU, eU, ksi);

					IF eC - sC < (SELECT value::integer FROM TOptics_Traj_vars WHERE key = 'min_trajs') THEN
						CONTINUE;
					END IF;

					INSERT INTO TOptics_Traj_clusters(o_id, clust_id)
					SELECT id, clust_id_tmp FROM TOptics_Traj_ordering WHERE sC <= id AND id <= eC;

					clust_id_tmp := clust_id_tmp + 1;
				END LOOP;
			ELSE
				indx := indx + 1;
				indx_r := TOptics_Traj_r(indx);
			END IF;
		END IF;
	END LOOP;

	DROP TABLE IF EXISTS TOptics_Traj_steep_down_areas CASCADE;

	DELETE FROM TOptics_Traj_vars WHERE key = 'ksi' OR key = 'clusters_nr';

	INSERT INTO TOptics_Traj_vars(key, value) VALUES ('ksi', ksi);
	INSERT INTO TOptics_Traj_vars(key, value) VALUES ('clusters_nr', clust_id_tmp - 1);
END;
$$ LANGUAGE plpgsql STRICT;
