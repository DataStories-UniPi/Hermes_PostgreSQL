/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the Trajectory data type.
 *
 * @see @ref Trajectory
 * @see @ref data_type_trajectory
 */
/******************************************************************************/
CREATE FUNCTION HInterpolationPrecision() RETURNS integer AS
	'$libdir/Hermes','getInterpolationPrecision'
LANGUAGE C;

CREATE FUNCTION HInterpolationPrecision(interpolationPrecision integer) RETURNS void AS
	'$libdir/Hermes','setInterpolationPrecision'
LANGUAGE C STRICT;
