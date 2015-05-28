/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION intersects(periodA Period, periodB Period) RETURNS boolean AS
	'$libdir/Hermes','intersectsPeriod_PeriodV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION contains(period Period, ts timestamp) RETURNS boolean AS
	'$libdir/Hermes','containsPeriod_TimestampV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION contains(periodA Period, periodB Period) RETURNS boolean AS
	'$libdir/Hermes','containsPeriod_PeriodV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION containsProperly(period Period, ts timestamp) RETURNS boolean AS
	'$libdir/Hermes','containsProperlyPeriod_TimestampV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION containsProperly(periodA Period, periodB Period) RETURNS boolean AS
	'$libdir/Hermes','containsProperlyPeriod_PeriodV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION after(period Period, ts timestamp) RETURNS boolean AS
	'$libdir/Hermes','afterPeriod_TimestampV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION after(periodA Period, periodB Period) RETURNS boolean AS
	'$libdir/Hermes','afterPeriod_PeriodV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION before(period Period, ts timestamp) RETURNS boolean AS
	'$libdir/Hermes','beforePeriod_TimestampV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION before(periodA Period, periodB Period) RETURNS boolean AS
	'$libdir/Hermes','beforePeriod_PeriodV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION meets(period Period, ts timestamp) RETURNS boolean AS
	'$libdir/Hermes','meetsPeriod_TimestampV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION meets(periodA Period, periodB Period) RETURNS boolean AS
	'$libdir/Hermes','meetsPeriod_PeriodV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION starts(periodA Period, periodB Period) RETURNS boolean AS
	'$libdir/Hermes','startsPeriod_PeriodV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION finishes(periodA Period, periodB Period) RETURNS boolean AS
	'$libdir/Hermes','finishesPeriod_PeriodV1'
LANGUAGE C IMMUTABLE STRICT;
