/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION HInterpolationPrecision() RETURNS integer AS
	'$libdir/Hermes','getInterpolationPrecision'
LANGUAGE C;

CREATE FUNCTION HInterpolationPrecision(interpolationPrecision integer) RETURNS void AS
	'$libdir/Hermes','setInterpolationPrecision'
LANGUAGE C STRICT;
