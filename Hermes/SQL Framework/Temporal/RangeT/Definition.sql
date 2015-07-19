/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief The implementantion in sql of Period.
 *
 * @details RangeT is a temporal data type used in Hermes
 *
 * @see @ref data_types_temporal
 *
 */

/******************************************************************************/
CREATE TYPE RangeT;

CREATE FUNCTION inRangeT(cstring) RETURNS RangeT AS
	'$libdir/Hermes','inRangeTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION outRangeT(RangeT) RETURNS cstring AS
	'$libdir/Hermes','outRangeTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION recvRangeT(internal) RETURNS RangeT AS
	'$libdir/Hermes','recvRangeT'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION send(RangeT) RETURNS bytea AS
	'$libdir/Hermes','sendRangeT'
LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE RangeT (
	INPUT = inRangeT,
	OUTPUT = outRangeT,
	RECEIVE = recvRangeT,
	SEND = send,

	INTERNALLENGTH = 24,
	ALIGNMENT = double
);

/******************************************************************************/
CREATE FUNCTION equals(rangeA RangeT, rangeB RangeT) RETURNS boolean AS
	'$libdir/Hermes','equalsRangeTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION hash(range RangeT) RETURNS integer AS
	'$libdir/Hermes','hashRangeT'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR = (
	PROCEDURE = equals,
	LEFTARG = RangeT,
	RIGHTARG = RangeT,

	COMMUTATOR = =,
	HASHES
);

CREATE OPERATOR CLASS RangeT_OPS DEFAULT FOR TYPE RangeT USING hash AS
	OPERATOR 1 =,
	FUNCTION 1 hash(RangeT);

/******************************************************************************/
CREATE FUNCTION getR(range RangeT) RETURNS interval AS
	'$libdir/Hermes','getRRangeT'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getC(range RangeT) RETURNS timestamp AS
	'$libdir/Hermes','getCRangeT'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION RangeT(r interval, c timestamp) RETURNS RangeT AS
	'$libdir/Hermes','constructorRangeTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION clone(range RangeT) RETURNS RangeT AS
	'$libdir/Hermes','cloneRangeTV1'
LANGUAGE C IMMUTABLE STRICT;
