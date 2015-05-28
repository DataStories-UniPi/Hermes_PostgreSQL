/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE TYPE Period;

CREATE FUNCTION inPeriod(cstring) RETURNS Period AS
	'$libdir/Hermes','inPeriodV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION outPeriod(Period) RETURNS cstring AS
	'$libdir/Hermes','outPeriodV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION recvPeriod(internal) RETURNS Period AS
	'$libdir/Hermes','recvPeriod'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION send(Period) RETURNS bytea AS
	'$libdir/Hermes','sendPeriod'
LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE Period (
	INPUT = inPeriod,
	OUTPUT = outPeriod,
	RECEIVE = recvPeriod,
	SEND = send,

	INTERNALLENGTH = 16,
	ALIGNMENT = double,
	ELEMENT = timestamp
);

/******************************************************************************/
CREATE FUNCTION equals(periodA Period, periodB Period) RETURNS boolean AS
	'$libdir/Hermes','equalsPeriodV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION hash(period Period) RETURNS integer AS
	'$libdir/Hermes','hashPeriod'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR = (
	PROCEDURE = equals,
	LEFTARG = Period,
	RIGHTARG = Period,

	COMMUTATOR = =,
	HASHES
);

CREATE OPERATOR CLASS Period_OPS DEFAULT FOR TYPE Period USING hash AS
	OPERATOR 1 =,
	FUNCTION 1 hash(Period);

/******************************************************************************/
CREATE FUNCTION getI(period Period) RETURNS timestamp AS
	'$libdir/Hermes','getIPeriod'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getE(period Period) RETURNS timestamp AS
	'$libdir/Hermes','getEPeriod'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION Period(i timestamp, e timestamp) RETURNS Period AS
	'$libdir/Hermes','constructorPeriodV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION clone(period Period) RETURNS Period AS
	'$libdir/Hermes','clonePeriodV1'
LANGUAGE C IMMUTABLE STRICT;
