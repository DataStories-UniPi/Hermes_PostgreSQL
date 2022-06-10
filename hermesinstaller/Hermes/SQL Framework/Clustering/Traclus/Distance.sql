/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION projectionPointTraclus(point PointSP, segment SegmentSP) RETURNS PointSP AS
	'$libdir/Hermes','projectionPointTraclusV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION perpendicularDistanceTraclus(segmentA SegmentSP, segmentB SegmentSP) RETURNS double precision AS
	'$libdir/Hermes','perpendicularDistanceTraclusV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION parallelDistanceTraclus(segmentA SegmentSP, segmentB SegmentSP) RETURNS double precision AS
	'$libdir/Hermes','parallelDistanceTraclusV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION angleDistanceTraclus(segmentA SegmentSP, segmentB SegmentSP) RETURNS double precision AS
	'$libdir/Hermes','angleDistanceTraclusV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION traclusDistance(segmentA SegmentSP, segmentB SegmentSP, w_perpendicular real DEFAULT 1, w_parallel real DEFAULT 1, w_angle real DEFAULT 1) RETURNS double precision AS
	'$libdir/Hermes','traclusDistanceV1'
LANGUAGE C IMMUTABLE STRICT;
