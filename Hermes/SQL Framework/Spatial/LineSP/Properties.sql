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
CREATE FUNCTION angleXX(line LineSP) RETURNS double precision AS
	'$libdir/Hermes','angleXXLineSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION slope(line LineSP) RETURNS double precision AS
	'$libdir/Hermes','slopeLineSPV1'
LANGUAGE C IMMUTABLE STRICT;
