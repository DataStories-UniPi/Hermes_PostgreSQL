/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE TYPE PointXY;

CREATE FUNCTION inPointXY(cstring) RETURNS PointXY AS
	'$libdir/Hermes','inPointXYV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION outPointXY(PointXY) RETURNS cstring AS
	'$libdir/Hermes','outPointXYV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION recvPointXY(internal) RETURNS PointXY AS
	'$libdir/Hermes','recvPointXY'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION send(PointXY) RETURNS bytea AS
	'$libdir/Hermes','sendPointXY'
LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE PointXY (
	INPUT = inPointXY,
	OUTPUT = outPointXY,
	RECEIVE = recvPointXY,
	SEND = send,

	INTERNALLENGTH = 16,
	ALIGNMENT = double,
	ELEMENT = double precision
);

/******************************************************************************/
CREATE FUNCTION equals(pointA PointXY, pointB PointXY) RETURNS boolean AS
	'$libdir/Hermes','equalsPointXYV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION hash(point PointXY) RETURNS integer AS
	'$libdir/Hermes','hashPointXY'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR = (
	PROCEDURE = equals,
	LEFTARG = PointXY,
	RIGHTARG = PointXY,

	COMMUTATOR = =,
	HASHES
);

CREATE OPERATOR CLASS PointXY_OPS DEFAULT FOR TYPE PointXY USING hash AS
	OPERATOR 1 =,
	FUNCTION 1 hash(PointXY);

/******************************************************************************/
CREATE FUNCTION getX(point PointXY) RETURNS double precision AS
	'$libdir/Hermes','getXPointXY'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getY(point PointXY) RETURNS double precision AS
	'$libdir/Hermes','getYPointXY'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION PointXY(x double precision, y double precision) RETURNS PointXY AS
	'$libdir/Hermes','constructorPointXYV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION clone(point PointXY) RETURNS PointXY AS
	'$libdir/Hermes','clonePointXYV1'
LANGUAGE C IMMUTABLE STRICT;
