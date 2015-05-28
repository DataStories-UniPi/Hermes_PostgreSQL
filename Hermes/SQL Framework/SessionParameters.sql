/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION HZeroTolerance() RETURNS double precision AS
	'$libdir/Hermes','getZeroTolerance'
LANGUAGE C;

CREATE FUNCTION HZeroTolerance(zeroTolerance double precision) RETURNS void AS
	'$libdir/Hermes','setZeroTolerance'
LANGUAGE C STRICT;

CREATE FUNCTION HZeroToleranceDefault() RETURNS void AS
	'$libdir/Hermes','setDefaultZeroTolerance'
LANGUAGE C;

/******************************************************************************/
CREATE FUNCTION HIsVerbose() RETURNS boolean AS
	'$libdir/Hermes','isVerbose'
LANGUAGE C;

CREATE FUNCTION HToggleVerbose() RETURNS void AS
	'$libdir/Hermes','toggleVerbose'
LANGUAGE C;

CREATE FUNCTION HVerbosityLevel() RETURNS integer AS
	'$libdir/Hermes','getVerbosityLevel'
LANGUAGE C;

CREATE FUNCTION HVerbosityLevel(verbosityLevel integer) RETURNS void AS
	'$libdir/Hermes','setVerbosityLevel'
LANGUAGE C STRICT;

/******************************************************************************/
CREATE FUNCTION HCurrentDatasetID() RETURNS integer AS
	'$libdir/Hermes','getCurrentDatasetID'
LANGUAGE C;

CREATE FUNCTION HCurrentDatasetID(currentDatasetID integer) RETURNS void AS
	'$libdir/Hermes','setCurrentDatasetID'
LANGUAGE C STRICT;
