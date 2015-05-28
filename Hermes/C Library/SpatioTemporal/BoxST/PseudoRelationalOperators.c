/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/SpatioTemporal/BoxST/Definition.h"
#include "../../h/Temporal/Period/RelationalOperators.h"
#include "../../h/Spatial/BoxSP/RelationalOperators.h"

PGDLLEXPORT Datum intersectsBoxST_PeriodV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(intersectsPeriod_Period(&((BoxST *) PG_GETARG_POINTER(0))->t, (Period *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(intersectsBoxST_PeriodV1);

PGDLLEXPORT Datum intersectsBoxST_BoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(intersectsBoxSP_BoxSP(&((BoxST *) PG_GETARG_POINTER(0))->sp, (BoxSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(intersectsBoxST_BoxSPV1);
