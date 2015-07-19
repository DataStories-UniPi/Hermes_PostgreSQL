/**
 *	@file
 *	@author Marios Vodas (mvodas@gmail.com).
 *	@brief The implementantion of SegmentSP
 *
 *	@details These functions are documented in the @ref SegmentSP/RelationalOperators.h
 *
 */

#include "../../h/Spatial/SegmentSP/RelationalOperators.h"
#include "../../h/Spatial/SegmentSP/Interactions.h"
#include "../../h/Spatial/SegmentSP/Distance.h"
#include "../../h/Spatial/SessionParameters.h"

bool intersectsSegmentSP_BoxSP(const SegmentSP *segment, const BoxSP *box) {
	return intersectionSegmentSP_BoxSP(segment, box, true, NULL, NULL) ? true : false;
}

bool intersectsSegmentSP_SegmentSP(const SegmentSP *segmentA, const SegmentSP *segmentB) {
	return intersectionSegmentSP_SegmentSP(segmentA, segmentB, NULL, NULL ) ? true : false;
}

PGDLLEXPORT Datum intersectsSegmentSP_BoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(intersectsSegmentSP_BoxSP((SegmentSP *) PG_GETARG_POINTER(0), (BoxSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(intersectsSegmentSP_BoxSPV1);

PGDLLEXPORT Datum intersectsSegmentSP_SegmentSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(intersectsSegmentSP_SegmentSP((SegmentSP *) PG_GETARG_POINTER(0), (SegmentSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(intersectsSegmentSP_SegmentSPV1);

bool containsSegmentSP_PointSP(const SegmentSP *segment, const PointSP *point) {
	if (distanceSegmentSP_PointSP(segment, point) < spatialTolerance)
		return true;

	return false;

	/*
	 * Alternative implementation that could be used as an example for other functions that might use fxLineSP or fyLineSP.
	 float8 fx;

	 if (!isnan(fx = fxLineSP(&tmpLine, point->x)) && (isinf(fx) || fabs(fx - point->y) < spatialTolerance)) {
	 return true;
	 } else {
	 return false;
	 }
	 */
}

PGDLLEXPORT Datum containsSegmentSP_PointSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(containsSegmentSP_PointSP((SegmentSP *) PG_GETARG_POINTER(0), (PointSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(containsSegmentSP_PointSPV1);

bool containedSegmentSP_BoxSP(const SegmentSP *segment, const BoxSP *box) {
	return box->l.x <= Min(segment->i.x, segment->e.x) && Max(segment->i.x, segment->e.x) <= box->h.x && box->l.y <= Min(segment->i.y, segment->e.y) && Max(segment->i.y, segment->e.y) <= box->h.y;
}

bool containedProperlySegmentSP_BoxSP(const SegmentSP *segment, const BoxSP *box) {
	return box->l.x < Min(segment->i.x, segment->e.x) && Max(segment->i.x, segment->e.x) < box->h.x && box->l.y < Min(segment->i.y, segment->e.y) && Max(segment->i.y, segment->e.y) < box->h.y;
}

PGDLLEXPORT Datum containedProperlySegmentSP_BoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(containedProperlySegmentSP_BoxSP((SegmentSP *) PG_GETARG_POINTER(0), (BoxSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(containedProperlySegmentSP_BoxSPV1);

PGDLLEXPORT Datum containedSegmentSP_BoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(containedSegmentSP_BoxSP((SegmentSP *) PG_GETARG_POINTER(0), (BoxSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(containedSegmentSP_BoxSPV1);

bool withinDistanceSegmentSP_RangeSP(const SegmentSP *segment, const RangeSP *range) {
	return distanceSegmentSP_PointSP(segment, &range->c) <= range->r ? true : false;
}

PGDLLEXPORT Datum withinDistanceSegmentSP_RangeSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(withinDistanceSegmentSP_RangeSP((SegmentSP *) PG_GETARG_POINTER(0), (RangeSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(withinDistanceSegmentSP_RangeSPV1);
