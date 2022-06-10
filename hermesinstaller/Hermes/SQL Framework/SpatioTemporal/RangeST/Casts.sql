/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION BoxST(range RangeST) RETURNS BoxST AS
	'$libdir/Hermes','RangeST2BoxSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE CAST (RangeST AS BoxST) WITH FUNCTION BoxST(RangeST) AS ASSIGNMENT;
