/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION BoxSP(segment SegmentSP) RETURNS BoxSP AS
	'$libdir/Hermes','SegmentSP2BoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE CAST (SegmentSP AS BoxSP) WITH FUNCTION BoxSP(SegmentSP) AS ASSIGNMENT;

/******************************************************************************/
CREATE FUNCTION LineSP(segment SegmentSP) RETURNS LineSP AS
	'$libdir/Hermes','SegmentSP2LineSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE CAST (SegmentSP AS LineSP) WITH FUNCTION LineSP(SegmentSP) AS ASSIGNMENT;
