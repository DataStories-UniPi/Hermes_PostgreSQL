/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the SegmentST data type.
 *
 * @see @ref SegmentST
 * @see @ref data_types_spatio_temp
 */

/******************************************************************************/
CREATE FUNCTION distance(segment SegmentST, ts timestamp) RETURNS double precision AS
	'$libdir/Hermes','distanceSegmentST_TimestampV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION distance(segment SegmentST, period Period) RETURNS double precision AS
	'$libdir/Hermes','distanceSegmentST_PeriodV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR <-> (
	PROCEDURE = distance,
	LEFTARG = SegmentST,
	RIGHTARG = timestamp
);

CREATE OPERATOR <-> (
	PROCEDURE = distance,
	LEFTARG = SegmentST,
	RIGHTARG = Period
);

/******************************************************************************/
CREATE FUNCTION distance(segment SegmentST, point PointSP) RETURNS double precision AS
	'$libdir/Hermes','distanceSegmentST_PointSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION distance(segment SegmentST, segmentSpatialOnly SegmentSP) RETURNS double precision AS
	'$libdir/Hermes','distanceSegmentST_SegmentSPV1'
LANGUAGE C STABLE STRICT;

CREATE FUNCTION distance(segment SegmentST, box BoxSP) RETURNS double precision AS
	'$libdir/Hermes','distanceSegmentST_BoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR <-> (
	PROCEDURE = distance,
	LEFTARG = SegmentST,
	RIGHTARG = PointSP
);

CREATE OPERATOR <-> (
	PROCEDURE = distance,
	LEFTARG = SegmentST,
	RIGHTARG = SegmentSP
);

CREATE OPERATOR <-> (
	PROCEDURE = distance,
	LEFTARG = SegmentST,
	RIGHTARG = BoxSP
);
