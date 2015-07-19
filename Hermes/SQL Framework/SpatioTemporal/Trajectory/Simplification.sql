/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the Trajectory data type.
 *
 * @see @ref Trajectory
 * @see @ref data_type_trajectory
 */

/******************************************************************************/
CREATE FUNCTION simplify(trajectory Trajectory, samplingPeriod interval, strict boolean DEFAULT false, epoch boolean DEFAULT false, interpolationPrecision integer DEFAULT HInterpolationPrecision()) RETURNS Trajectory AS
	'$libdir/Hermes','simplifyTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION simplifyDP(trajectory Trajectory, eps double precision, plus boolean DEFAULT false) RETURNS Trajectory AS
	'$libdir/Hermes','simplifyDPTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;
