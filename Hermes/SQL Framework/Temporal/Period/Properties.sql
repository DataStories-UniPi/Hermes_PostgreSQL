/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION isPunctal(period Period) RETURNS boolean AS
	'$libdir/Hermes','isPunctalPeriodV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION duration(period Period) RETURNS interval AS
	'$libdir/Hermes','durationInIntervalPeriodV1'
LANGUAGE C IMMUTABLE STRICT;
