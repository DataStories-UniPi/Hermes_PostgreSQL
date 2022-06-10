/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE TYPE BoxSP;

CREATE FUNCTION inBoxSP(cstring) RETURNS BoxSP AS
	'$libdir/Hermes','inBoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION outBoxSP(BoxSP) RETURNS cstring AS
	'$libdir/Hermes','outBoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION recvBoxSP(internal) RETURNS BoxSP AS
	'$libdir/Hermes','recvBoxSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION send(BoxSP) RETURNS bytea AS
	'$libdir/Hermes','sendBoxSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE BoxSP (
	INPUT = inBoxSP,
	OUTPUT = outBoxSP,
	RECEIVE = recvBoxSP,
	SEND = send,

	INTERNALLENGTH = 16,
	ALIGNMENT = int4,
	ELEMENT = PointSP
);

/******************************************************************************/
CREATE FUNCTION equals(boxA BoxSP, boxB BoxSP) RETURNS boolean AS
	'$libdir/Hermes','equalsBoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION hash(box BoxSP) RETURNS integer AS
	'$libdir/Hermes','hashBoxSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR = (
	PROCEDURE = equals,
	LEFTARG = BoxSP,
	RIGHTARG = BoxSP,

	COMMUTATOR = =,
	HASHES
);

CREATE OPERATOR CLASS BoxSP_OPS DEFAULT FOR TYPE BoxSP USING hash AS
	OPERATOR 1 =,
	FUNCTION 1 hash(BoxSP);

/******************************************************************************/
CREATE FUNCTION getL(box BoxSP) RETURNS PointSP AS
	'$libdir/Hermes','getLBoxSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getLx(box BoxSP) RETURNS integer AS
	'$libdir/Hermes','getLxBoxSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getLy(box BoxSP) RETURNS integer AS
	'$libdir/Hermes','getLyBoxSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getH(box BoxSP) RETURNS PointSP AS
	'$libdir/Hermes','getHBoxSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getHx(box BoxSP) RETURNS integer AS
	'$libdir/Hermes','getHxBoxSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getHy(box BoxSP) RETURNS integer AS
	'$libdir/Hermes','getHyBoxSP'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION BoxSP(lx integer, ly integer, hx integer, hy integer) RETURNS BoxSP AS
	'$libdir/Hermes','constructorBoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION BoxSP(l PointSP, h PointSP) RETURNS BoxSP AS
	'$libdir/Hermes','constructorHighBoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION clone(box BoxSP) RETURNS BoxSP AS
	'$libdir/Hermes','cloneBoxSPV1'
LANGUAGE C IMMUTABLE STRICT;
