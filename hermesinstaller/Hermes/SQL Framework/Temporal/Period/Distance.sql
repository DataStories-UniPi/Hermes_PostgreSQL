/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION distance(period Period, ts timestamp) RETURNS interval AS
	'$libdir/Hermes','distanceInIntervalPeriod_TimestampV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION distance(periodA Period, periodB Period) RETURNS interval AS
	'$libdir/Hermes','distanceInIntervalPeriod_PeriodV1'
LANGUAGE C IMMUTABLE STRICT;
