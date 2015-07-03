/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the SegmentST data type.
 *
 * @see @ref SegmentST
 * @see @ref data_types_spatio_temp
 */

/******************************************************************************/
CREATE FUNCTION BoxST(segment SegmentST) RETURNS BoxST AS
	'$libdir/Hermes','SegmentST2BoxSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE CAST (SegmentST AS BoxST) WITH FUNCTION BoxST(SegmentST) AS ASSIGNMENT;
