/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE TYPE Vector3D;

CREATE FUNCTION inVector3D(cstring) RETURNS Vector3D AS
	'$libdir/Hermes','inVector3DV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION outVector3D(Vector3D) RETURNS cstring AS
	'$libdir/Hermes','outVector3DV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION recvVector3D(internal) RETURNS Vector3D AS
	'$libdir/Hermes','recvVector3D'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION send(Vector3D) RETURNS bytea AS
	'$libdir/Hermes','sendVector3D'
LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE Vector3D (
	INPUT = inVector3D,
	OUTPUT = outVector3D,
	RECEIVE = recvVector3D,
	SEND = send,

	INTERNALLENGTH = 24,
	ALIGNMENT = double,
	ELEMENT = double precision
);

/******************************************************************************/
CREATE FUNCTION equals(vectorA Vector3D, vectorB Vector3D) RETURNS boolean AS
	'$libdir/Hermes','equalsVector3DV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION hash(vector Vector3D) RETURNS integer AS
	'$libdir/Hermes','hashVector3D'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR = (
	PROCEDURE = equals,
	LEFTARG = Vector3D,
	RIGHTARG = Vector3D,

	COMMUTATOR = =,
	HASHES
);

CREATE OPERATOR CLASS Vector3D_OPS DEFAULT FOR TYPE Vector3D USING hash AS
	OPERATOR 1 =,
	FUNCTION 1 hash(Vector3D);

/******************************************************************************/
CREATE FUNCTION getX(vector Vector3D) RETURNS double precision AS
	'$libdir/Hermes','getXVector3D'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getY(vector Vector3D) RETURNS double precision AS
	'$libdir/Hermes','getYVector3D'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getZ(vector Vector3D) RETURNS double precision AS
	'$libdir/Hermes','getZVector3D'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION Vector3D(x double precision, y double precision, z double precision) RETURNS Vector3D AS
	'$libdir/Hermes','constructorVector3DV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION clone(vector Vector3D) RETURNS Vector3D AS
	'$libdir/Hermes','cloneVector3DV1'
LANGUAGE C IMMUTABLE STRICT;
