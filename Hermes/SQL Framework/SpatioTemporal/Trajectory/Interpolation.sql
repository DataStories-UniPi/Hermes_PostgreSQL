/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the Trajectory data type.
 *
 * @see @ref Trajectory
 * @see @ref data_type_trajectory
 */

/******************************************************************************/
CREATE FUNCTION speedAt(trajectory Trajectory, i integer, s0 double precision) RETURNS double precision AS
	'$libdir/Hermes','speedAtTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION speedAt(trajectory Trajectory, i integer, interpolationPrecision integer DEFAULT HInterpolationPrecision()) RETURNS double precision AS
	'$libdir/Hermes','speedAtTrajectoryV1_2'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION accelerationAt(trajectory Trajectory, i integer, s0 double precision) RETURNS double precision AS
	'$libdir/Hermes','accelerationAtTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION accelerationAt(trajectory Trajectory, i integer, interpolationPrecision integer DEFAULT HInterpolationPrecision()) RETURNS double precision AS
	'$libdir/Hermes','accelerationAtTrajectoryV1_2'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION speedAt(trajectory Trajectory, ts timestamp, interpolationPrecision integer DEFAULT HInterpolationPrecision()) RETURNS double precision AS
	'$libdir/Hermes','speedAtTTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION accelerationAt(trajectory Trajectory, ts timestamp, interpolationPrecision integer DEFAULT HInterpolationPrecision()) RETURNS double precision AS
	'$libdir/Hermes','accelerationAtTTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION atInstant(trajectory Trajectory, ts timestamp, interpolationPrecision integer DEFAULT HInterpolationPrecision()) RETURNS PointST AS
	'$libdir/Hermes','atInstantTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION atPeriod(trajectory Trajectory, period Period, interpolationPrecision integer DEFAULT HInterpolationPrecision()) RETURNS Trajectory AS
	'$libdir/Hermes','atPeriodTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION atDistance(trajectory Trajectory, distance double precision, interpolationPrecision integer DEFAULT HInterpolationPrecision()) RETURNS PointST AS
	'$libdir/Hermes','atDistanceTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION atPoint(trajectory Trajectory, point PointSP, interpolationPrecision integer DEFAULT HInterpolationPrecision())
RETURNS TABLE(p PointST) AS
	'$libdir/Hermes','atPointTrajectoryV1'
LANGUAGE C STABLE STRICT;

CREATE FUNCTION atBox(trajectory Trajectory, box BoxSP, interpolationPrecision integer DEFAULT HInterpolationPrecision())
RETURNS TABLE(tr Trajectory) AS
	'$libdir/Hermes','atBoxTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION atBox(trajectory Trajectory, box BoxST, interpolationPrecision integer DEFAULT HInterpolationPrecision())
RETURNS TABLE(tr Trajectory) AS
	'$libdir/Hermes','atBoxSTTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;
