/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION area(range RangeSP) RETURNS double precision AS
	'$libdir/Hermes','areaRangeSPV1'
LANGUAGE C IMMUTABLE STRICT;
