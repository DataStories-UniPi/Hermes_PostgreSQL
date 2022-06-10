/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE TYPE PointENU;

CREATE FUNCTION inPointENU(cstring) RETURNS PointENU AS
	'$libdir/Hermes','inPointENUV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION outPointENU(PointENU) RETURNS cstring AS
	'$libdir/Hermes','outPointENUV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION recvPointENU(internal) RETURNS PointENU AS
	'$libdir/Hermes','recvPointENU'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION send(PointENU) RETURNS bytea AS
	'$libdir/Hermes','sendPointENU'
LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE PointENU (
	INPUT = inPointENU,
	OUTPUT = outPointENU,
	RECEIVE = recvPointENU,
	SEND = send,

	INTERNALLENGTH = 24,
	ALIGNMENT = double,
	ELEMENT = double precision
);

/******************************************************************************/
CREATE FUNCTION equals(pointA PointENU, pointB PointENU) RETURNS boolean AS
	'$libdir/Hermes','equalsPointENUV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION hash(point PointENU) RETURNS integer AS
	'$libdir/Hermes','hashPointENU'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR = (
	PROCEDURE = equals,
	LEFTARG = PointENU,
	RIGHTARG = PointENU,

	COMMUTATOR = =,
	HASHES
);

CREATE OPERATOR CLASS PointENU_OPS DEFAULT FOR TYPE PointENU USING hash AS
	OPERATOR 1 =,
	FUNCTION 1 hash(PointENU);

/******************************************************************************/
CREATE FUNCTION getE(point PointENU) RETURNS double precision AS
	'$libdir/Hermes','getEPointENU'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getN(point PointENU) RETURNS double precision AS
	'$libdir/Hermes','getNPointENU'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getU(point PointENU) RETURNS double precision AS
	'$libdir/Hermes','getUPointENU'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION PointENU(e double precision, n double precision, u double precision) RETURNS PointENU AS
	'$libdir/Hermes','constructorPointENUV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION clone(point PointENU) RETURNS PointENU AS
	'$libdir/Hermes','clonePointENUV1'
LANGUAGE C IMMUTABLE STRICT;
