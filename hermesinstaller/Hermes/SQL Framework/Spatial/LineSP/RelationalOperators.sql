/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION isParallel(lineA LineSP, lineB LineSP) RETURNS boolean AS
	'$libdir/Hermes','isParallelLineSP_LineSPV1'
LANGUAGE C IMMUTABLE STRICT;
