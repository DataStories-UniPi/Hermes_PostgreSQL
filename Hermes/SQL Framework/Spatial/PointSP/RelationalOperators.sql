/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION north(pointA PointSP, pointB PointSP) RETURNS boolean AS
	'$libdir/Hermes','northPointSP_PointSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION south(pointA PointSP, pointB PointSP) RETURNS boolean AS
	'$libdir/Hermes','southPointSP_PointSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION east(pointA PointSP, pointB PointSP) RETURNS boolean AS
	'$libdir/Hermes','eastPointSP_PointSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION west(pointA PointSP, pointB PointSP) RETURNS boolean AS
	'$libdir/Hermes','westPointSP_PointSPV1'
LANGUAGE C IMMUTABLE STRICT;
