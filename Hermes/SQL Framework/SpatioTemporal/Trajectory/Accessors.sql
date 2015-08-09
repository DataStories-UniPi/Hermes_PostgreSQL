/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the Trajectory data type.
 *
 * @see @ref Trajectory
 * @see @ref data_type_trajectory
 */

/** @brief This function returns the size of the trajectory in bytes.
 *
 *  @param[in] trajectory The trajectory
 *
 *	@return the size
 *
 *	@see @ref methods_size
 *
 */
CREATE FUNCTION size(trajectory Trajectory) RETURNS bigint AS
	'$libdir/Hermes','sizeTrajectory'
LANGUAGE C IMMUTABLE STRICT;

/** @brief This functions returns the number of points in a trajectory.
 *
 *  @param[in] trajectory The trajectory
 *
 *	@return the number of points
 *
 *	@see @ref methods_NrPoints
 *
 */
CREATE FUNCTION NrPoints(trajectory Trajectory) RETURNS integer AS
	'$libdir/Hermes','NrPointsTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief This function returns the number of segments in a trajectory.
 *
 *  @param[in] trajectory The trajectory
 *
 *	@return the number of segments
 *
 *	@see @ref methods_NrSegments
 *
 */
CREATE FUNCTION NrSegments(trajectory Trajectory) RETURNS integer AS
	'$libdir/Hermes','NrSegmentsTrajectory'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION samplingPeriod(trajectory Trajectory) RETURNS interval AS
	'$libdir/Hermes','samplingPeriodTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief This function returns the point at a specific position of the trajectory.
 *
 *  @param[in] trajectory The trajectory
 *  @param[in] i The position of the trajectory
 *
 *	@return the point
 *
 *	@see @ref methods_pointAt
 *
 */
CREATE FUNCTION pointAt(trajectory Trajectory, i integer) RETURNS PointST AS
	'$libdir/Hermes','pointAtTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief This function returns the segment at a specific position of the trajectory.
 *
 *  @param[in] trajectory The trajectory
 *  @param[in] i The position of the trajectory
 *
 *	@return the segment
 *
 *	@see @ref methods_segmentAt
 *
 */
CREATE FUNCTION segmentAt(trajectory Trajectory, i integer) RETURNS SegmentST AS
	'$libdir/Hermes','segmentAtTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief This function returns the first point of a trajectory.
 *
 *  @param[in] trajectory The trajectory
 *
 *	@return the point
 *
 *	@see @ref methods_firstPoint
 *
 */
CREATE FUNCTION firstPoint(trajectory Trajectory) RETURNS PointST AS
	'$libdir/Hermes','firstPointTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief This function returns the last point of a trajectory.
 *
 *  @param[in] trajectory The trajectory
 *
 *	@return the point
 *
 *	@see @ref methods_lastPoint
 *
 */
CREATE FUNCTION lastPoint(trajectory Trajectory) RETURNS PointST AS
	'$libdir/Hermes','lastPointTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief This function returns the first segment of a trajectory.
 *
 *  @param[in] trajectory The trajectory
 *
 *	@return the segment
 *
 *	@see @ref methods_firstSegment
 *
 */
CREATE FUNCTION firstSegment(trajectory Trajectory) RETURNS SegmentST AS
	'$libdir/Hermes','firstSegmentTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief This function returns the last segment of a trajectory.
 *
 *  @param[in] trajectory The trajectory
 *
 *	@return the segment
 *
 *	@see @ref methods_lastSegment
 *
 */
CREATE FUNCTION lastSegment(trajectory Trajectory) RETURNS SegmentST AS
	'$libdir/Hermes','lastSegmentTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief This function returns the sub-trajectory from a trajectory specified 
 * 	by the two integers.
 *
 *  @param[in] trajectory The trajectory
 * 	@param[in] s The start position of the trajectory
 * 	@param[in] e The end position of the trajectory
 *
 *	@return the sub-trajectory
 *
 *	@see @ref methods_sub
 *
 */
CREATE FUNCTION sub(trajectory Trajectory, s integer, e integer) RETURNS Trajectory AS
	'$libdir/Hermes','subTrajectoryV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION points(trajectory Trajectory)
RETURNS TABLE(p PointST) AS
	'$libdir/Hermes','pointsTrajectory'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION segments(trajectory Trajectory)
RETURNS TABLE(s SegmentST) AS
	'$libdir/Hermes','segmentsTrajectory'
LANGUAGE C IMMUTABLE STRICT;
