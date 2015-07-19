/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief The implementantion in sql of BoxSP.
 *
 * @details BoxSP is a spatial data type used in Hermes
 *
 * @see @ref data_types_spatial
 *
 */

/******************************************************************************/
CREATE FUNCTION RangeSP(box BoxSP) RETURNS RangeSP AS
	'$libdir/Hermes','BoxSP2RangeSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE CAST (BoxSP AS RangeSP) WITH FUNCTION RangeSP(BoxSP) AS ASSIGNMENT;
