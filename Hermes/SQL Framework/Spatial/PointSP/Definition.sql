/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE TYPE PointSP;

CREATE FUNCTION inPointSP(cstring) RETURNS PointSP AS
	'$libdir/Hermes','inPointSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION outPointSP(PointSP) RETURNS cstring AS
	'$libdir/Hermes','outPointSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION recvPointSP(internal) RETURNS PointSP AS
	'$libdir/Hermes','recvPointSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION send(PointSP) RETURNS bytea AS
	'$libdir/Hermes','sendPointSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE PointSP (
	INPUT = inPointSP,
	OUTPUT = outPointSP,
	RECEIVE = recvPointSP,
	SEND = send,

	INTERNALLENGTH = 8,
	ALIGNMENT = int4,
	ELEMENT = integer
);

/******************************************************************************/
CREATE FUNCTION equals(pointA PointSP, pointB PointSP) RETURNS boolean AS
	'$libdir/Hermes','equalsPointSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION hash(point PointSP) RETURNS integer AS
	'$libdir/Hermes','hashPointSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR = (
	PROCEDURE = equals,
	LEFTARG = PointSP,
	RIGHTARG = PointSP,

	COMMUTATOR = =,
	HASHES
);

CREATE OPERATOR CLASS PointSP_OPS DEFAULT FOR TYPE PointSP USING hash AS
	OPERATOR 1 =,
	FUNCTION 1 hash(PointSP);

/******************************************************************************/
CREATE FUNCTION getX(point PointSP) RETURNS integer AS
	'$libdir/Hermes','getXPointSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getY(point PointSP) RETURNS integer AS
	'$libdir/Hermes','getYPointSP'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION PointSP(x integer, y integer) RETURNS PointSP AS
	'$libdir/Hermes','constructorPointSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION clone(point PointSP) RETURNS PointSP AS
	'$libdir/Hermes','clonePointSPV1'
LANGUAGE C IMMUTABLE STRICT;
