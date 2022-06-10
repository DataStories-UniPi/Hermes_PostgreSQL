/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE TYPE Vector2D;

CREATE FUNCTION inVector2D(cstring) RETURNS Vector2D AS
	'$libdir/Hermes','inVector2DV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION outVector2D(Vector2D) RETURNS cstring AS
	'$libdir/Hermes','outVector2DV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION recvVector2D(internal) RETURNS Vector2D AS
	'$libdir/Hermes','recvVector2D'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION send(Vector2D) RETURNS bytea AS
	'$libdir/Hermes','sendVector2D'
LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE Vector2D (
	INPUT = inVector2D,
	OUTPUT = outVector2D,
	RECEIVE = recvVector2D,
	SEND = send,

	INTERNALLENGTH = 16,
	ALIGNMENT = double,
	ELEMENT = double precision
);

/******************************************************************************/
CREATE FUNCTION equals(vectorA Vector2D, vectorB Vector2D) RETURNS boolean AS
	'$libdir/Hermes','equalsVector2DV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION hash(vector Vector2D) RETURNS integer AS
	'$libdir/Hermes','hashVector2D'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR = (
	PROCEDURE = equals,
	LEFTARG = Vector2D,
	RIGHTARG = Vector2D,

	COMMUTATOR = =,
	HASHES
);

CREATE OPERATOR CLASS Vector2D_OPS DEFAULT FOR TYPE Vector2D USING hash AS
	OPERATOR 1 =,
	FUNCTION 1 hash(Vector2D);

/******************************************************************************/
CREATE FUNCTION getX(vector Vector2D) RETURNS double precision AS
	'$libdir/Hermes','getXVector2D'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getY(vector Vector2D) RETURNS double precision AS
	'$libdir/Hermes','getYVector2D'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION Vector2D(x double precision, y double precision) RETURNS Vector2D AS
	'$libdir/Hermes','constructorVector2DV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION clone(vector Vector2D) RETURNS Vector2D AS
	'$libdir/Hermes','cloneVector2DV1'
LANGUAGE C IMMUTABLE STRICT;
