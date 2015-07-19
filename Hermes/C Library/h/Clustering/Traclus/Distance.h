/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief The implementantion of TRACLUS algorithm.
 *
 * @details TRACLUS (TRAjectory CLUStering) is an algorithm introduced
 * by @cite lee2007trajectory for the trejectoring clustering of
 * similiar trajectories. For more detailed descritpion look at
 * @ref traclus
 *
 * @see @ref traclus
 * @see http://hanj.cs.illinois.edu/pdf/sigmod07_jglee.pdf
 * @see Clustering/Traclus/Distance.sql
 *
 */

#ifndef HE_TRACLUS_DISTANCE_H_
#define HE_TRACLUS_DISTANCE_H_

#include "../../Spatial/SegmentSP/Definition.h"

/** @brief The function returns the perpendicular projection
 * 	of point onto a line and is need it by the TRACLUS algorithm.
 *  It`s definition is given by @cite lee2007trajectory
 *
 *  @return the point of perpendicular projection of point onto line
 */
PointSP *projectionPointTraclus(const PointSP *point, const SegmentSP *segment, PointSP *result);
PGDLLEXPORT Datum projectionPointTraclusV1(PG_FUNCTION_ARGS);

/** @brief The function returns the perpendicular distance between
 *  two segments and is need it by the TRACLUS algorithm.
 *  It`s definition is given by @cite lee2007trajectory
 *
 *  @return perpendicular distance
 */
float8 perpendicularDistanceTraclus(const SegmentSP *segmentA, const SegmentSP *segmentB);

/** @brief The function returns the parallel distance between
 *  two segments and is need it by the TRACLUS algorithm.
 *  It`s definition is given by @cite lee2007trajectory
 *
 *  @return parallel distance
 */
float8 parallelDistanceTraclus(const SegmentSP *segmentA, const SegmentSP *segmentB);

/** @brief The function returns the anle distance between
 *  two segments and is need it by the TRACLUS algorithm.
 *  It`s definition is given by @cite lee2007trajectory
 *
 *  @return angle distance
 */
float8 angleDistanceTraclus(const SegmentSP *segmentA, const SegmentSP *segmentB);

/** @brief The function returns the TRACLUS clustering.
 *  It`s definition is given by @cite lee2007trajectory
 *
 *  @return TRACLUS clustering
 */
float8 traclusDistance(const SegmentSP *segmentA, const SegmentSP *segmentB, float4 w_perpendicular, float4 w_parallel, float4 w_angle);
PGDLLEXPORT Datum perpendicularDistanceTraclusV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum parallelDistanceTraclusV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum angleDistanceTraclusV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum traclusDistanceV1(PG_FUNCTION_ARGS);

#endif
