/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/Spatial/RangeSP/Properties.h"
#include "../../h/Utilities/Utilities.h"
#include <math.h>

float8 areaRangeSP(const RangeSP *range) {
	return dPi() * pow(range->r, 2.0);
}

PGDLLEXPORT Datum areaRangeSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(areaRangeSP((RangeSP *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(areaRangeSPV1);
