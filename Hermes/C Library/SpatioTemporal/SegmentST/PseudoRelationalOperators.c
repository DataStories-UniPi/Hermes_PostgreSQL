/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/SpatioTemporal/SegmentST/Definition.h"
#include "../../h/Temporal/Period/RelationalOperators.h"
#include "../../h/Spatial/SegmentSP/RelationalOperators.h"
#include "../../h/Spatial/SegmentSP/Casts.h"
#include "../../h/Spatial/BoxSP/RelationalOperators.h"

PGDLLEXPORT Datum intersectsSegmentST_PeriodV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(intersectsPeriod_Period(&((SegmentST *) PG_GETARG_POINTER(0))->t, (Period *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(intersectsSegmentST_PeriodV1);

PGDLLEXPORT Datum intersectsSegmentST_BoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(intersectsSegmentSP_BoxSP(&((SegmentST *) PG_GETARG_POINTER(0))->sp, (BoxSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(intersectsSegmentST_BoxSPV1);

PGDLLEXPORT Datum intersectsSegmentST_SegmentSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(intersectsSegmentSP_SegmentSP(&((SegmentST *) PG_GETARG_POINTER(0))->sp, (SegmentSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(intersectsSegmentST_SegmentSPV1);

PGDLLEXPORT Datum containsSegmentST_TimestampV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(containsPeriod_Timestamp(&((SegmentST *) PG_GETARG_POINTER(0))->t, PG_GETARG_TIMESTAMP(1)));
}
PG_FUNCTION_INFO_V1(containsSegmentST_TimestampV1);

PGDLLEXPORT Datum containsSegmentST_PeriodV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(containsPeriod_Period(&((SegmentST *) PG_GETARG_POINTER(0))->t, (Period *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(containsSegmentST_PeriodV1);

PGDLLEXPORT Datum containsSegmentST_PointSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(containsSegmentSP_PointSP(&((SegmentST *) PG_GETARG_POINTER(0))->sp, (PointSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(containsSegmentST_PointSPV1);

PGDLLEXPORT Datum containedSegmentST_PeriodV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(containsPeriod_Period((Period *) PG_GETARG_POINTER(1), &((SegmentST *) PG_GETARG_POINTER(0))->t));
}
PG_FUNCTION_INFO_V1(containedSegmentST_PeriodV1);

PGDLLEXPORT Datum containedSegmentST_BoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(containedSegmentSP_BoxSP(&((SegmentST *) PG_GETARG_POINTER(0) )->sp, (BoxSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(containedSegmentST_BoxSPV1);

PGDLLEXPORT Datum withinDistanceSegmentST_RangeSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(withinDistanceSegmentSP_RangeSP(&((SegmentST *) PG_GETARG_POINTER(0) )->sp, (RangeSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(withinDistanceSegmentST_RangeSPV1);
