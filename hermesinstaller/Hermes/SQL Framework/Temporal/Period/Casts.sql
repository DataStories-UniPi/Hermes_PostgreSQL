/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION RangeT(period Period) RETURNS RangeT AS
	'$libdir/Hermes','Period2RangeTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE CAST (Period AS RangeT) WITH FUNCTION RangeT(Period) AS ASSIGNMENT;
