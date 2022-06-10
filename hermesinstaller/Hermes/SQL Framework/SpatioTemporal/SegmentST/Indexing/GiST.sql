/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION consistentGiSTSegmentST(internal, internal, smallint, oid, internal) RETURNS bool AS
	'$libdir/Hermes','consistentGiSTSegmentST'
LANGUAGE C STRICT;

CREATE FUNCTION unionGiSTSegmentST(internal, internal) RETURNS internal AS
	'$libdir/Hermes','unionGiSTSegmentST'
LANGUAGE C STRICT;

CREATE FUNCTION compressGiSTSegmentST(internal) RETURNS internal AS
	'$libdir/Hermes','compressGiSTSegmentST'
LANGUAGE C STRICT;

CREATE FUNCTION decompressGiSTSegmentST(internal) RETURNS internal AS
	'$libdir/Hermes','decompressGiSTSegmentST'
LANGUAGE C STRICT;

CREATE FUNCTION penaltyGiSTSegmentST(internal, internal, internal) RETURNS internal AS
	'$libdir/Hermes','penaltyGiSTSegmentST'
LANGUAGE C STRICT;

CREATE FUNCTION picksplitGiSTSegmentST(internal, internal) RETURNS internal AS
	'$libdir/Hermes','picksplitGiSTSegmentST'
LANGUAGE C STRICT;

CREATE FUNCTION sameGiSTSegmentST(internal, internal, internal) RETURNS internal AS
	'$libdir/Hermes','sameGiSTSegmentST'
LANGUAGE C STRICT;

CREATE FUNCTION distanceGiSTSegmentST(internal, internal, smallint, oid) RETURNS float8 AS
	'$libdir/Hermes','distanceGiSTSegmentST'
LANGUAGE C STRICT;

/******************************************************************************/
CREATE OPERATOR CLASS SegmentST_OPS DEFAULT FOR TYPE SegmentST USING gist AS
	OPERATOR 1 && (SegmentST, BoxST),
	OPERATOR 2 && (SegmentST, SegmentST),
	OPERATOR 3 ~ (SegmentST, PointST),
	OPERATOR 4 @ (SegmentST, BoxST),
	OPERATOR 5 @! (SegmentST, BoxST),
	OPERATOR 6 -< (SegmentST, RangeST),

	OPERATOR 7 && (SegmentST, Period),
	OPERATOR 8 && (SegmentST, BoxSP),
	OPERATOR 9 && (SegmentST, SegmentSP),

	OPERATOR 10 ~ (SegmentST, timestamp),
	OPERATOR 11 ~ (SegmentST, Period),
	OPERATOR 12 ~ (SegmentST, PointSP),

	OPERATOR 13 @ (SegmentST, Period),
	OPERATOR 14 @ (SegmentST, BoxSP),

	OPERATOR 15 -< (SegmentST, RangeSP),

	OPERATOR 16 <-> (SegmentST, timestamp) FOR ORDER BY pg_catalog.float_ops,
	OPERATOR 17 <-> (SegmentST, Period) FOR ORDER BY pg_catalog.float_ops,

	OPERATOR 18 <-> (SegmentST, PointSP) FOR ORDER BY pg_catalog.float_ops,
	OPERATOR 19 <-> (SegmentST, SegmentSP) FOR ORDER BY pg_catalog.float_ops,
	OPERATOR 20 <-> (SegmentST, BoxSP) FOR ORDER BY pg_catalog.float_ops,

	OPERATOR 21 &&& (SegmentST, Trajectory),
	OPERATOR 22 &&& (SegmentST, BoxST),

	FUNCTION 1 consistentGiSTSegmentST(internal, internal, smallint, oid, internal),
	FUNCTION 2 unionGiSTSegmentST(internal, internal),
	FUNCTION 3 compressGiSTSegmentST(internal),
	FUNCTION 4 decompressGiSTSegmentST(internal),
	FUNCTION 5 penaltyGiSTSegmentST(internal, internal, internal),
	FUNCTION 6 picksplitGiSTSegmentST(internal, internal),
	FUNCTION 7 sameGiSTSegmentST(internal, internal, internal),
	FUNCTION 8 distanceGiSTSegmentST(internal, internal, smallint, oid);
