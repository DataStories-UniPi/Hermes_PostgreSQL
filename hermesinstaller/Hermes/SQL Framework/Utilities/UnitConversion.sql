/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION km2metres(km double precision) RETURNS double precision AS
	'$libdir/Hermes','km2metresV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION metres2km(metres double precision) RETURNS double precision AS
	'$libdir/Hermes','metres2kmV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION nm2metres(nm double precision) RETURNS double precision AS
	'$libdir/Hermes','nm2metresV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION metres2nm(metres double precision) RETURNS double precision AS
	'$libdir/Hermes','metres2nmV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION kmph2mps(kmph double precision) RETURNS double precision AS
	'$libdir/Hermes','kmph2mpsV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION mps2kmph(mps double precision) RETURNS double precision AS
	'$libdir/Hermes','mps2kmphV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION knots2mps(knots double precision) RETURNS double precision AS
	'$libdir/Hermes','knots2mpsV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION mps2knots(mps double precision) RETURNS double precision AS
	'$libdir/Hermes','mps2knotsV1'
LANGUAGE C IMMUTABLE STRICT;
