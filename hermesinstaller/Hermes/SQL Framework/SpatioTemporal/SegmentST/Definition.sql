/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE TYPE SegmentST;

CREATE FUNCTION inSegmentST(cstring) RETURNS SegmentST AS
	'$libdir/Hermes','inSegmentSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION outSegmentST(SegmentST) RETURNS cstring AS
	'$libdir/Hermes','outSegmentSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION recvSegmentST(internal) RETURNS SegmentST AS
	'$libdir/Hermes','recvSegmentST'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION send(SegmentST) RETURNS bytea AS
	'$libdir/Hermes','sendSegmentST'
LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE SegmentST (
	INPUT = inSegmentST,
	OUTPUT = outSegmentST,
	RECEIVE = recvSegmentST,
	SEND = send,

	INTERNALLENGTH = 32,
	ALIGNMENT = double
);

/******************************************************************************/
CREATE FUNCTION equals(segmentA SegmentST, segmentB SegmentST) RETURNS boolean AS
	'$libdir/Hermes','equalsSegmentSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION hash(segment SegmentST) RETURNS integer AS
	'$libdir/Hermes','hashSegmentST'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR = (
	PROCEDURE = equals,
	LEFTARG = SegmentST,
	RIGHTARG = SegmentST,

	COMMUTATOR = =,
	HASHES
);

CREATE OPERATOR CLASS SegmentST_OPS DEFAULT FOR TYPE SegmentST USING hash AS
	OPERATOR 1 =,
	FUNCTION 1 hash(SegmentST);

/******************************************************************************/
CREATE FUNCTION getT(segment SegmentST) RETURNS Period AS
	'$libdir/Hermes','getTSegmentST'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getTi(segment SegmentST) RETURNS timestamp AS
	'$libdir/Hermes','getTiSegmentST'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getTe(segment SegmentST) RETURNS timestamp AS
	'$libdir/Hermes','getTeSegmentST'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getSp(segment SegmentST) RETURNS SegmentSP AS
	'$libdir/Hermes','getSpSegmentST'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getI(segment SegmentST) RETURNS PointSP AS
	'$libdir/Hermes','getISegmentST'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getIx(segment SegmentST) RETURNS integer AS
	'$libdir/Hermes','getIxSegmentST'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getIy(segment SegmentST) RETURNS integer AS
	'$libdir/Hermes','getIySegmentST'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getE(segment SegmentST) RETURNS PointSP AS
	'$libdir/Hermes','getESegmentST'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getEx(segment SegmentST) RETURNS integer AS
	'$libdir/Hermes','getExSegmentST'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getEy(segment SegmentST) RETURNS integer AS
	'$libdir/Hermes','getEySegmentST'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION SegmentST(ti timestamp, ix integer, iy integer, te timestamp, ex integer, ey integer) RETURNS SegmentST AS
	'$libdir/Hermes','constructorSegmentSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION SegmentST(i PointST, e PointST) RETURNS SegmentST AS
	'$libdir/Hermes','constructorHighSegmentSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION clone(segment SegmentST) RETURNS SegmentST AS
	'$libdir/Hermes','cloneSegmentSTV1'
LANGUAGE C IMMUTABLE STRICT;
