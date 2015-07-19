/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief The implementantion in sql of PointST.
 *
 * @details PointST is a spatial data type used in Hermes
 *
 * @see @ref data_types_spatio_temp
 *
 */

/******************************************************************************/
CREATE TYPE PointST;

CREATE FUNCTION inPointST(cstring) RETURNS PointST AS
	'$libdir/Hermes','inPointSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION outPointST(PointST) RETURNS cstring AS
	'$libdir/Hermes','outPointSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION recvPointST(internal) RETURNS PointST AS
	'$libdir/Hermes','recvPointST'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION send(PointST) RETURNS bytea AS
	'$libdir/Hermes','sendPointST'
LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE PointST (
	INPUT = inPointST,
	OUTPUT = outPointST,
	RECEIVE = recvPointST,
	SEND = send,

	INTERNALLENGTH = 16,
	ALIGNMENT = double
);

/******************************************************************************/
CREATE FUNCTION equals(pointA PointST, pointB PointST) RETURNS boolean AS
	'$libdir/Hermes','equalsPointSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION hash(point PointST) RETURNS integer AS
	'$libdir/Hermes','hashPointST'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR = (
	PROCEDURE = equals,
	LEFTARG = PointST,
	RIGHTARG = PointST,

	COMMUTATOR = =,
	HASHES
);

CREATE OPERATOR CLASS PointST_OPS DEFAULT FOR TYPE PointST USING hash AS
	OPERATOR 1 =,
	FUNCTION 1 hash(PointST);

/******************************************************************************/
CREATE FUNCTION getT(point PointST) RETURNS timestamp AS
	'$libdir/Hermes','getTPointST'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getSp(point PointST) RETURNS PointSP AS
	'$libdir/Hermes','getSpPointST'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getX(point PointST) RETURNS integer AS
	'$libdir/Hermes','getXPointST'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getY(point PointST) RETURNS integer AS
	'$libdir/Hermes','getYPointST'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION PointST(t timestamp, x integer, y integer) RETURNS PointST AS
	'$libdir/Hermes','constructorPointSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION PointST(t timestamp, sp PointSP) RETURNS PointST AS
	'$libdir/Hermes','constructorHighPointSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION clone(point PointST) RETURNS PointST AS
	'$libdir/Hermes','clonePointSTV1'
LANGUAGE C IMMUTABLE STRICT;
