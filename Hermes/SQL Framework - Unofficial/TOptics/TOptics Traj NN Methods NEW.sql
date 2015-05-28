/*
 * Authors: Marios Vodas (mvodas@gmail.com), Sotiria Kallitzaki (skallitzaki@gmail.com).
 */

CREATE OR REPLACE FUNCTION he_TOptics_Traj_NN_Euclidean(DB text, p_obj integer, p_traj integer) RETURNS void AS $$
DECLARE
	rec record;
	distance double precision;

	common_period Period;
	p Trajectory;
BEGIN
	EXECUTE
	'
		SELECT traj
		FROM ' || quote_ident(DB) || '
		WHERE (obj_id, traj_id) = ($1, $2);
	'
		INTO p
	USING p_obj, p_traj;

	FOR rec IN
		EXECUTE
		'
			SELECT obj_id, traj_id, traj
			FROM ' || quote_ident(DB) || '
			WHERE (obj_id, traj_id) <> ($1, $2)
				AND traj && BoxST($3);
		'
		USING p_obj, p_traj, p
	LOOP
		common_period := intersection(p::Period, rec.traj::Period);
		IF common_period IS NULL THEN
			RETURN;
		END IF;

		distance := Euclidean(atPeriod(p, common_period), atPeriod(rec.traj, common_period));

		IF distance IS NOT NULL THEN
			INSERT INTO TOptics_Traj_neighbors(obj_id, traj_id, distance) VALUES (rec.obj_id, rec.traj_id, distance);
		END IF;
	END LOOP;
END;
$$ LANGUAGE plpgsql STRICT;


CREATE OR REPLACE FUNCTION he_TOptics_Traj_NN_Manhattan(DB text, p_obj integer, p_traj integer) RETURNS void AS $$
DECLARE
	rec record;
	distance double precision;
	
	common_period Period;
	p Trajectory;
BEGIN
	EXECUTE
	'
		SELECT traj
		FROM ' || quote_ident(DB) || '
		WHERE (obj_id, traj_id) = ($1, $2);
	'
		INTO p
	USING p_obj, p_traj;

	FOR rec IN
		EXECUTE
		'
			SELECT obj_id, traj_id, traj
			FROM ' || quote_ident(DB) || '
			WHERE (obj_id, traj_id) <> ($1, $2)
				AND traj && BoxST($3);
		'
		USING p_obj, p_traj, p
	LOOP
		common_period := intersection(p::Period, rec.traj::Period);
		IF common_period IS NULL THEN
			RETURN;
		END IF;
		distance := Manhattan(atPeriod(p, common_period), atPeriod(rec.traj, common_period));

		IF distance IS NOT NULL THEN
			INSERT INTO TOptics_Traj_neighbors(obj_id, traj_id, distance) VALUES (rec.obj_id, rec.traj_id, distance);
		END IF;
	END LOOP;
END;
$$ LANGUAGE plpgsql STRICT;


CREATE OR REPLACE FUNCTION he_TOptics_Traj_NN_Chebyshev(DB text, p_obj integer, p_traj integer) RETURNS void AS $$
DECLARE
	rec record;
	distance double precision;
	
	common_period Period;
	p Trajectory;
BEGIN
	EXECUTE
	'
		SELECT traj
		FROM ' || quote_ident(DB) || '
		WHERE (obj_id, traj_id) = ($1, $2);
	'
		INTO p
	USING p_obj, p_traj;

	FOR rec IN
		EXECUTE
		'
			SELECT obj_id, traj_id, traj
			FROM ' || quote_ident(DB) || '
			WHERE (obj_id, traj_id) <> ($1, $2)
				AND traj && BoxST($3);
		'
		USING p_obj, p_traj, p
	LOOP
		common_period := intersection(p::Period, rec.traj::Period);
		IF common_period IS NULL THEN
			RETURN;
		END IF;

		distance := Chebyshev(atPeriod(p, common_period), atPeriod(rec.traj, common_period));

		IF distance IS NOT NULL THEN
			INSERT INTO TOptics_Traj_neighbors(obj_id, traj_id, distance) VALUES (rec.obj_id, rec.traj_id, distance);
		END IF;
	END LOOP;
END;
$$ LANGUAGE plpgsql STRICT;

--INSERT INTO TOptics_Traj_NN_vars(key, value) VALUES ('wrap_window', 0);
--INSERT INTO TOptics_Traj_NN_vars(key, value) VALUES ('lp', 1); //1-Manhattan 2-Euclidean
CREATE OR REPLACE FUNCTION he_TOptics_Traj_NN_DTW(DB text, p_obj integer, p_traj integer) RETURNS void AS $$
DECLARE
	rec record;
	distance double precision;

	common_period Period;
	p Trajectory;
BEGIN
	EXECUTE
	'
		SELECT traj
		FROM ' || quote_ident(DB) || '
		WHERE (obj_id, traj_id) = ($1, $2);
	'
		INTO p
	USING p_obj, p_traj;

	FOR rec IN
		EXECUTE
		'
			SELECT obj_id, traj_id, traj
			FROM ' || quote_ident(DB) || '
			WHERE (obj_id, traj_id) <> ($1, $2)
				AND traj && BoxST($3);
		'
		USING p_obj, p_traj, p
	LOOP
		distance := DTW(p, rec.traj,(SELECT value FROM TOptics_Traj_NN_vars WHERE key = 'wrap_window')::integer, (SELECT value FROM TOptics_Traj_NN_vars WHERE key = 'lp')::integer);

		IF distance IS NOT NULL THEN
			INSERT INTO TOptics_Traj_neighbors(obj_id, traj_id, distance) VALUES (rec.obj_id, rec.traj_id, distance);
		END IF;
	END LOOP;
END;
$$ LANGUAGE plpgsql STRICT;

--INSERT INTO TOptics_Traj_NN_vars(key, value) VALUES ('max_time', 20); //уе sec;;
--INSERT INTO TOptics_Traj_NN_vars(key, value) VALUES ('d_perc', 0); //double
--INSERT INTO TOptics_Traj_NN_vars(key, value) VALUES ('max_distance', 0); //0<=float<=1
CREATE OR REPLACE FUNCTION he_TOptics_Traj_NN_LCSS(DB text, p_obj integer, p_traj integer) RETURNS void AS $$
DECLARE
	rec record;
	distance double precision;

	common_period Period;
	p Trajectory;
BEGIN
	EXECUTE
	'
		SELECT traj
		FROM ' || quote_ident(DB) || '
		WHERE (obj_id, traj_id) = ($1, $2);
	'
		INTO p
	USING p_obj, p_traj;

	FOR rec IN
		EXECUTE
		'
			SELECT obj_id, traj_id, traj
			FROM ' || quote_ident(DB) || '
			WHERE (obj_id, traj_id) <> ($1, $2)
				AND traj && BoxST($3);
		'
		USING p_obj, p_traj, p
	LOOP
		distance := LCSS(p, rec.traj,(SELECT value FROM TOptics_Traj_NN_vars WHERE key = 'max_time')::integer, (SELECT value FROM TOptics_Traj_NN_vars WHERE key = 'd_perc')::float, (SELECT value FROM TOptics_Traj_NN_vars WHERE key = 'max_distance')::float);

		IF distance IS NOT NULL THEN
			INSERT INTO TOptics_Traj_neighbors(obj_id, traj_id, distance) VALUES (rec.obj_id, rec.traj_id, distance);
		END IF;
	END LOOP;
END;
$$ LANGUAGE plpgsql STRICT;

--INSERT INTO TOptics_Traj_NN_vars(key, value) VALUES ('e', 0); //float
CREATE OR REPLACE FUNCTION he_TOptics_Traj_NN_EDR(DB text, p_obj integer, p_traj integer) RETURNS void AS $$
DECLARE
	rec record;
	distance double precision;

	common_period Period;
	p Trajectory;
BEGIN
	EXECUTE
	'
		SELECT traj
		FROM ' || quote_ident(DB) || '
		WHERE (obj_id, traj_id) = ($1, $2);
	'
		INTO p
	USING p_obj, p_traj;

	FOR rec IN
		EXECUTE
		'
			SELECT obj_id, traj_id, traj
			FROM ' || quote_ident(DB) || '
			WHERE (obj_id, traj_id) <> ($1, $2)
				AND traj && BoxST($3);
		'
		USING p_obj, p_traj, p
	LOOP
		distance := EDR(p, rec.traj,(SELECT value FROM TOptics_Traj_NN_vars WHERE key = 'e')::float);

		IF distance IS NOT NULL THEN
			INSERT INTO TOptics_Traj_neighbors(obj_id, traj_id, distance) VALUES (rec.obj_id, rec.traj_id, distance);
		END IF;
	END LOOP;
END;
$$ LANGUAGE plpgsql STRICT;

--INSERT INTO TOptics_Traj_NN_vars(key, value) VALUES ('lp', 1); //integer
CREATE OR REPLACE FUNCTION he_TOptics_Traj_NN_ERP(DB text, p_obj integer, p_traj integer) RETURNS void AS $$
DECLARE
	rec record;
	distance double precision;

	common_period Period;
	p Trajectory;
BEGIN
	EXECUTE
	'
		SELECT traj
		FROM ' || quote_ident(DB) || '
		WHERE (obj_id, traj_id) = ($1, $2);
	'
		INTO p
	USING p_obj, p_traj;

	FOR rec IN
		EXECUTE
		'
			SELECT obj_id, traj_id, traj
			FROM ' || quote_ident(DB) || '
			WHERE (obj_id, traj_id) <> ($1, $2)
				AND traj && BoxST($3);
		'
		USING p_obj, p_traj, p
	LOOP
		distance := ERP(p, rec.traj,(SELECT value FROM TOptics_Traj_NN_vars WHERE key = 'lp')::integer);

		IF distance IS NOT NULL THEN
			INSERT INTO TOptics_Traj_neighbors(obj_id, traj_id, distance) VALUES (rec.obj_id, rec.traj_id, distance);
		END IF;
	END LOOP;
END;
$$ LANGUAGE plpgsql STRICT;


CREATE OR REPLACE FUNCTION he_TOptics_Traj_NN_EuclideanStart(DB text, p_obj integer, p_traj integer) RETURNS void AS $$
DECLARE
	rec record;
	distance double precision;

	common_period Period;
	p Trajectory;
BEGIN
	EXECUTE
	'
		SELECT traj
		FROM ' || quote_ident(DB) || '
		WHERE (obj_id, traj_id) = ($1, $2);
	'
		INTO p
	USING p_obj, p_traj;

	FOR rec IN
		EXECUTE
		'
			SELECT obj_id, traj_id, traj
			FROM ' || quote_ident(DB) || '
			WHERE (obj_id, traj_id) <> ($1, $2)
				AND traj && BoxST($3);
		'
		USING p_obj, p_traj, p
	LOOP
		common_period := intersection(p::Period, rec.traj::Period);
		IF common_period IS NULL THEN
			RETURN;
		END IF;

		distance := EuclideanStart(atPeriod(p, common_period), atPeriod(rec.traj, common_period));

		IF distance IS NOT NULL THEN
			INSERT INTO TOptics_Traj_neighbors(obj_id, traj_id, distance) VALUES (rec.obj_id, rec.traj_id, distance);
		END IF;
	END LOOP;
END;
$$ LANGUAGE plpgsql STRICT;



CREATE OR REPLACE FUNCTION he_TOptics_Traj_NN_EuclideanEnd(DB text, p_obj integer, p_traj integer) RETURNS void AS $$
DECLARE
	rec record;
	distance double precision;

	common_period Period;
	p Trajectory;
BEGIN
	EXECUTE
	'
		SELECT traj
		FROM ' || quote_ident(DB) || '
		WHERE (obj_id, traj_id) = ($1, $2);
	'
		INTO p
	USING p_obj, p_traj;

	FOR rec IN
		EXECUTE
		'
			SELECT obj_id, traj_id, traj
			FROM ' || quote_ident(DB) || '
			WHERE (obj_id, traj_id) <> ($1, $2)
				AND traj && BoxST($3);
		'
		USING p_obj, p_traj, p
	LOOP
		common_period := intersection(p::Period, rec.traj::Period);
		IF common_period IS NULL THEN
			RETURN;
		END IF;

		distance := EuclideanEnd(atPeriod(p, common_period), atPeriod(rec.traj, common_period));

		IF distance IS NOT NULL THEN
			INSERT INTO TOptics_Traj_neighbors(obj_id, traj_id, distance) VALUES (rec.obj_id, rec.traj_id, distance);
		END IF;
	END LOOP;
END;
$$ LANGUAGE plpgsql STRICT;


CREATE OR REPLACE FUNCTION he_TOptics_Traj_NN_EuclideanStartEnd(DB text, p_obj integer, p_traj integer) RETURNS void AS $$
DECLARE
	rec record;
	distance double precision;

	common_period Period;
	p Trajectory;
BEGIN
	EXECUTE
	'
		SELECT traj
		FROM ' || quote_ident(DB) || '
		WHERE (obj_id, traj_id) = ($1, $2);
	'
		INTO p
	USING p_obj, p_traj;

	FOR rec IN
		EXECUTE
		'
			SELECT obj_id, traj_id, traj
			FROM ' || quote_ident(DB) || '
			WHERE (obj_id, traj_id) <> ($1, $2)
				AND traj && BoxST($3);
		'
		USING p_obj, p_traj, p
	LOOP
		common_period := intersection(p::Period, rec.traj::Period);
		IF common_period IS NULL THEN
			RETURN;
		END IF;

		distance := EuclideanStartEnd(atPeriod(p, common_period), atPeriod(rec.traj, common_period));

		IF distance IS NOT NULL THEN
			INSERT INTO TOptics_Traj_neighbors(obj_id, traj_id, distance) VALUES (rec.obj_id, rec.traj_id, distance);
		END IF;
	END LOOP;
END;
$$ LANGUAGE plpgsql STRICT;