/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION intersection(boxA BoxST, boxB BoxST) RETURNS BoxST AS
	'$libdir/Hermes','intersectionBoxST_BoxSTV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION HUnion(box BoxST, point PointST) RETURNS BoxST AS
	'$libdir/Hermes','unionBoxST_PointSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION HUnion(box BoxST, segment SegmentST) RETURNS BoxST AS
	'$libdir/Hermes','unionBoxST_SegmentSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION HUnion(boxA BoxST, boxB BoxST) RETURNS BoxST AS
	'$libdir/Hermes','unionBoxST_BoxSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE AGGREGATE HUnion(BoxST) (
	SFUNC = HUnion(BoxST, BoxST),
	STYPE = BoxST
);
