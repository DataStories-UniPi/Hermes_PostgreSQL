/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief The implementantion in sql of TRACLUS algorithm.
 *
 * @details TRACLUS (TRAjectory CLUStering) is an algorithm introduced
 * by @cite lee2007trajectory for the trejectoring clustering of
 * similiar trajectories. For more detailed descritpion look at
 * @ref traclus
 *
 * @see @ref traclus
 * @see http://hanj.cs.illinois.edu/pdf/sigmod07_jglee.pdf
 *
 */

/** @brief The function returns the perpendicular projection
 * 	of point onto a line and is need it by the TRACLUS algorithm.
 *  It`s definition is given by @cite lee2007trajectory
 *
 *	@param[in] point the point for which we are searching the perpendicular distance
 *	@param[in] segment the segment from which we are searching the perpendicular distance
 *
 *  @return the point of perpendicular projection of point onto line
 */
CREATE FUNCTION projectionPointTraclus(point PointSP, segment SegmentSP) RETURNS PointSP AS
	'$libdir/Hermes','projectionPointTraclusV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the perpendicular distance between
 *  two segments and is need it by the TRACLUS algorithm.
 *  It`s definition is given by @cite lee2007trajectory
 * 
 *  @param[in] segmentA The first segment
 *	@param[in] segmentB The second segment
 *
 *  @return perpendicular distance
 */
CREATE FUNCTION perpendicularDistanceTraclus(segmentA SegmentSP, segmentB SegmentSP) RETURNS double precision AS
	'$libdir/Hermes','perpendicularDistanceTraclusV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the parallel distance between
 *  two segments and is need it by the TRACLUS algorithm.
 *  It`s definition is given by @cite lee2007trajectory
 * 
 * 	@param[in] segmentA The first segment
 *	@param[in] segmentB The second segment
 *
 *  @return parallel distance
 */
CREATE FUNCTION parallelDistanceTraclus(segmentA SegmentSP, segmentB SegmentSP) RETURNS double precision AS
	'$libdir/Hermes','parallelDistanceTraclusV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the anle distance between
 *  two segments and is need it by the TRACLUS algorithm.
 *  It`s definition is given by @cite lee2007trajectory
 *
 *	@param[in] segmentA The first segment
 *	@param[in] segmentB The second segment
 *
 *  @return angle distance
 */
CREATE FUNCTION angleDistanceTraclus(segmentA SegmentSP, segmentB SegmentSP) RETURNS double precision AS
	'$libdir/Hermes','angleDistanceTraclusV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the TRACLUS clustering.
 *  It`s definition is given by @cite lee2007trajectory
 * 
 * 	@param[in] segmentA The first segment
 *	@param[in] segmentB The second segment
 *	@param[in] w_perpendicular I have no idea. Ask marios
 *	@param[in] w_parallel I have no idea
 *	@param[in] w_angle I have no idea
 *
 *  @return TRACLUS clustering
 */
CREATE FUNCTION traclusDistance(segmentA SegmentSP, segmentB SegmentSP, w_perpendicular real DEFAULT 1, w_parallel real DEFAULT 1, w_angle real DEFAULT 1) RETURNS double precision AS
	'$libdir/Hermes','traclusDistanceV1'
LANGUAGE C IMMUTABLE STRICT;
