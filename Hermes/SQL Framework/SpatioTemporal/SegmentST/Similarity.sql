/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION Trapezoidal(segmentA SegmentST, segmentB SegmentST) RETURNS double precision AS
	'$libdir/Hermes','TrapezoidalSegmentSTV1'
LANGUAGE C IMMUTABLE STRICT;
