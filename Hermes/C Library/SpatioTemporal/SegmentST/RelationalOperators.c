/**
 *	@file
 *	@author Marios Vodas (mvodas@gmail.com).
 *	@brief The implementation of SegmentST
 *
 *	@details These functions are documented in the @ref SegmentST/RelationalOperators.h
 *
 */

#include "../../h/SpatioTemporal/SegmentST/RelationalOperators.h"
#include "../../h/SpatioTemporal/SegmentST/Interpolation.h"
#include "../../h/Temporal/Period/RelationalOperators.h"
#include "../../h/Temporal/RangeT/Casts.h"
#include "../../h/Spatial/SessionParameters.h"
#include "../../h/Spatial/SegmentSP/RelationalOperators.h"
#include "../../h/Spatial/PointSP/Distance.h"

bool intersectsSegmentST_BoxST(const SegmentST *segment, const BoxST *box) {
	if (atBoxSTSegmentST(segment, box, NULL, NULL ))
		return true;

	return false;
}

bool intersectsSegmentST_SegmentST(const SegmentST *segmentA, const SegmentST *segmentB) {
	PointST point;

	if (intersectionPointSegmentST_SegmentST(segmentA, segmentB, &point))
		return true;

	return false;
}

PGDLLEXPORT Datum intersectsSegmentST_BoxSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(intersectsSegmentST_BoxST((SegmentST *) PG_GETARG_POINTER(0), (BoxST *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(intersectsSegmentST_BoxSTV1);

PGDLLEXPORT Datum intersectsSegmentST_SegmentSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(intersectsSegmentST_SegmentST((SegmentST *) PG_GETARG_POINTER(0), (SegmentST *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(intersectsSegmentST_SegmentSTV1);

bool containsSegmentST_PointST(const SegmentST *segment, const PointST *point) {
	PointSP pointAtInstant;

	if (atInstantSPSegmentST(segment, point->t, true, &pointAtInstant))
		if (distancePointSP_PointSP(&pointAtInstant, &point->sp) < spatialTolerance)
			return true;

	return false;
}

PGDLLEXPORT Datum containsSegmentST_PointSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(containsSegmentST_PointST((SegmentST *) PG_GETARG_POINTER(0), (PointST *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(containsSegmentST_PointSTV1);

bool containedSegmentST_BoxST(const SegmentST *segment, const BoxST *box) {
	return containsPeriod_Period(&box->t, &segment->t) && containedSegmentSP_BoxSP(&segment->sp, &box->sp);
}

bool containedProperlySegmentST_BoxST(const SegmentST *segment, const BoxST *box) {
	return containsProperlyPeriod_Period(&box->t, &segment->t) && containedProperlySegmentSP_BoxSP(&segment->sp, &box->sp);
}

PGDLLEXPORT Datum containedSegmentST_BoxSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(containedSegmentST_BoxST((SegmentST *) PG_GETARG_POINTER(0), (BoxST *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(containedSegmentST_BoxSTV1);

PGDLLEXPORT Datum containedProperlySegmentST_BoxSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(containedProperlySegmentST_BoxST((SegmentST *) PG_GETARG_POINTER(0), (BoxST *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(containedProperlySegmentST_BoxSTV1);

bool withinDistanceSegmentST_RangeST(const SegmentST *segment, const RangeST *range) {
	Period period;

	int32 nr_intersection_points;
	SegmentSP segmentAtPeriod;
	PointSP pointAtPeriod;

	RangeT2Period(&range->t, &period);

	nr_intersection_points = atPeriodSPSegmentST(segment, &period, &segmentAtPeriod, &pointAtPeriod);

	if (nr_intersection_points == 1) {
		if (distancePointSP_PointSP(&pointAtPeriod, &range->sp.c) <= range->sp.r)
			return true;
	} else if (nr_intersection_points == 2) {
		if (withinDistanceSegmentSP_RangeSP(&segmentAtPeriod, &range->sp))
			return true;
	}

	return false;
}

PGDLLEXPORT Datum withinDistanceSegmentST_RangeSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(withinDistanceSegmentST_RangeST((SegmentST *) PG_GETARG_POINTER(0), (RangeST *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(withinDistanceSegmentST_RangeSTV1);
