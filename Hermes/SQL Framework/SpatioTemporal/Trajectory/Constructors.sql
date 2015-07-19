/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the Trajectory data type.
 *
 * @see @ref Trajectory
 * @see @ref data_type_trajectory
 */

/******************************************************************************/
CREATE FUNCTION Trajectory() RETURNS Trajectory AS
	'$libdir/Hermes','constructorEmptyTrajectory'
LANGUAGE C IMMUTABLE;

CREATE FUNCTION Trajectory(point PointST) RETURNS Trajectory AS
	'$libdir/Hermes','constructorPointTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION Trajectory(pointArray PointST[]) RETURNS Trajectory AS
	'$libdir/Hermes','constructorPointArrayTrajectory'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION TrajCache_Allocate(length integer) RETURNS integer AS
	'$libdir/Hermes','TrajCache_Allocate'
LANGUAGE C STRICT;

CREATE FUNCTION TrajCache_Free(cache_id integer) RETURNS void AS
	'$libdir/Hermes','TrajCache_Free'
LANGUAGE C STRICT;

CREATE FUNCTION TrajCache_Clean() RETURNS void AS
	'$libdir/Hermes','TrajCache_Clean'
LANGUAGE C;

CREATE FUNCTION TrajCache_ResetIndex(cache_id integer) RETURNS void AS
	'$libdir/Hermes','TrajCache_ResetIndex'
LANGUAGE C STRICT;

CREATE FUNCTION TrajCache_Rewind(cache_id integer) RETURNS void AS
	'$libdir/Hermes','TrajCache_Rewind'
LANGUAGE C STRICT;

CREATE FUNCTION TrajCache_Append(cache_id integer, point PointST) RETURNS void AS
	'$libdir/Hermes','TrajCache_Append'
LANGUAGE C STRICT;

CREATE FUNCTION TrajCache_Index(cache_id integer) RETURNS integer AS
	'$libdir/Hermes','TrajCache_getIndex'
LANGUAGE C STRICT;

CREATE FUNCTION TrajCache_Point(cache_id integer, i integer) RETURNS PointST AS
	'$libdir/Hermes','TrajCache_Point'
LANGUAGE C STRICT;

CREATE FUNCTION TrajCache2Trajectory(cache_id integer) RETURNS Trajectory AS
	'$libdir/Hermes','TrajCache2Trajectory'
LANGUAGE C STRICT;
