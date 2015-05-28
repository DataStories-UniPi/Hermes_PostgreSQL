/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE TYPE PointLL;

CREATE FUNCTION inPointLL(cstring) RETURNS PointLL AS
	'$libdir/Hermes','inPointLLV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION outPointLL(PointLL) RETURNS cstring AS
	'$libdir/Hermes','outPointLLV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION recvPointLL(internal) RETURNS PointLL AS
	'$libdir/Hermes','recvPointLL'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION send(PointLL) RETURNS bytea AS
	'$libdir/Hermes','sendPointLL'
LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE PointLL (
	INPUT = inPointLL,
	OUTPUT = outPointLL,
	RECEIVE = recvPointLL,
	SEND = send,

	INTERNALLENGTH = 16,
	ALIGNMENT = double,
	ELEMENT = double precision
);

/******************************************************************************/
CREATE FUNCTION equals(pointA PointLL, pointB PointLL) RETURNS boolean AS
	'$libdir/Hermes','equalsPointLLV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION hash(point PointLL) RETURNS integer AS
	'$libdir/Hermes','hashPointLL'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR = (
	PROCEDURE = equals,
	LEFTARG = PointLL,
	RIGHTARG = PointLL,

	COMMUTATOR = =,
	HASHES
);

CREATE OPERATOR CLASS PointLL_OPS DEFAULT FOR TYPE PointLL USING hash AS
	OPERATOR 1 =,
	FUNCTION 1 hash(PointLL);

/******************************************************************************/
CREATE FUNCTION getLon(point PointLL) RETURNS double precision AS
	'$libdir/Hermes','getLonPointLL'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getLat(point PointLL) RETURNS double precision AS
	'$libdir/Hermes','getLatPointLL'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION PointLL(lon double precision, lat double precision) RETURNS PointLL AS
	'$libdir/Hermes','constructorPointLLV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION clone(point PointLL) RETURNS PointLL AS
	'$libdir/Hermes','clonePointLLV1'
LANGUAGE C IMMUTABLE STRICT;
