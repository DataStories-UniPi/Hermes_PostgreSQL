/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION distance(pointA PointSP, pointB PointSP) RETURNS double precision AS
	'$libdir/Hermes','distancePointSP_PointSPV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION Manhattan(pointA PointSP, pointB PointSP) RETURNS double precision AS
	'$libdir/Hermes','distanceManhattanPointSP_PointSPV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION Chebyshev(pointA PointSP, pointB PointSP) RETURNS double precision AS
	'$libdir/Hermes','distanceChebyshevPointSP_PointSPV1'
LANGUAGE C IMMUTABLE STRICT;
