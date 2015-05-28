/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/Spatial/LineSP/Properties.h"
#include "../../h/Utilities/MSVCWorkarounds.h"
#include <math.h>

float8 angleXXLineSP(const LineSP *line) {
	return atan2(-line->A, line->B);
}

PGDLLEXPORT Datum angleXXLineSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(angleXXLineSP((LineSP *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(angleXXLineSPV1);

float8 slopeLineSP(const LineSP *line) {
	if (line->B == 0.0) {
		return NAN;
	}

	return -line->A / line->B;
}

PGDLLEXPORT Datum slopeLineSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(slopeLineSP((LineSP *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(slopeLineSPV1);
