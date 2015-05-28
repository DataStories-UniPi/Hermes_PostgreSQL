/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION distance(box BoxSP, point PointSP) RETURNS double precision AS
	'$libdir/Hermes','distanceBoxSP_PointSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION distance(box BoxSP, segment SegmentSP) RETURNS double precision AS
	'$libdir/Hermes','distanceBoxSP_SegmentSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION distance(boxA BoxSP, boxB BoxSP) RETURNS double precision AS
	'$libdir/Hermes','distanceBoxSP_BoxSPV1'
LANGUAGE C IMMUTABLE STRICT;
