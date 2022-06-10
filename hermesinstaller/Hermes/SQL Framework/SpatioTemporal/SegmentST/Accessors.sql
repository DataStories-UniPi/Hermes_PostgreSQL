/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */
 
/******************************************************************************/
CREATE FUNCTION i(segment SegmentST) RETURNS PointST AS
	'$libdir/Hermes','iSegmentSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION e(segment SegmentST) RETURNS PointST AS
	'$libdir/Hermes','eSegmentSTV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION points(segment SegmentST)
RETURNS TABLE(p PointST) AS
	'$libdir/Hermes','pointsSegmentST'
LANGUAGE C IMMUTABLE STRICT;
