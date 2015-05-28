/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION angleXX(line LineSP) RETURNS double precision AS
	'$libdir/Hermes','angleXXLineSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION slope(line LineSP) RETURNS double precision AS
	'$libdir/Hermes','slopeLineSPV1'
LANGUAGE C IMMUTABLE STRICT;
