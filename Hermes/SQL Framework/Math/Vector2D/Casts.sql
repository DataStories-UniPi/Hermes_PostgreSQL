/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION PointSP(vector Vector2D) RETURNS PointSP AS
	'$libdir/Hermes','Vector2D2PointSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE CAST (Vector2D AS PointSP) WITH FUNCTION PointSP(Vector2D) AS ASSIGNMENT;
