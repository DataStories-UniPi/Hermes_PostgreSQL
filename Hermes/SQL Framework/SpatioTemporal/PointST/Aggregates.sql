/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION minT(pointA PointST, pointB PointST) RETURNS PointST AS
	'$libdir/Hermes','minTPointST_PointST'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION maxT(pointA PointST, pointB PointST) RETURNS PointST AS
	'$libdir/Hermes','maxTPointST_PointST'
LANGUAGE C IMMUTABLE STRICT;

CREATE AGGREGATE minT(PointST) (
	SFUNC = minT(PointST, PointST),
	STYPE = PointST
);

CREATE AGGREGATE maxT(PointST) (
	SFUNC = maxT(PointST, PointST),
	STYPE = PointST
);
