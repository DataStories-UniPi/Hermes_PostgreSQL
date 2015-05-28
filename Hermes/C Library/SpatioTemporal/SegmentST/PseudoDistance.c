/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/SpatioTemporal/SegmentST/Definition.h"
#include "../../h/Temporal/Period/Distance.h"
#include "../../h/Spatial/SegmentSP/Distance.h"
#include "../../h/Spatial/BoxSP/Distance.h"

PGDLLEXPORT Datum distanceSegmentST_TimestampV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(distancePeriod_Timestamp(&((SegmentST *) PG_GETARG_POINTER(0))->t, PG_GETARG_TIMESTAMP(1)));
}
PG_FUNCTION_INFO_V1(distanceSegmentST_TimestampV1);

PGDLLEXPORT Datum distanceSegmentST_PeriodV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(distancePeriod_Period(&((SegmentST *) PG_GETARG_POINTER(0))->t, (Period *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(distanceSegmentST_PeriodV1);

PGDLLEXPORT Datum distanceSegmentST_PointSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(distanceSegmentSP_PointSP(&((SegmentST *) PG_GETARG_POINTER(0))->sp, (PointSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(distanceSegmentST_PointSPV1);

PGDLLEXPORT Datum distanceSegmentST_SegmentSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(distanceSegmentSP_SegmentSP(&((SegmentST *) PG_GETARG_POINTER(0))->sp, (SegmentSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(distanceSegmentST_SegmentSPV1);

PGDLLEXPORT Datum distanceSegmentST_BoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(distanceBoxSP_SegmentSP((BoxSP *) PG_GETARG_POINTER(1), &((SegmentST *) PG_GETARG_POINTER(0))->sp));
}
PG_FUNCTION_INFO_V1(distanceSegmentST_BoxSPV1);
