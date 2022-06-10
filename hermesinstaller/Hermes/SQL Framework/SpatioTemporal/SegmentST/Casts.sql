/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION BoxST(segment SegmentST) RETURNS BoxST AS
	'$libdir/Hermes','SegmentST2BoxSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE CAST (SegmentST AS BoxST) WITH FUNCTION BoxST(SegmentST) AS ASSIGNMENT;
