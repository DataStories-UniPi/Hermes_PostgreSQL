/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION fx(line LineSP, x double precision) RETURNS double precision AS
	'$libdir/Hermes','fxLineSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION fy(line LineSP, y double precision) RETURNS double precision AS
	'$libdir/Hermes','fyLineSPV1'
LANGUAGE C IMMUTABLE STRICT;
