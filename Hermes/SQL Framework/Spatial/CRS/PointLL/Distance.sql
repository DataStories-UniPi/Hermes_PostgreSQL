/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION distanceSphere(pointA PointLL, pointB PointLL) RETURNS double precision AS
	'$libdir/Hermes','distanceSpherePointLL_PointLLV1'
LANGUAGE C IMMUTABLE STRICT;
