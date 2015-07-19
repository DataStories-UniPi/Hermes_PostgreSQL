/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the SegmentST data type.
 *
 * @see @ref SegmentST
 * @see @ref data_types_spatio_temp
 */
 
/******************************************************************************/
CREATE FUNCTION i(segment SegmentST) RETURNS PointST AS
	'$libdir/Hermes','iSegmentSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION e(segment SegmentST) RETURNS PointST AS
	'$libdir/Hermes','eSegmentSTV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION points(segment SegmentST)
RETURNS TABLE(p PointST) AS
	'$libdir/Hermes','pointsSegmentST'
LANGUAGE C IMMUTABLE STRICT;
