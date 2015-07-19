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
CREATE FUNCTION RangeT(period Period) RETURNS RangeT AS
	'$libdir/Hermes','Period2RangeTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE CAST (Period AS RangeT) WITH FUNCTION RangeT(Period) AS ASSIGNMENT;
