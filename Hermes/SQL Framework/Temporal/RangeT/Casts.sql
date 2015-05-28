/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION Period(range RangeT) RETURNS Period AS
	'$libdir/Hermes','RangeT2PeriodV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE CAST (RangeT AS Period) WITH FUNCTION Period(RangeT) AS ASSIGNMENT;
