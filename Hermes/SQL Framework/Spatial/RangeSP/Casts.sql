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
CREATE FUNCTION BoxSP(range RangeSP) RETURNS BoxSP AS
	'$libdir/Hermes','RangeSP2BoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE CAST (RangeSP AS BoxSP) WITH FUNCTION BoxSP(RangeSP) AS ASSIGNMENT;
