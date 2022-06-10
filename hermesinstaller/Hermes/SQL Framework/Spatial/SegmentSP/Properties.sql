/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION length(segment SegmentSP) RETURNS double precision AS
	'$libdir/Hermes','lengthSegmentSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION direction(segment SegmentSP) RETURNS Vector2D AS
	'$libdir/Hermes','directionSegmentSPV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION center(segment SegmentSP) RETURNS PointXY AS
	'$libdir/Hermes','centerSegmentSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION centerX(segment SegmentSP) RETURNS double precision AS
	'$libdir/Hermes','centerXSegmentSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION centerY(segment SegmentSP) RETURNS double precision AS
	'$libdir/Hermes','centerYSegmentSPV1'
LANGUAGE C IMMUTABLE STRICT;
