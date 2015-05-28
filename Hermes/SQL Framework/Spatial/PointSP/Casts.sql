/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION PointXY(point PointSP) RETURNS PointXY AS
	'$libdir/Hermes','PointSP2PointXYV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE CAST (PointSP AS PointXY) WITH FUNCTION PointXY(PointSP) AS ASSIGNMENT;

/******************************************************************************/
CREATE FUNCTION BoxSP(point PointSP) RETURNS BoxSP AS
	'$libdir/Hermes','PointSP2BoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE CAST (PointSP AS BoxSP) WITH FUNCTION BoxSP(PointSP) AS ASSIGNMENT;

/******************************************************************************/
CREATE FUNCTION Vector2D(point PointSP) RETURNS Vector2D AS
	'$libdir/Hermes','PointSP2Vector2DV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE CAST (PointSP AS Vector2D) WITH FUNCTION Vector2D(PointSP) AS ASSIGNMENT;
