/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief The implementantion in sql of Period.
 *
 * @details RangeT is a temporal data type used in Hermes
 *
 * @see @ref data_types_temporal
 *
 */

/******************************************************************************/
CREATE FUNCTION Period(range RangeT) RETURNS Period AS
	'$libdir/Hermes','RangeT2PeriodV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE CAST (RangeT AS Period) WITH FUNCTION Period(RangeT) AS ASSIGNMENT;
