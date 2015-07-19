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
CREATE FUNCTION intersects(segment SegmentSP, box BoxSP) RETURNS boolean AS
	'$libdir/Hermes','intersectsSegmentSP_BoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION intersects(segmentA SegmentSP, segmentB SegmentSP) RETURNS boolean AS
	'$libdir/Hermes','intersectsSegmentSP_SegmentSPV1'
LANGUAGE C STABLE STRICT;

/******************************************************************************/
CREATE FUNCTION contains(segment SegmentSP, point PointSP) RETURNS boolean AS
	'$libdir/Hermes','containsSegmentSP_PointSPV1'
LANGUAGE C STABLE STRICT;

/******************************************************************************/
CREATE FUNCTION contained(segment SegmentSP, box BoxSP) RETURNS boolean AS
	'$libdir/Hermes','containedSegmentSP_BoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION containedProperly(segment SegmentSP, box BoxSP) RETURNS boolean AS
	'$libdir/Hermes','containedProperlySegmentSP_BoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION withinDistance(segment SegmentSP, range RangeSP) RETURNS boolean AS
	'$libdir/Hermes','withinDistanceSegmentSP_RangeSPV1'
LANGUAGE C IMMUTABLE STRICT;
