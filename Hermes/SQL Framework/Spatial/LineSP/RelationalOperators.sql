/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief The implementantion in sql of LineSP.
 *
 * @details LineSP is a spatial data type used in Hermes
 *
 * @see @ref data_types_spatial
 *
 */

/******************************************************************************/
CREATE FUNCTION isParallel(lineA LineSP, lineB LineSP) RETURNS boolean AS
	'$libdir/Hermes','isParallelLineSP_LineSPV1'
LANGUAGE C IMMUTABLE STRICT;
