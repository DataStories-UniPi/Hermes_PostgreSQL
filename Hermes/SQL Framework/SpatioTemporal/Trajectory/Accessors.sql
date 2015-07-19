/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the Trajectory data type.
 *
 * @see @ref Trajectory
 * @see @ref data_type_trajectory
 */
/******************************************************************************/
CREATE FUNCTION size(trajectory Trajectory) RETURNS bigint AS
	'$libdir/Hermes','sizeTrajectory'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION NrPoints(trajectory Trajectory) RETURNS integer AS
	'$libdir/Hermes','NrPointsTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION NrSegments(trajectory Trajectory) RETURNS integer AS
	'$libdir/Hermes','NrSegmentsTrajectory'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION samplingPeriod(trajectory Trajectory) RETURNS interval AS
	'$libdir/Hermes','samplingPeriodTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION pointAt(trajectory Trajectory, i integer) RETURNS PointST AS
	'$libdir/Hermes','pointAtTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION segmentAt(trajectory Trajectory, i integer) RETURNS SegmentST AS
	'$libdir/Hermes','segmentAtTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION firstPoint(trajectory Trajectory) RETURNS PointST AS
	'$libdir/Hermes','firstPointTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION lastPoint(trajectory Trajectory) RETURNS PointST AS
	'$libdir/Hermes','lastPointTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION firstSegment(trajectory Trajectory) RETURNS SegmentST AS
	'$libdir/Hermes','firstSegmentTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION lastSegment(trajectory Trajectory) RETURNS SegmentST AS
	'$libdir/Hermes','lastSegmentTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION sub(trajectory Trajectory, s integer, e integer) RETURNS Trajectory AS
	'$libdir/Hermes','subTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION points(trajectory Trajectory)
RETURNS TABLE(p PointST) AS
	'$libdir/Hermes','pointsTrajectory'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION segments(trajectory Trajectory)
RETURNS TABLE(s SegmentST) AS
	'$libdir/Hermes','segmentsTrajectory'
LANGUAGE C IMMUTABLE STRICT;
