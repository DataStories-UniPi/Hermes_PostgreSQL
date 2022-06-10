/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION round(ts timestamp) RETURNS timestamp AS
	'$libdir/Hermes','roundTimestampV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION degrees360(degrees180 double precision) RETURNS double precision AS
	'$libdir/Hermes','degrees360V1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION min(a real, b real) RETURNS real AS
	'$libdir/Hermes','minFloat4V1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION max(a real, b real) RETURNS real AS
	'$libdir/Hermes','maxFloat4V1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION min(a double precision, b double precision) RETURNS double precision AS
	'$libdir/Hermes','minFloat8V1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION max(a double precision, b double precision) RETURNS double precision AS
	'$libdir/Hermes','maxFloat8V1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION min(a smallint, b smallint) RETURNS smallint AS
	'$libdir/Hermes','minInt16V1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION max(a smallint, b smallint) RETURNS smallint AS
	'$libdir/Hermes','maxInt16V1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION min(a integer, b integer) RETURNS integer AS
	'$libdir/Hermes','minInt32V1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION max(a integer, b integer) RETURNS integer AS
	'$libdir/Hermes','maxInt32V1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION min(a bigint, b bigint) RETURNS bigint AS
	'$libdir/Hermes','minInt64V1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION max(a bigint, b bigint) RETURNS bigint AS
	'$libdir/Hermes','maxInt64V1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION min(a timestamp, b timestamp) RETURNS timestamp AS
	'$libdir/Hermes','minTimestampV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION max(a timestamp, b timestamp) RETURNS timestamp AS
	'$libdir/Hermes','maxTimestampV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION min(a interval, b interval) RETURNS interval AS
	'$libdir/Hermes','minIntervalV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION max(a interval, b interval) RETURNS interval AS
	'$libdir/Hermes','maxIntervalV1'
LANGUAGE C IMMUTABLE STRICT;
