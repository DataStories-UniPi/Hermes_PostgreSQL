/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief The implementantion in sql of SegmentSP.
 *
 * @details SegmentSP is a spatial data type used in Hermes
 *
 * @see @ref data_types_spatial
 *
 */

/******************************************************************************/
CREATE FUNCTION distance(segment SegmentSP, point PointSP) RETURNS double precision AS
	'$libdir/Hermes','distanceSegmentSP_PointSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION distance(segmentA SegmentSP, segmentB SegmentSP) RETURNS double precision AS
	'$libdir/Hermes','distanceSegmentSP_SegmentSPV1'
LANGUAGE C STABLE STRICT;
