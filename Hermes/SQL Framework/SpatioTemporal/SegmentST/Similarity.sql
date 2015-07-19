/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the SegmentST data type.
 *
 * @see @ref SegmentST
 * @see @ref data_types_spatio_temp
 */

/******************************************************************************/
CREATE FUNCTION Trapezoidal(segmentA SegmentST, segmentB SegmentST) RETURNS double precision AS
	'$libdir/Hermes','TrapezoidalSegmentSTV1'
LANGUAGE C IMMUTABLE STRICT;
