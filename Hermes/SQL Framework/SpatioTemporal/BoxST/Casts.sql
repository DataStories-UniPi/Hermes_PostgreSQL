/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief The implementantion in sql of BoxST.
 *
 * @details BoxST is a spatial data type used in Hermes
 *
 * @see @ref data_types_spatio_temp
 *
 */

/******************************************************************************/
CREATE FUNCTION RangeST(box BoxST) RETURNS RangeST AS
	'$libdir/Hermes','BoxST2RangeSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE CAST (BoxST AS RangeST) WITH FUNCTION RangeST(BoxST) AS ASSIGNMENT;
