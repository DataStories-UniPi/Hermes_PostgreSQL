/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief The file implements the properties of the SegmentST object
 *
 * @see @ref methods
 *
 */

CREATE FUNCTION isStopped(segment SegmentST, tolerance double precision DEFAULT HSpatialTolerance()) RETURNS boolean AS
	'$libdir/Hermes','isStoppedSegmentSTV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the average speed of a segment
 *
 *	@param[in] segment the segment
 *
 *  @return the average speed of the trajectory
 */
CREATE FUNCTION averageSpeed(segment SegmentST) RETURNS double precision AS
	'$libdir/Hermes','averageSpeedSegmentSTV1'
LANGUAGE C IMMUTABLE STRICT;
