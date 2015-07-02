/*
* @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the Timestamp data type.
 *
 * @see @ref data_types_temporal
 */

/******************************************************************************/
CREATE FUNCTION meets(ts timestamp, period Period) RETURNS boolean AS
	'$libdir/Hermes','meetsTimestamp_PeriodV1'
LANGUAGE C IMMUTABLE STRICT;
