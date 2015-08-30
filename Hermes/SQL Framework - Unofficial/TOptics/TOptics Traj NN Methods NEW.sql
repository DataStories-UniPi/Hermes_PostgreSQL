/*
 * Authors: Marios Vodas (mvodas@gmail.com), Sotiria Kallitzaki (skallitzaki@gmail.com).
 */

CREATE OR REPLACE FUNCTION he_TOptics_Traj_NN_Euclidean(DB text, p_obj integer, p_traj integer, epsilon integer) RETURNS void AS $$
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
				AND Euclidean(traj,$3) < $4;
		'
		USING p_obj, p_traj, p, epsilon
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


CREATE OR REPLACE FUNCTION he_TOptics_Traj_NN_Manhattan(DB text, p_obj integer, p_traj integer, epsilon integer) RETURNS void AS $$
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
				AND Manhattan(traj, $3) < $4;
		'
		USING p_obj, p_traj, p, epsilon
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


CREATE OR REPLACE FUNCTION he_TOptics_Traj_NN_Chebyshev(DB text, p_obj integer, p_traj integer, epsilon integer) RETURNS void AS $$
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
				AND Chebyshev(traj, $3) < $4;
		'
		USING p_obj, p_traj, p, epsilon
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


CREATE OR REPLACE FUNCTION he_TOptics_Traj_NN_EuclideanStart(DB text, p_obj integer, p_traj integer, epsilon integer) RETURNS void AS $$
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
				AND EuclideanStart(traj, $3) < $4;
		'
		USING p_obj, p_traj, p, epsilon
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


CREATE OR REPLACE FUNCTION he_TOptics_Traj_NN_EuclideanEnd(DB text, p_obj integer, p_traj integer, epsilon integer) RETURNS void AS $$
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
				AND EuclideanEnd(traj, $3) < $4;
		'
		USING p_obj, p_traj, p, epsilon
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


CREATE OR REPLACE FUNCTION he_TOptics_Traj_NN_EuclideanStartEnd(DB text, p_obj integer, p_traj integer, epsilon integer) RETURNS void AS $$
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
				AND EuclideanStartEnd(traj, $3) < $4;
		'
		USING p_obj, p_traj, p, epsilon
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


CREATE OR REPLACE FUNCTION he_TOptics_Traj_NN_DTW(DB text, p_obj integer, p_traj integer, epsilon integer) RETURNS void AS $$
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
				AND DTW(traj, $3,(SELECT value FROM TOptics_Traj_NN_vars WHERE key = ''wrap_window'')::integer, (SELECT value FROM TOptics_Traj_NN_vars WHERE key = ''lp'')::integer) < $4;
		'
		USING p_obj, p_traj, p, epsilon
	LOOP
		distance := DTW(p, rec.traj,(SELECT value FROM TOptics_Traj_NN_vars WHERE key = 'wrap_window')::integer, (SELECT value FROM TOptics_Traj_NN_vars WHERE key = 'lp')::integer);

		IF distance IS NOT NULL THEN
			INSERT INTO TOptics_Traj_neighbors(obj_id, traj_id, distance) VALUES (rec.obj_id, rec.traj_id, distance);
		END IF;
	END LOOP;
END;
$$ LANGUAGE plpgsql STRICT;


CREATE OR REPLACE FUNCTION he_TOptics_Traj_NN_EDR(DB text, p_obj integer, p_traj integer, epsilon integer) RETURNS void AS $$
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
				AND EDR(traj, $3,(SELECT value FROM TOptics_Traj_NN_vars WHERE key = ''e'')::float) < $4;
		'
		USING p_obj, p_traj, p, epsilon
	LOOP
		distance := EDR(p, rec.traj,(SELECT value FROM TOptics_Traj_NN_vars WHERE key = 'e')::float);

		IF distance IS NOT NULL THEN
			INSERT INTO TOptics_Traj_neighbors(obj_id, traj_id, distance) VALUES (rec.obj_id, rec.traj_id, distance);
		END IF;
	END LOOP;
END;
$$ LANGUAGE plpgsql STRICT;


CREATE OR REPLACE FUNCTION he_TOptics_Traj_NN_ERP(DB text, p_obj integer, p_traj integer, epsilon integer) RETURNS void AS $$
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
				AND ERP(traj, $3,(SELECT value FROM TOptics_Traj_NN_vars WHERE key = ''lp'')::integer) < $4;
		'
		USING p_obj, p_traj, p, epsilon
	LOOP
		distance := ERP(p, rec.traj,(SELECT value FROM TOptics_Traj_NN_vars WHERE key = 'lp')::integer);

		IF distance IS NOT NULL THEN
			INSERT INTO TOptics_Traj_neighbors(obj_id, traj_id, distance) VALUES (rec.obj_id, rec.traj_id, distance);
		END IF;
	END LOOP;
END;
$$ LANGUAGE plpgsql STRICT;


CREATE OR REPLACE FUNCTION he_TOptics_Traj_NN_LCSS(DB text, p_obj integer, p_traj integer, epsilon integer) RETURNS void AS $$
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
				AND LCSS(traj, $3, (SELECT value FROM TOptics_Traj_NN_vars WHERE key = ''max_time'')::integer, (SELECT value FROM TOptics_Traj_NN_vars WHERE key = ''d_perc'')::float, (SELECT value FROM TOptics_Traj_NN_vars WHERE key = ''max_distance'')::float) < $4;
		'
		USING p_obj, p_traj, p, epsilon
	LOOP
		distance := LCSS(p, rec.traj,(SELECT value FROM TOptics_Traj_NN_vars WHERE key = 'max_time')::integer, (SELECT value FROM TOptics_Traj_NN_vars WHERE key = 'd_perc')::float, (SELECT value FROM TOptics_Traj_NN_vars WHERE key = 'max_distance')::float);

		IF distance IS NOT NULL THEN
			INSERT INTO TOptics_Traj_neighbors(obj_id, traj_id, distance) VALUES (rec.obj_id, rec.traj_id, distance);
		END IF;
	END LOOP;
END;
$$ LANGUAGE plpgsql STRICT;
