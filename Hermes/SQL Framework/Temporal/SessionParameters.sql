/*
* @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the functions for setting the temporal torelance.
 *
 * @see @ref data_types_temporal
 */

/** @brief The function returns the temporal tolerance of Hermes MOD
 *
 *
 *  @return the temporal tolerance
 */
CREATE FUNCTION HTemporalTolerance() RETURNS interval AS
	'$libdir/Hermes','getTemporalTolerance'
LANGUAGE C;

/** @brief The function that sets the temporal tolerance of Hermes MOD
 *
 *	@param[in] temporalTolerance the temporal tolerance
 *
 */
CREATE FUNCTION HTemporalTolerance(temporalTolerance interval) RETURNS void AS
	'$libdir/Hermes','setTemporalTolerance'
LANGUAGE C STRICT;

/** @brief The function set the temporal tolerance of Hermes MOD 
 * 	to a second
 *
 */
CREATE FUNCTION HTemporalToleranceToSecond() RETURNS void AS
	'$libdir/Hermes','setTemporalToleranceToSecond'
LANGUAGE C;
