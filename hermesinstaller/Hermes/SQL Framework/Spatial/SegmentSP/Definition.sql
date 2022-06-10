/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE TYPE SegmentSP;

CREATE FUNCTION inSegmentSP(cstring) RETURNS SegmentSP AS
	'$libdir/Hermes','inSegmentSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION outSegmentSP(SegmentSP) RETURNS cstring AS
	'$libdir/Hermes','outSegmentSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION recvSegmentSP(internal) RETURNS SegmentSP AS
	'$libdir/Hermes','recvSegmentSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION send(SegmentSP) RETURNS bytea AS
	'$libdir/Hermes','sendSegmentSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE SegmentSP (
	INPUT = inSegmentSP,
	OUTPUT = outSegmentSP,
	RECEIVE = recvSegmentSP,
	SEND = send,

	INTERNALLENGTH = 16,
	ALIGNMENT = int4,
	ELEMENT = PointSP
);

/******************************************************************************/
CREATE FUNCTION equals(segmentA SegmentSP, segmentB SegmentSP) RETURNS boolean AS
	'$libdir/Hermes','equalsSegmentSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION hash(segment SegmentSP) RETURNS integer AS
	'$libdir/Hermes','hashSegmentSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR = (
	PROCEDURE = equals,
	LEFTARG = SegmentSP,
	RIGHTARG = SegmentSP,

	COMMUTATOR = =,
	HASHES
);

CREATE OPERATOR CLASS SegmentSP_OPS DEFAULT FOR TYPE SegmentSP USING hash AS
	OPERATOR 1 =,
	FUNCTION 1 hash(SegmentSP);

/******************************************************************************/
CREATE FUNCTION getI(segment SegmentSP) RETURNS PointSP AS
	'$libdir/Hermes','getISegmentSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getIx(segment SegmentSP) RETURNS integer AS
	'$libdir/Hermes','getIxSegmentSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getIy(segment SegmentSP) RETURNS integer AS
	'$libdir/Hermes','getIySegmentSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getE(segment SegmentSP) RETURNS PointSP AS
	'$libdir/Hermes','getESegmentSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getEx(segment SegmentSP) RETURNS integer AS
	'$libdir/Hermes','getExSegmentSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getEy(segment SegmentSP) RETURNS integer AS
	'$libdir/Hermes','getEySegmentSP'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION SegmentSP(ix integer, iy integer, ex integer, ey integer) RETURNS SegmentSP AS
	'$libdir/Hermes','constructorSegmentSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION SegmentSP(i PointSP, e PointSP) RETURNS SegmentSP AS
	'$libdir/Hermes','constructorHighSegmentSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION clone(segment SegmentSP) RETURNS SegmentSP AS
	'$libdir/Hermes','cloneSegmentSPV1'
LANGUAGE C IMMUTABLE STRICT;
