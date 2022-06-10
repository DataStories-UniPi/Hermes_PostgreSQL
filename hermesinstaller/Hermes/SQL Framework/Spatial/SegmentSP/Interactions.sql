/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION intersection(segment SegmentSP, box BoxSP, solid boolean DEFAULT true,
	OUT n integer, OUT s SegmentSP, OUT p PointSP
) AS
	'$libdir/Hermes','intersectionSegmentSP_BoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION intersection(segmentA SegmentSP, segmentB SegmentSP,
	OUT n integer, OUT s SegmentSP, OUT p PointSP
) AS
	'$libdir/Hermes','intersectionSegmentSP_SegmentSPV1'
LANGUAGE C STABLE STRICT;

/******************************************************************************/
CREATE FUNCTION closestPoint(segment SegmentSP, point PointSP) RETURNS PointSP AS
	'$libdir/Hermes','closestPointSegmentSP_PointSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION closestPoints(segmentA SegmentSP, segmentB SegmentSP,
	OUT areEqual boolean, OUT cpA PointSP, OUT cpB PointSP
) AS
	'$libdir/Hermes','closestPointsSegmentSP_SegmentSPV1'
LANGUAGE C STABLE STRICT;
