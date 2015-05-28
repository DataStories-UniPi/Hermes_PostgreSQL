/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION HTemporalTolerance() RETURNS interval AS
	'$libdir/Hermes','getTemporalTolerance'
LANGUAGE C;

CREATE FUNCTION HTemporalTolerance(temporalTolerance interval) RETURNS void AS
	'$libdir/Hermes','setTemporalTolerance'
LANGUAGE C STRICT;

CREATE FUNCTION HTemporalToleranceToSecond() RETURNS void AS
	'$libdir/Hermes','setTemporalToleranceToSecond'
LANGUAGE C;
