/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief The file implements the properties of the Trajectory object
 *
 * @see @ref methods
 *
 */

/** @brief This function returns the duration of a trajectory.
 *
 *	@param[in] trajectory the trajectory
 *
 *  @return the duration of the trajectory
 */
CREATE FUNCTION duration(trajectory Trajectory) RETURNS interval AS
	'$libdir/Hermes','durationInIntervalTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief This function returns the length of a trajectory.
 *
 *	@param[in] trajectory the trajectory
 *
 *  @return the length of the trajectory
 */
CREATE FUNCTION length(trajectory Trajectory) RETURNS double precision AS
	'$libdir/Hermes','lengthTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief This function returns the displacement of a trajectory.
 *
 *	@param[in] trajectory the trajectory
 *
 *  @return the displacement of the trajectory
 */
CREATE FUNCTION displacement(trajectory Trajectory) RETURNS double precision AS
	'$libdir/Hermes','displacementTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief This function returns the centroid of a trajectory.
 *
 *	@param[in] trajectory the trajectory
 *
 *  @return the centroid of the trajectory
 */
CREATE FUNCTION centroid(trajectory Trajectory) RETURNS PointST AS
	'$libdir/Hermes','centroidTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief This function returns the massCenter of a trajectory.
 *
 *	@param[in] trajectory the trajectory
 *
 *  @return the massCenter of the trajectory
 */
CREATE FUNCTION massCenter(trajectory Trajectory) RETURNS PointSP AS
	'$libdir/Hermes','massCenterTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief This function returns the gyradius of a trajectory.
 *
 *	@param[in] trajectory the trajectory
 *
 *  @return the gyradius of the trajectory
 */
CREATE FUNCTION gyradius(trajectory Trajectory) RETURNS double precision AS
	'$libdir/Hermes','gyradiusTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief This function returns the angleXXAvg of a trajectory.
 *
 *	@param[in] trajectory the trajectory
 *
 *  @return the angleXXAvg of the trajectory
 */
CREATE FUNCTION angleXXAvg(trajectory Trajectory) RETURNS double precision AS
	'$libdir/Hermes','angleXXAvgTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief This function returns the angleXX of a trajectory.
 *
 *	@param[in] trajectory the trajectory
 *
 *  @return the angleXX of the trajectory
 */
CREATE FUNCTION angleXX(trajectory Trajectory) RETURNS double precision AS
	'$libdir/Hermes','angleXXTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;


/** @brief The function returns the average speed of a trajectory
 *
 *	@param[in] trajectory the trajectory
 *
 *  @return the average speed of the trajectory
 */
CREATE FUNCTION averageSpeed(trajectory Trajectory) RETURNS double precision AS
	'$libdir/Hermes','averageSpeedTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief This function returns the normalized sampling rate of a trajectory.
 *
 *	@param[in] trajectory the trajectory
 *
 *  @return the normalized sampling rate of the trajectory
 */
CREATE FUNCTION normalizedSamplingRate(trajectory Trajectory) RETURNS double precision AS
	'$libdir/Hermes','normalizedSamplingRateTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;
