/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION intersection(boxA BoxSP, boxB BoxSP) RETURNS BoxSP AS
	'$libdir/Hermes','intersectionBoxSP_BoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION HUnion(box BoxSP, point PointSP) RETURNS BoxSP AS
	'$libdir/Hermes','unionBoxSP_PointSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION HUnion(box BoxSP, segment SegmentSP) RETURNS BoxSP AS
	'$libdir/Hermes','unionBoxSP_SegmentSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION HUnion(boxA BoxSP, boxB BoxSP) RETURNS BoxSP AS
	'$libdir/Hermes','unionBoxSP_BoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE AGGREGATE HUnion(BoxSP) (
	SFUNC = HUnion(BoxSP, BoxSP),
	STYPE = BoxSP
);
