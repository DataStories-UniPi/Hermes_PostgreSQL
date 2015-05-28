/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/Spatial/LineSP/ArithmeticOperators.h"
#include "../../h/Utilities/MSVCWorkarounds.h"
#include <math.h>

float8 fxLineSP(const LineSP *line, float8 x) {
	if (line->B == 0.0) {
		if (x == -line->C / line->A) {
			return INFINITY;
		} else {
			return NAN;
		}
	}

	return (-line->A / line->B) * x - line->C / line->B;
}

float8 fyLineSP(const LineSP *line, float8 y) {
	if (line->A == 0.0) {
		if (y == -line->C / line->B) {
			return INFINITY;
		} else {
			return NAN;
		}
	}

	return (-line->B / line->A) * y - line->C / line->A;
}

PGDLLEXPORT Datum fxLineSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(fxLineSP((LineSP *) PG_GETARG_POINTER(0), PG_GETARG_FLOAT8(1)));
}
PG_FUNCTION_INFO_V1(fxLineSPV1);

PGDLLEXPORT Datum fyLineSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(fyLineSP((LineSP *) PG_GETARG_POINTER(0), PG_GETARG_FLOAT8(1)));
}
PG_FUNCTION_INFO_V1(fyLineSPV1);
