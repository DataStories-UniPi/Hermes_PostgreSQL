/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief The file implements the properties of the Trajectory object
 *
 * @see @ref methods
 *
 */

CREATE FUNCTION duration(trajectory Trajectory) RETURNS interval AS
	'$libdir/Hermes','durationInIntervalTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION length(trajectory Trajectory) RETURNS double precision AS
	'$libdir/Hermes','lengthTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION displacement(trajectory Trajectory) RETURNS double precision AS
	'$libdir/Hermes','displacementTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION centroid(trajectory Trajectory) RETURNS PointST AS
	'$libdir/Hermes','centroidTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION massCenter(trajectory Trajectory) RETURNS PointSP AS
	'$libdir/Hermes','massCenterTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION gyradius(trajectory Trajectory) RETURNS double precision AS
	'$libdir/Hermes','gyradiusTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION angleXXAvg(trajectory Trajectory) RETURNS double precision AS
	'$libdir/Hermes','angleXXAvgTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

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

CREATE FUNCTION normalizedSamplingRate(trajectory Trajectory) RETURNS double precision AS
	'$libdir/Hermes','normalizedSamplingRateTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;
