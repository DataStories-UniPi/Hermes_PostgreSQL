/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE TYPE PointXYZ;

CREATE FUNCTION inPointXYZ(cstring) RETURNS PointXYZ AS
	'$libdir/Hermes','inPointXYZV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION outPointXYZ(PointXYZ) RETURNS cstring AS
	'$libdir/Hermes','outPointXYZV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION recvPointXYZ(internal) RETURNS PointXYZ AS
	'$libdir/Hermes','recvPointXYZ'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION send(PointXYZ) RETURNS bytea AS
	'$libdir/Hermes','sendPointXYZ'
LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE PointXYZ (
	INPUT = inPointXYZ,
	OUTPUT = outPointXYZ,
	RECEIVE = recvPointXYZ,
	SEND = send,

	INTERNALLENGTH = 24,
	ALIGNMENT = double,
	ELEMENT = double precision
);

/******************************************************************************/
CREATE FUNCTION equals(pointA PointXYZ, pointB PointXYZ) RETURNS boolean AS
	'$libdir/Hermes','equalsPointXYZV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION hash(point PointXYZ) RETURNS integer AS
	'$libdir/Hermes','hashPointXYZ'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR = (
	PROCEDURE = equals,
	LEFTARG = PointXYZ,
	RIGHTARG = PointXYZ,

	COMMUTATOR = =,
	HASHES
);

CREATE OPERATOR CLASS PointXYZ_OPS DEFAULT FOR TYPE PointXYZ USING hash AS
	OPERATOR 1 =,
	FUNCTION 1 hash(PointXYZ);

/******************************************************************************/
CREATE FUNCTION getX(point PointXYZ) RETURNS double precision AS
	'$libdir/Hermes','getXPointXYZ'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getY(point PointXYZ) RETURNS double precision AS
	'$libdir/Hermes','getYPointXYZ'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getZ(point PointXYZ) RETURNS double precision AS
	'$libdir/Hermes','getZPointXYZ'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION PointXYZ(x double precision, y double precision, z double precision) RETURNS PointXYZ AS
	'$libdir/Hermes','constructorPointXYZV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION clone(point PointXYZ) RETURNS PointXYZ AS
	'$libdir/Hermes','clonePointXYZV1'
LANGUAGE C IMMUTABLE STRICT;
