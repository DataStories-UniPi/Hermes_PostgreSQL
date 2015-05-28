/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/Clustering/Traclus/Distance.h"
#include "../../h/Spatial/SegmentSP/Properties.h"
#include "../../h/Spatial/SegmentSP/Casts.h"
#include "../../h/Spatial/LineSP/Properties.h"
#include "../../h/Spatial/PointSP/Distance.h"
#include "../../h/Utilities/Utilities.h"
#include "../../h/Utilities/MSVCWorkarounds.h"
#include <math.h>

PointSP *projectionPointTraclus(const PointSP *point, const SegmentSP *segment, PointSP *result) {
	float8 u;
	PointSP *ret = result ? result : (PointSP *) palloc(sizeof(*ret));

	if (segment->i.x == segment->e.x && segment->i.y == segment->e.y) {
		ret->x = segment->i.x;
		ret->y = segment->i.y;
	} else {
		u = ((segment->i.y - point->y) * (segment->i.y - segment->e.y) - (segment->i.x - point->x) * (segment->e.x - segment->i.x)) / pow(sqrt(pow(segment->e.x - segment->i.x, 2.0) + pow(segment->e.y - segment->i.y, 2.0)), 2.0);

		ret->x = (int32) round(segment->i.x + u * (segment->e.x - segment->i.x));
		ret->y = (int32) round(segment->i.y + u * (segment->e.y - segment->i.y));
	}

	return ret;
}

PGDLLEXPORT Datum projectionPointTraclusV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(projectionPointTraclus((PointSP *) PG_GETARG_POINTER(0), (SegmentSP *) PG_GETARG_POINTER(1), NULL));
}
PG_FUNCTION_INFO_V1(projectionPointTraclusV1);

float8 perpendicularDistanceTraclus(const SegmentSP *segmentA, const SegmentSP *segmentB) {
	float8 L1;
	float8 L2;
	PointSP proj_point;

	projectionPointTraclus(&segmentB->i, segmentA, &proj_point);
	L1 = distancePointSP_PointSP(&proj_point, &segmentB->i);

	projectionPointTraclus(&segmentB->e, segmentA, &proj_point);
	L2 = distancePointSP_PointSP(&proj_point, &segmentB->e);

	if (L1 + L2 == 0.0)
		return 0.0;

	return (pow(L1, 2) + pow(L2, 2)) / (L1 + L2);
}

float8 parallelDistanceTraclus(const SegmentSP *segmentA, const SegmentSP *segmentB) {
	float8 L1;
	float8 L2;
	PointSP proj_point;

	projectionPointTraclus(&segmentB->i, segmentA, &proj_point);
	L1 = Min(distancePointSP_PointSP(&proj_point, &segmentA->i), distancePointSP_PointSP(&proj_point, &segmentA->e));

	projectionPointTraclus(&segmentB->e, segmentA, &proj_point);
	L2 = Min(distancePointSP_PointSP(&proj_point, &segmentA->i), distancePointSP_PointSP(&proj_point, &segmentA->e));

	return Min(L1, L2);
}

float8 angleDistanceTraclus(const SegmentSP *segmentA, const SegmentSP *segmentB) {
	float8 ang;
	float8 angdeg;
	LineSP lineA, lineB;

	ang = fabs(angleXXLineSP(SegmentSP2LineSP(segmentA, &lineA)) - angleXXLineSP(SegmentSP2LineSP(segmentB, &lineB)));
	angdeg = radians2degrees(ang);

	if (angdeg >= 0 && angdeg < 90) {
		return lengthSegmentSP(segmentB) * sin(ang);
	} else {
		return lengthSegmentSP(segmentB);
	}
}

float8 traclusDistance(const SegmentSP *segmentA, const SegmentSP *segmentB, float4 w_perpendicular, float4 w_parallel, float4 w_angle) {
	return ((float8) w_perpendicular) * perpendicularDistanceTraclus(segmentA, segmentB) + ((float8) w_parallel) * parallelDistanceTraclus(segmentA, segmentB) + ((float8) w_angle) * angleDistanceTraclus(segmentA, segmentB);
}

PGDLLEXPORT Datum perpendicularDistanceTraclusV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(perpendicularDistanceTraclus((SegmentSP *) PG_GETARG_POINTER(0), (SegmentSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(perpendicularDistanceTraclusV1);

PGDLLEXPORT Datum parallelDistanceTraclusV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(parallelDistanceTraclus((SegmentSP *) PG_GETARG_POINTER(0), (SegmentSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(parallelDistanceTraclusV1);

PGDLLEXPORT Datum angleDistanceTraclusV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(angleDistanceTraclus((SegmentSP *) PG_GETARG_POINTER(0), (SegmentSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(angleDistanceTraclusV1);

PGDLLEXPORT Datum traclusDistanceV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(traclusDistance((SegmentSP *) PG_GETARG_POINTER(0), (SegmentSP *) PG_GETARG_POINTER(1), PG_GETARG_FLOAT4(2), PG_GETARG_FLOAT4(3), PG_GETARG_FLOAT4(4)));
}
PG_FUNCTION_INFO_V1(traclusDistanceV1);
