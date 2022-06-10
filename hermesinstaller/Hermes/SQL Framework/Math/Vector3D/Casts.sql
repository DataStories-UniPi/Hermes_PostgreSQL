/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION PointST(vector Vector3D) RETURNS PointST AS
	'$libdir/Hermes','Vector3D2PointSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE CAST (Vector3D AS PointST) WITH FUNCTION PointST(Vector3D) AS ASSIGNMENT;
