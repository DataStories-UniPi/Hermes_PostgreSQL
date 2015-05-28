/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/SpatioTemporal/BoxST/Definition.h"
#include "../../h/Temporal/Period/Distance.h"
#include "../../h/Spatial/BoxSP/Distance.h"

PGDLLEXPORT Datum distanceBoxST_PeriodV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(distancePeriod_Period(&((BoxST *) PG_GETARG_POINTER(0))->t, (Period *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(distanceBoxST_PeriodV1);

PGDLLEXPORT Datum distanceBoxST_BoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(distanceBoxSP_BoxSP(&((BoxST *) PG_GETARG_POINTER(0))->sp, (BoxSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(distanceBoxST_BoxSPV1);
