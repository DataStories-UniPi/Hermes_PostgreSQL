/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION BoxSP(range RangeSP) RETURNS BoxSP AS
	'$libdir/Hermes','RangeSP2BoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE CAST (RangeSP AS BoxSP) WITH FUNCTION BoxSP(RangeSP) AS ASSIGNMENT;
