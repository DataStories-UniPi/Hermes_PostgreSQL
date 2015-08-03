/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the Trajectory data type.
 *
 * @see @ref Trajectory
 * @see @ref data_type_trajectory
 */

/** @brief The function constructs an empty trajectory
 * 
 *  @return the actual trajectory
 */
CREATE FUNCTION Trajectory() RETURNS Trajectory AS
	'$libdir/Hermes','constructorEmptyTrajectory'
LANGUAGE C IMMUTABLE;

/** @brief The function constructs a trajectory with a single point
 * 
 *  @param[in] point an array of PointST
 *
 *  @return the actual trajectory
 */
CREATE FUNCTION Trajectory(point PointST) RETURNS Trajectory AS
	'$libdir/Hermes','constructorPointTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function constructs a trajectory
 * 
 *  @param[in] pointArray an array of PointST
 *
 *  @return the actual trajectory
 */
CREATE FUNCTION Trajectory(pointArray PointST[]) RETURNS Trajectory AS
	'$libdir/Hermes','constructorPointArrayTrajectory'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function allocates the appropriate space in memory
 * 	for a trajectory
 * 
 *  @param[in] length The length of the trajectory
 *
 *  @return an integer which is an identifier of the current allocation
 */
CREATE FUNCTION TrajCache_Allocate(length integer) RETURNS integer AS
	'$libdir/Hermes','TrajCache_Allocate'
LANGUAGE C STRICT;

CREATE FUNCTION TrajCache_Free(cache_id integer) RETURNS void AS
	'$libdir/Hermes','TrajCache_Free'
LANGUAGE C STRICT;

CREATE FUNCTION TrajCache_Clean() RETURNS void AS
	'$libdir/Hermes','TrajCache_Clean'
LANGUAGE C;

/** @brief The function resets the index of the current allocation 
 * 	and set it at the beginning for a trajectory
 * 
 *  @param[in] cache_id the identifier of the TrajCache allocation
 *
 */
CREATE FUNCTION TrajCache_ResetIndex(cache_id integer) RETURNS void AS
	'$libdir/Hermes','TrajCache_ResetIndex'
LANGUAGE C STRICT;

CREATE FUNCTION TrajCache_Rewind(cache_id integer) RETURNS void AS
	'$libdir/Hermes','TrajCache_Rewind'
LANGUAGE C STRICT;

/** @brief The function append the PointSTs to the trajectory one by one
 * 
 *  @param[in] cache_id the identifier of the trajectory
 * 	@param[in] point the point to be appended
 *
 */
CREATE FUNCTION TrajCache_Append(cache_id integer, point PointST) RETURNS void AS
	'$libdir/Hermes','TrajCache_Append'
LANGUAGE C STRICT;

CREATE FUNCTION TrajCache_Index(cache_id integer) RETURNS integer AS
	'$libdir/Hermes','TrajCache_getIndex'
LANGUAGE C STRICT;

CREATE FUNCTION TrajCache_Point(cache_id integer, i integer) RETURNS PointST AS
	'$libdir/Hermes','TrajCache_Point'
LANGUAGE C STRICT;

/** @brief The function materialize the trajectory
 * 
 *  @param[in] cache_id the identifier of the trajectory
 *
 *  @return the actual trajectory
 */
CREATE FUNCTION TrajCache2Trajectory(cache_id integer) RETURNS Trajectory AS
	'$libdir/Hermes','TrajCache2Trajectory'
LANGUAGE C STRICT;
