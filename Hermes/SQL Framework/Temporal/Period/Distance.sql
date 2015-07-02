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
CREATE FUNCTION distance(period Period, ts timestamp) RETURNS interval AS
	'$libdir/Hermes','distanceInIntervalPeriod_TimestampV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION distance(periodA Period, periodB Period) RETURNS interval AS
	'$libdir/Hermes','distanceInIntervalPeriod_PeriodV1'
LANGUAGE C IMMUTABLE STRICT;
