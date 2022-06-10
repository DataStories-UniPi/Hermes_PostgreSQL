/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION intersects(trajectory Trajectory, box BoxST) RETURNS boolean AS
	'$libdir/Hermes','intersectsTrajectory_BoxSTV1'
LANGUAGE C STABLE STRICT;

CREATE FUNCTION intersects(trajectory Trajectory, period Period) RETURNS boolean AS
	'$libdir/Hermes','intersectsTrajectory_PeriodV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION intersects(trajectory Trajectory, box BoxSP) RETURNS boolean AS
	'$libdir/Hermes','intersectsTrajectory_BoxSPV1'
LANGUAGE C IMMUTABLE STRICT;
