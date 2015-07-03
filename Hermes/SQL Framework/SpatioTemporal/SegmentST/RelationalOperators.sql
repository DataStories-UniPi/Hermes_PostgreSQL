/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the SegmentST data type.
 *
 * @see @ref SegmentST
 * @see @ref data_types_spatio_temp
 */

/******************************************************************************/
CREATE FUNCTION intersects(segment SegmentST, box BoxST) RETURNS boolean AS
	'$libdir/Hermes','intersectsSegmentST_BoxSTV1'
LANGUAGE C STABLE STRICT;

CREATE FUNCTION intersects(segmentA SegmentST, segmentB SegmentST) RETURNS boolean AS
	'$libdir/Hermes','intersectsSegmentST_SegmentSTV1'
LANGUAGE C STABLE STRICT;

CREATE OPERATOR && (
	PROCEDURE = intersects,
	LEFTARG = SegmentST,
	RIGHTARG = BoxST
);

CREATE OPERATOR && (
	PROCEDURE = intersects,
	LEFTARG = SegmentST,
	RIGHTARG = SegmentST
);

/******************************************************************************/
CREATE FUNCTION contains(segment SegmentST, point PointST) RETURNS boolean AS
	'$libdir/Hermes','containsSegmentST_PointSTV1'
LANGUAGE C STABLE STRICT;

CREATE OPERATOR ~ (
	PROCEDURE = contains,
	LEFTARG = SegmentST,
	RIGHTARG = PointST
);

/******************************************************************************/
CREATE FUNCTION contained(segment SegmentST, box BoxST) RETURNS boolean AS
	'$libdir/Hermes','containedSegmentST_BoxSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION containedProperly(segment SegmentST, box BoxST) RETURNS boolean AS
	'$libdir/Hermes','containedProperlySegmentST_BoxSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR @ (
	PROCEDURE = contained,
	LEFTARG = SegmentST,
	RIGHTARG = BoxST
);

CREATE OPERATOR @! (
	PROCEDURE = containedProperly,
	LEFTARG = SegmentST,
	RIGHTARG = BoxST
);

/******************************************************************************/
CREATE FUNCTION withinDistance(segment SegmentST, range RangeST) RETURNS boolean AS
	'$libdir/Hermes','withinDistanceSegmentST_RangeSTV1'
LANGUAGE C STABLE STRICT;

CREATE OPERATOR -< (
	PROCEDURE = withinDistance,
	LEFTARG = SegmentST,
	RIGHTARG = RangeST
);
