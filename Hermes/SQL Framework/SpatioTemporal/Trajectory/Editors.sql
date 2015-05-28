/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION expand(trajectory Trajectory, point PointST) RETURNS Trajectory AS
	'$libdir/Hermes','expandOnPointTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE AGGREGATE Trajectory_Agg(PointST) (
	SFUNC = expand(Trajectory, PointST),
	STYPE = Trajectory,
	INITCOND = ''
);

/******************************************************************************/
CREATE FUNCTION expand(trajectory Trajectory, segment SegmentST) RETURNS Trajectory AS
	'$libdir/Hermes','expandOnSegmentTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE AGGREGATE Trajectory_Agg(SegmentST) (
	SFUNC = expand(Trajectory, SegmentST),
	STYPE = Trajectory,
	INITCOND = ''
);

/******************************************************************************/
CREATE FUNCTION expand(trajectoryA Trajectory, trajectoryB Trajectory) RETURNS Trajectory AS
	'$libdir/Hermes','expandOnTrajectoryTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE AGGREGATE Trajectory_Agg(Trajectory) (
	SFUNC = expand(Trajectory, Trajectory),
	STYPE = Trajectory
);
