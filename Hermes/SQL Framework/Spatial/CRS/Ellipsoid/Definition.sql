/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE TYPE Ellipsoid;

CREATE FUNCTION inEllipsoid(cstring) RETURNS Ellipsoid AS
	'$libdir/Hermes','inEllipsoidV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION outEllipsoid(Ellipsoid) RETURNS cstring AS
	'$libdir/Hermes','outEllipsoidV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION recvEllipsoid(internal) RETURNS Ellipsoid AS
	'$libdir/Hermes','recvEllipsoid'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION send(Ellipsoid) RETURNS bytea AS
	'$libdir/Hermes','sendEllipsoid'
LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE Ellipsoid (
	INPUT = inEllipsoid,
	OUTPUT = outEllipsoid,
	RECEIVE = recvEllipsoid,
	SEND = send,

	INTERNALLENGTH = 48,
	ALIGNMENT = double,
	ELEMENT = double precision
);

/******************************************************************************/
CREATE FUNCTION equals(ellipsoidA Ellipsoid, ellipsoidB Ellipsoid) RETURNS boolean AS
	'$libdir/Hermes','equalsEllipsoidV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION hash(ellipsoid Ellipsoid) RETURNS integer AS
	'$libdir/Hermes','hashEllipsoid'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR = (
	PROCEDURE = equals,
	LEFTARG = Ellipsoid,
	RIGHTARG = Ellipsoid,

	COMMUTATOR = =,
	HASHES
);

CREATE OPERATOR CLASS Ellipsoid_OPS DEFAULT FOR TYPE Ellipsoid USING hash AS
	OPERATOR 1 =,
	FUNCTION 1 hash(Ellipsoid);

/******************************************************************************/
CREATE FUNCTION getA(ellipsoid Ellipsoid) RETURNS double precision AS
	'$libdir/Hermes','getAEllipsoid'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getRf(ellipsoid Ellipsoid) RETURNS double precision AS
	'$libdir/Hermes','getRfEllipsoid'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getB(ellipsoid Ellipsoid) RETURNS double precision AS
	'$libdir/Hermes','getBEllipsoid'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getE1stSq(ellipsoid Ellipsoid) RETURNS double precision AS
	'$libdir/Hermes','getE1stSqEllipsoid'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getE2ndSq(ellipsoid Ellipsoid) RETURNS double precision AS
	'$libdir/Hermes','getE2ndSqEllipsoid'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getR1(ellipsoid Ellipsoid) RETURNS double precision AS
	'$libdir/Hermes','getR1Ellipsoid'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION Ellipsoid(a double precision, rf double precision) RETURNS Ellipsoid AS
	'$libdir/Hermes','constructorEllipsoidV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION clone(ellipsoid Ellipsoid) RETURNS Ellipsoid AS
	'$libdir/Hermes','cloneEllipsoidV1'
LANGUAGE C IMMUTABLE STRICT;
