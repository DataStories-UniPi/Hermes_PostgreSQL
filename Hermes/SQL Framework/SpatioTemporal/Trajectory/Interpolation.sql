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

/** @brief This function takes a trajectory and a timestamp as parameters and returns
 * 	the point where the object was found at the given timestamp
 *
 *	@param[in] trajectory the trajectory
 *	@param[in] interpolationPrecision the interpolation precision
 *
 *  @return a PointST with the position of the interpolation
 * 
 * 	@see @ref methods_instant
 */
CREATE FUNCTION atInstant(trajectory Trajectory, ts timestamp, interpolationPrecision integer DEFAULT HInterpolationPrecision()) RETURNS PointST AS
	'$libdir/Hermes','atInstantTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief This function takes a Trajectory and a period as parameters and 
 * returns the part of the trajectory that corresponds to the given period.
 * 
 *	@param[in] trajectory the Trajectory
 *	@param[in] period the period
 *	@param[in] interpolationPrecision the interpolation precision
 *
 *  @return the part of the trajectory that corresponds to the given period
 * 
 * 	@see @ref methods_atperiod
 */
CREATE FUNCTION atPeriod(trajectory Trajectory, period Period, interpolationPrecision integer DEFAULT HInterpolationPrecision()) RETURNS Trajectory AS
	'$libdir/Hermes','atPeriodTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION atDistance(trajectory Trajectory, distance double precision, interpolationPrecision integer DEFAULT HInterpolationPrecision()) RETURNS PointST AS
	'$libdir/Hermes','atDistanceTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief This function takes a trajectory and a point as parameters and 
 * 	returns the timestamp at which the object was found at the given point. 
 * 	The point has to be on the trajectory, otherwise the function returns NULL.
 *
 *	@param[in] segment the segment
 *	@param[in] point the point
 *	@param[in] interpolationPrecision the interpolation precision
 *
 *  @return a PointST with the timestamp of the interpolation
 * 
 *  @see @ref methods_atpoint
 */
CREATE FUNCTION atPoint(trajectory Trajectory, point PointSP, interpolationPrecision integer DEFAULT HInterpolationPrecision())
RETURNS TABLE(p PointST) AS
	'$libdir/Hermes','atPointTrajectoryV1'
LANGUAGE C STABLE STRICT;

/** @brief This function takes a trajectory and a box as 
 * parameters and returns the part of the trajectory that 
 * resides within the box. 
 *
 *	@param[in] trajectory the trajectory
 *	@param[in] box the box
 * 	@param[in] interpolationPrecision the interpolation precision
 *
 *  @return the part of the Trajectory
 * 
 * 	@see @ref methods_atbox
 */
CREATE FUNCTION atBox(trajectory Trajectory, box BoxSP, interpolationPrecision integer DEFAULT HInterpolationPrecision())
RETURNS TABLE(tr Trajectory) AS
	'$libdir/Hermes','atBoxTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION atBox(trajectory Trajectory, box BoxST, interpolationPrecision integer DEFAULT HInterpolationPrecision())
RETURNS TABLE(tr Trajectory) AS
	'$libdir/Hermes','atBoxSTTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;
