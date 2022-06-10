/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION meets(ts timestamp, period Period) RETURNS boolean AS
	'$libdir/Hermes','meetsTimestamp_PeriodV1'
LANGUAGE C IMMUTABLE STRICT;
