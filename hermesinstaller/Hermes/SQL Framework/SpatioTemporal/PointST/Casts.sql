/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION BoxST(point PointST) RETURNS BoxST AS
	'$libdir/Hermes','PointST2BoxSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE CAST (PointST AS BoxST) WITH FUNCTION BoxST(PointST) AS ASSIGNMENT;

/******************************************************************************/
CREATE FUNCTION Vector3D(point PointST) RETURNS Vector3D AS
	'$libdir/Hermes','PointST2Vector3DV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE CAST (PointST AS Vector3D) WITH FUNCTION Vector3D(PointST) AS ASSIGNMENT;
