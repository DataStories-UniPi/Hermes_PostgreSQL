/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION RangeST(box BoxST) RETURNS RangeST AS
	'$libdir/Hermes','BoxST2RangeSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE CAST (BoxST AS RangeST) WITH FUNCTION RangeST(BoxST) AS ASSIGNMENT;
