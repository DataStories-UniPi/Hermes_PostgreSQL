/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION RangeSP(box BoxSP) RETURNS RangeSP AS
	'$libdir/Hermes','BoxSP2RangeSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE CAST (BoxSP AS RangeSP) WITH FUNCTION RangeSP(BoxSP) AS ASSIGNMENT;
