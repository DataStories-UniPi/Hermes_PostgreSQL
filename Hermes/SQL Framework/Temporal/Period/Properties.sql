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
CREATE FUNCTION isPunctal(period Period) RETURNS boolean AS
	'$libdir/Hermes','isPunctalPeriodV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION duration(period Period) RETURNS interval AS
	'$libdir/Hermes','durationInIntervalPeriodV1'
LANGUAGE C IMMUTABLE STRICT;
