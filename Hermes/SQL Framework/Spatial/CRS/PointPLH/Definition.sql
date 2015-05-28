/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE TYPE PointPLH;

CREATE FUNCTION inPointPLH(cstring) RETURNS PointPLH AS
	'$libdir/Hermes','inPointPLHV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION outPointPLH(PointPLH) RETURNS cstring AS
	'$libdir/Hermes','outPointPLHV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION recvPointPLH(internal) RETURNS PointPLH AS
	'$libdir/Hermes','recvPointPLH'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION send(PointPLH) RETURNS bytea AS
	'$libdir/Hermes','sendPointPLH'
LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE PointPLH (
	INPUT = inPointPLH,
	OUTPUT = outPointPLH,
	RECEIVE = recvPointPLH,
	SEND = send,

	INTERNALLENGTH = 24,
	ALIGNMENT = double,
	ELEMENT = double precision
);

/******************************************************************************/
CREATE FUNCTION equals(pointA PointPLH, pointB PointPLH) RETURNS boolean AS
	'$libdir/Hermes','equalsPointPLHV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION hash(point PointPLH) RETURNS integer AS
	'$libdir/Hermes','hashPointPLH'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR = (
	PROCEDURE = equals,
	LEFTARG = PointPLH,
	RIGHTARG = PointPLH,

	COMMUTATOR = =,
	HASHES
);

CREATE OPERATOR CLASS PointPLH_OPS DEFAULT FOR TYPE PointPLH USING hash AS
	OPERATOR 1 =,
	FUNCTION 1 hash(PointPLH);

/******************************************************************************/
CREATE FUNCTION getPhi(point PointPLH) RETURNS double precision AS
	'$libdir/Hermes','getPhiPointPLH'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getLambda(point PointPLH) RETURNS double precision AS
	'$libdir/Hermes','getLambdaPointPLH'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getH(point PointPLH) RETURNS double precision AS
	'$libdir/Hermes','getHPointPLH'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION PointPLH(phi double precision, lambda double precision, h double precision) RETURNS PointPLH AS
	'$libdir/Hermes','constructorPointPLHV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION clone(point PointPLH) RETURNS PointPLH AS
	'$libdir/Hermes','clonePointPLHV1'
LANGUAGE C IMMUTABLE STRICT;
