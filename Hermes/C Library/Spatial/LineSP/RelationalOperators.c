/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/Spatial/LineSP/RelationalOperators.h"
#include "../../h/Spatial/LineSP/Properties.h"

bool isParallelLineSP_LineSP(const LineSP *lineA, const LineSP *lineB) {
	if ((lineA->A == lineB->A) && (lineA->A == 0.0)) {
		return true ;
	}

	if ((lineA->B == lineB->B) && (lineA->B == 0.0)) {
		return true ;
	}

	if (lineA->A == 0.0 || lineB->A == 0.0 || lineA->B == 0.0 || lineB->B == 0.0) {
		return false ;
	}

	if (slopeLineSP(lineA) == slopeLineSP(lineB)) {
		return true ;
	}

	return false ;
}

PGDLLEXPORT Datum isParallelLineSP_LineSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(isParallelLineSP_LineSP((LineSP *) PG_GETARG_POINTER(0), (LineSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(isParallelLineSP_LineSPV1);
