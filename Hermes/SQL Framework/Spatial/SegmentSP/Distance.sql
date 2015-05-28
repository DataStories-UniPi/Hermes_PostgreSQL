/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION distance(segment SegmentSP, point PointSP) RETURNS double precision AS
	'$libdir/Hermes','distanceSegmentSP_PointSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION distance(segmentA SegmentSP, segmentB SegmentSP) RETURNS double precision AS
	'$libdir/Hermes','distanceSegmentSP_SegmentSPV1'
LANGUAGE C STABLE STRICT;
