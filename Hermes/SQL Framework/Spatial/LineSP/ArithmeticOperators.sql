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
CREATE FUNCTION fx(line LineSP, x double precision) RETURNS double precision AS
	'$libdir/Hermes','fxLineSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION fy(line LineSP, y double precision) RETURNS double precision AS
	'$libdir/Hermes','fyLineSPV1'
LANGUAGE C IMMUTABLE STRICT;
