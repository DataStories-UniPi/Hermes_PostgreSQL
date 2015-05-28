/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION resize(segment SegmentSP, nlength double precision) RETURNS SegmentSP AS
	'$libdir/Hermes','resizeSegmentSPV1'
LANGUAGE C IMMUTABLE STRICT;
