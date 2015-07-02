/*
* @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the functions for setting the spatial torelance.
 *
 * @see @ref data_types_temporal
 */

/******************************************************************************/
CREATE FUNCTION HSpatialTolerance() RETURNS double precision AS
	'$libdir/Hermes','getSpatialTolerance'
LANGUAGE C;

CREATE FUNCTION HSpatialTolerance(spatialTolerance double precision) RETURNS void AS
	'$libdir/Hermes','setSpatialTolerance'
LANGUAGE C STRICT;

CREATE FUNCTION HSpatialToleranceToCentimetre() RETURNS void AS
	'$libdir/Hermes','setSpatialToleranceToCentimetre'
LANGUAGE C;

CREATE FUNCTION HSpatialToleranceToMetre() RETURNS void AS
	'$libdir/Hermes','setSpatialToleranceToMetre'
LANGUAGE C;

CREATE FUNCTION HSpatialToleranceToKilometre() RETURNS void AS
	'$libdir/Hermes','setSpatialToleranceToKilometre'
LANGUAGE C;

CREATE FUNCTION HSpatialToleranceToFoot() RETURNS void AS
	'$libdir/Hermes','setSpatialToleranceToFoot'
LANGUAGE C;
