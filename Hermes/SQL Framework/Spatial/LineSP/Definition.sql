/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/*
 * General Form of the line equation: A*x + B*y + C = 0
 */

/******************************************************************************/
CREATE TYPE LineSP;

CREATE FUNCTION inLineSP(cstring) RETURNS LineSP AS
	'$libdir/Hermes','inLineSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION outLineSP(LineSP) RETURNS cstring AS
	'$libdir/Hermes','outLineSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION recvLineSP(internal) RETURNS LineSP AS
	'$libdir/Hermes','recvLineSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION send(LineSP) RETURNS bytea AS
	'$libdir/Hermes','sendLineSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE LineSP (
	INPUT = inLineSP,
	OUTPUT = outLineSP,
	RECEIVE = recvLineSP,
	SEND = send,

	INTERNALLENGTH = 24,
	ALIGNMENT = double,
	ELEMENT = double precision
);

/******************************************************************************/
CREATE FUNCTION equals(lineA LineSP, lineB LineSP) RETURNS boolean AS
	'$libdir/Hermes','equalsLineSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION hash(line LineSP) RETURNS integer AS
	'$libdir/Hermes','hashLineSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR = (
	PROCEDURE = equals,
	LEFTARG = LineSP,
	RIGHTARG = LineSP,

	COMMUTATOR = =,
	HASHES
);

CREATE OPERATOR CLASS LineSP_OPS DEFAULT FOR TYPE LineSP USING hash AS
	OPERATOR 1 =,
	FUNCTION 1 hash(LineSP);

/******************************************************************************/
CREATE FUNCTION getA(line LineSP) RETURNS double precision AS
	'$libdir/Hermes','getALineSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getB(line LineSP) RETURNS double precision AS
	'$libdir/Hermes','getBLineSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getC(line LineSP) RETURNS double precision AS
	'$libdir/Hermes','getCLineSP'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION LineSP(A double precision, B double precision, C double precision) RETURNS LineSP AS
	'$libdir/Hermes','constructorLineSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION clone(line LineSP) RETURNS LineSP AS
	'$libdir/Hermes','cloneLineSPV1'
LANGUAGE C IMMUTABLE STRICT;
