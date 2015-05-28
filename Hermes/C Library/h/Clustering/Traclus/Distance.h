/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_TRACLUS_DISTANCE_H_
#define HE_TRACLUS_DISTANCE_H_

#include "../../Spatial/SegmentSP/Definition.h"

/*
 * returns the point of perpendicular projection of point onto line
 */
PointSP *projectionPointTraclus(const PointSP *point, const SegmentSP *segment, PointSP *result);
PGDLLEXPORT Datum projectionPointTraclusV1(PG_FUNCTION_ARGS);

float8 perpendicularDistanceTraclus(const SegmentSP *segmentA, const SegmentSP *segmentB);
float8 parallelDistanceTraclus(const SegmentSP *segmentA, const SegmentSP *segmentB);
float8 angleDistanceTraclus(const SegmentSP *segmentA, const SegmentSP *segmentB);
float8 traclusDistance(const SegmentSP *segmentA, const SegmentSP *segmentB, float4 w_perpendicular, float4 w_parallel, float4 w_angle);
PGDLLEXPORT Datum perpendicularDistanceTraclusV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum parallelDistanceTraclusV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum angleDistanceTraclusV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum traclusDistanceV1(PG_FUNCTION_ARGS);

#endif
