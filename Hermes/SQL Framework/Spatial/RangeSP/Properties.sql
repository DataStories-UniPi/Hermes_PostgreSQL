/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief The implementantion in sql of RangeSP.
 *
 * @details RangeSP is a spatial data type used in Hermes
 *
 * @see @ref data_types_spatial
 *
 */

/******************************************************************************/
CREATE FUNCTION area(range RangeSP) RETURNS double precision AS
	'$libdir/Hermes','areaRangeSPV1'
LANGUAGE C IMMUTABLE STRICT;
