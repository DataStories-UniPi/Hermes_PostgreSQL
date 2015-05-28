/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION simplify(trajectory Trajectory, samplingPeriod interval, strict boolean DEFAULT false, epoch boolean DEFAULT false, interpolationPrecision integer DEFAULT HInterpolationPrecision()) RETURNS Trajectory AS
	'$libdir/Hermes','simplifyTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION simplifyDP(trajectory Trajectory, eps double precision, plus boolean DEFAULT false) RETURNS Trajectory AS
	'$libdir/Hermes','simplifyDPTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;
