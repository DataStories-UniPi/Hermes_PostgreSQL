/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION intersects(segment SegmentST, period Period) RETURNS boolean AS
	'$libdir/Hermes','intersectsSegmentST_PeriodV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION intersects(segment SegmentST, box BoxSP) RETURNS boolean AS
	'$libdir/Hermes','intersectsSegmentST_BoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION intersects(segment SegmentST, segmentSpatialOnly SegmentSP) RETURNS boolean AS
	'$libdir/Hermes','intersectsSegmentST_SegmentSPV1'
LANGUAGE C STABLE STRICT;

CREATE OPERATOR && (
	PROCEDURE = intersects,
	LEFTARG = SegmentST,
	RIGHTARG = Period
);

CREATE OPERATOR && (
	PROCEDURE = intersects,
	LEFTARG = SegmentST,
	RIGHTARG = BoxSP
);

CREATE OPERATOR && (
	PROCEDURE = intersects,
	LEFTARG = SegmentST,
	RIGHTARG = SegmentSP
);

/******************************************************************************/
CREATE FUNCTION contains(segment SegmentST, ts timestamp) RETURNS boolean AS
	'$libdir/Hermes','containsSegmentST_TimestampV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION contains(segment SegmentST, period Period) RETURNS boolean AS
	'$libdir/Hermes','containsSegmentST_PeriodV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION contains(segment SegmentST, point PointSP) RETURNS boolean AS
	'$libdir/Hermes','containsSegmentST_PointSPV1'
LANGUAGE C STABLE STRICT;

CREATE OPERATOR ~ (
	PROCEDURE = contains,
	LEFTARG = SegmentST,
	RIGHTARG = timestamp
);

CREATE OPERATOR ~ (
	PROCEDURE = contains,
	LEFTARG = SegmentST,
	RIGHTARG = Period
);

CREATE OPERATOR ~ (
	PROCEDURE = contains,
	LEFTARG = SegmentST,
	RIGHTARG = PointSP
);

/******************************************************************************/
CREATE FUNCTION contained(segment SegmentST, period Period) RETURNS boolean AS
	'$libdir/Hermes','containedSegmentST_PeriodV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION contained(segment SegmentST, box BoxSP) RETURNS boolean AS
	'$libdir/Hermes','containedSegmentST_BoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR @ (
	PROCEDURE = contained,
	LEFTARG = SegmentST,
	RIGHTARG = Period
);

CREATE OPERATOR @ (
	PROCEDURE = contained,
	LEFTARG = SegmentST,
	RIGHTARG = BoxSP
);

/******************************************************************************/
CREATE FUNCTION withinDistance(segment SegmentST, range RangeSP) RETURNS boolean AS
	'$libdir/Hermes','withinDistanceSegmentST_RangeSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR -< (
	PROCEDURE = withinDistance,
	LEFTARG = SegmentST,
	RIGHTARG = RangeSP
);
