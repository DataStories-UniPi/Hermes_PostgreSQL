/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION isStopped(segment SegmentST, tolerance double precision DEFAULT HSpatialTolerance()) RETURNS boolean AS
	'$libdir/Hermes','isStoppedSegmentSTV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION averageSpeed(segment SegmentST) RETURNS double precision AS
	'$libdir/Hermes','averageSpeedSegmentSTV1'
LANGUAGE C IMMUTABLE STRICT;
