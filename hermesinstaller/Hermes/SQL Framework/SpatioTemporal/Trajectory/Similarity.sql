/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION EuclideanStart(trajectoryA Trajectory, trajectoryB Trajectory) RETURNS double precision AS
	'$libdir/Hermes','EuclideanStartV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION EuclideanEnd(trajectoryA Trajectory, trajectoryB Trajectory) RETURNS double precision AS
	'$libdir/Hermes','EuclideanEndV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION EuclideanStartEnd(trajectoryA Trajectory, trajectoryB Trajectory) RETURNS double precision AS
	'$libdir/Hermes','EuclideanStartEndV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION Manhattan(trajectoryA Trajectory, trajectoryB Trajectory, interpolationPrecision integer DEFAULT HInterpolationPrecision()) RETURNS double precision AS
	'$libdir/Hermes','ManhattanV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION Euclidean(trajectoryA Trajectory, trajectoryB Trajectory, interpolationPrecision integer DEFAULT HInterpolationPrecision()) RETURNS double precision AS
	'$libdir/Hermes','EuclideanV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION Chebyshev(trajectoryA Trajectory, trajectoryB Trajectory, interpolationPrecision integer DEFAULT HInterpolationPrecision()) RETURNS double precision AS
	'$libdir/Hermes','ChebyshevV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION DISSIMExact(trajectoryA Trajectory, trajectoryB Trajectory, interpolationPrecision integer DEFAULT HInterpolationPrecision()) RETURNS double precision AS
	'$libdir/Hermes','DISSIMExact'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION DISSIMApproximate(trajectoryA Trajectory, trajectoryB Trajectory, interpolationPrecision integer DEFAULT HInterpolationPrecision(),
	OUT dissim double precision, OUT error double precision
) AS
	'$libdir/Hermes','DISSIMApproximate'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION DTW(trajectoryA Trajectory, trajectoryB Trajectory, w integer DEFAULT -1, w_percentage double precision DEFAULT 1, lp integer DEFAULT 2) RETURNS double precision AS
	'$libdir/Hermes','DTWV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION LCSS(trajectoryA Trajectory, trajectoryB Trajectory, w integer DEFAULT -1, w_percentage double precision DEFAULT 1, eps double precision DEFAULT 1) RETURNS double precision AS
	'$libdir/Hermes','LCSSV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION EDR(trajectoryA Trajectory, trajectoryB Trajectory, eps double precision DEFAULT 1) RETURNS double precision AS
	'$libdir/Hermes','EDRV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION ERP(trajectoryA Trajectory, trajectoryB Trajectory, lp integer DEFAULT 2) RETURNS double precision AS
	'$libdir/Hermes','ERPV1'
LANGUAGE C IMMUTABLE STRICT;
