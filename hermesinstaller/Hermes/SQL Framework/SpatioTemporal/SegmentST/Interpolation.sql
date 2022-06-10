/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION atInstantSP(segment SegmentST, ts timestamp, checkContainment boolean DEFAULT true) RETURNS PointSP AS
	'$libdir/Hermes','atInstantSPSegmentSTV1'
LANGUAGE C STABLE STRICT;

CREATE FUNCTION atInstant(segment SegmentST, ts timestamp, checkContainment boolean DEFAULT true) RETURNS PointST AS
	'$libdir/Hermes','atInstantSegmentSTV1'
LANGUAGE C STABLE STRICT;

/******************************************************************************/
CREATE FUNCTION atPeriodSP(segment SegmentST, period Period,
	OUT n integer, OUT s SegmentSP, OUT p PointSP
) AS
	'$libdir/Hermes','atPeriodSPSegmentSTV1'
LANGUAGE C STABLE STRICT;

CREATE FUNCTION atPeriod(segment SegmentST, period Period,
	OUT n integer, OUT s SegmentST, OUT p PointST
) AS
	'$libdir/Hermes','atPeriodSegmentSTV1'
LANGUAGE C STABLE STRICT;

/******************************************************************************/
CREATE FUNCTION atPointT(segment SegmentST, point PointSP, checkContainment boolean DEFAULT true) RETURNS timestamp AS
	'$libdir/Hermes','atPointTSegmentSTV1'
LANGUAGE C STABLE STRICT;

CREATE FUNCTION atPoint(segment SegmentST, point PointSP, checkContainment boolean DEFAULT true) RETURNS PointST AS
	'$libdir/Hermes','atPointSegmentSTV1'
LANGUAGE C STABLE STRICT;

/******************************************************************************/
CREATE FUNCTION atBoxT(segment SegmentST, box BoxSP,
	OUT n integer, OUT p Period, OUT ts timestamp
) AS
	'$libdir/Hermes','atBoxTSegmentSTV1'
LANGUAGE C STABLE STRICT;

CREATE FUNCTION atBox(segment SegmentST, box BoxSP,
	OUT n integer, OUT s SegmentST, OUT p PointST
) AS
	'$libdir/Hermes','atBoxSegmentSTV1'
LANGUAGE C STABLE STRICT;

/******************************************************************************/
CREATE FUNCTION atBox(segment SegmentST, box BoxST,
	OUT n integer, OUT s SegmentST, OUT p PointST
) AS
	'$libdir/Hermes','atBoxSTSegmentSTV1'
LANGUAGE C STABLE STRICT;

/******************************************************************************/
CREATE FUNCTION intersectionPoint(segmentA SegmentST, segmentB SegmentST) RETURNS PointST AS
	'$libdir/Hermes','intersectionPointSegmentST_SegmentSTV1'
LANGUAGE C STABLE STRICT;
