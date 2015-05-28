/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION HermesLicense() RETURNS text AS
	'$libdir/Hermes','hermesLicense'
LANGUAGE C STABLE;

/******************************************************************************/
CREATE FUNCTION HermesVersionMajor() RETURNS integer AS
	'$libdir/Hermes','hermesVersionMajor'
LANGUAGE C STABLE;

CREATE FUNCTION HermesVersionMinor() RETURNS integer AS
	'$libdir/Hermes','hermesVersionMinor'
LANGUAGE C STABLE;

CREATE FUNCTION HermesVersionPatch() RETURNS integer AS
	'$libdir/Hermes','hermesVersionPatch'
LANGUAGE C STABLE;

CREATE FUNCTION HermesVersionString() RETURNS text AS
	'$libdir/Hermes','hermesVersionString'
LANGUAGE C STABLE;

CREATE FUNCTION HermesVersionCodename() RETURNS text AS
	'$libdir/Hermes','hermesVersionCodename'
LANGUAGE C STABLE;

CREATE FUNCTION HermesVersionReleaseDate() RETURNS date AS
	'$libdir/Hermes','hermesVersionReleaseDate'
LANGUAGE C STABLE;

/******************************************************************************/
CREATE FUNCTION HermesDataTypes()
RETURNS TABLE(name text, internallength integer, alignment integer) AS
	'$libdir/Hermes','hermesDataTypes'
LANGUAGE C STABLE;

CREATE FUNCTION HermesDataTypesAsXML() RETURNS xml AS
	'$libdir/Hermes','hermesDataTypesAsXML'
LANGUAGE C STABLE;
