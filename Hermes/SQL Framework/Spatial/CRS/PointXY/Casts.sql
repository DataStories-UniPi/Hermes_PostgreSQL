/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION PointSP(point PointXY) RETURNS PointSP AS
	'$libdir/Hermes','PointXY2PointSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE CAST (PointXY AS PointSP) WITH FUNCTION PointSP(PointXY) AS ASSIGNMENT;

/******************************************************************************/
CREATE FUNCTION Vector2D(point PointXY) RETURNS Vector2D AS
	'$libdir/Hermes','PointXY2Vector2DV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE CAST (PointXY AS Vector2D) WITH FUNCTION Vector2D(PointXY) AS ASSIGNMENT;
