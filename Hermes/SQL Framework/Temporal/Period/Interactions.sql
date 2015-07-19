/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief The implementantion in sql of Period.
 *
 * @details Period is a temporal data type used in Hermes
 *
 * @see @ref data_types_temporal
 *
 */

/******************************************************************************/
CREATE FUNCTION intersection(periodA Period, periodB Period) RETURNS Period AS
	'$libdir/Hermes','intersectionPeriod_PeriodV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION HUnion(period Period, ts timestamp) RETURNS Period AS
	'$libdir/Hermes','unionPeriod_TimestampV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION HUnion(periodA Period, periodB Period) RETURNS Period AS
	'$libdir/Hermes','unionPeriod_PeriodV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE AGGREGATE HUnion(Period) (
	SFUNC = HUnion(Period, Period),
	STYPE = Period
);
