/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE TYPE BoxST;

CREATE FUNCTION inBoxST(cstring) RETURNS BoxST AS
	'$libdir/Hermes','inBoxSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION outBoxST(BoxST) RETURNS cstring AS
	'$libdir/Hermes','outBoxSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION recvBoxST(internal) RETURNS BoxST AS
	'$libdir/Hermes','recvBoxST'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION send(BoxST) RETURNS bytea AS
	'$libdir/Hermes','sendBoxST'
LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE BoxST (
	INPUT = inBoxST,
	OUTPUT = outBoxST,
	RECEIVE = recvBoxST,
	SEND = send,

	INTERNALLENGTH = 32,
	ALIGNMENT = double
);

/******************************************************************************/
CREATE FUNCTION equals(boxA BoxST, boxB BoxST) RETURNS boolean AS
	'$libdir/Hermes','equalsBoxSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION hash(box BoxST) RETURNS integer AS
	'$libdir/Hermes','hashBoxST'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR = (
	PROCEDURE = equals,
	LEFTARG = BoxST,
	RIGHTARG = BoxST,

	COMMUTATOR = =,
	HASHES
);

CREATE OPERATOR CLASS BoxST_OPS DEFAULT FOR TYPE BoxST USING hash AS
	OPERATOR 1 =,
	FUNCTION 1 hash(BoxST);

/******************************************************************************/
CREATE FUNCTION getT(box BoxST) RETURNS Period AS
	'$libdir/Hermes','getTBoxST'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getTi(box BoxST) RETURNS timestamp AS
	'$libdir/Hermes','getTiBoxST'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getTe(box BoxST) RETURNS timestamp AS
	'$libdir/Hermes','getTeBoxST'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getSp(box BoxST) RETURNS BoxSP AS
	'$libdir/Hermes','getSpBoxST'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getL(box BoxST) RETURNS PointSP AS
	'$libdir/Hermes','getLBoxST'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getLx(box BoxST) RETURNS integer AS
	'$libdir/Hermes','getLxBoxST'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getLy(box BoxST) RETURNS integer AS
	'$libdir/Hermes','getLyBoxST'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getH(box BoxST) RETURNS PointSP AS
	'$libdir/Hermes','getHBoxST'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getHx(box BoxST) RETURNS integer AS
	'$libdir/Hermes','getHxBoxST'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getHy(box BoxST) RETURNS integer AS
	'$libdir/Hermes','getHyBoxST'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION BoxST(ti timestamp, te timestamp, lx integer, ly integer, hx integer, hy integer) RETURNS BoxST AS
	'$libdir/Hermes','constructorBoxSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION BoxST(t Period, sp BoxSP) RETURNS BoxST AS
	'$libdir/Hermes','constructorHighBoxSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION clone(box BoxST) RETURNS BoxST AS
	'$libdir/Hermes','cloneBoxSTV1'
LANGUAGE C IMMUTABLE STRICT;
