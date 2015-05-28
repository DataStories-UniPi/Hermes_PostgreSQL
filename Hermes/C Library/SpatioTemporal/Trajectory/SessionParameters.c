/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/SpatioTemporal/Trajectory/SessionParameters.h"

int32 interpolationPrecision = 0;

int32 validateInterpolationPrecision(int32 interpolationPrecision) {
	return interpolationPrecision < 0 ? -2 : interpolationPrecision;
}

PGDLLEXPORT Datum getInterpolationPrecision(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(interpolationPrecision);
}
PG_FUNCTION_INFO_V1(getInterpolationPrecision);

PGDLLEXPORT Datum setInterpolationPrecision(PG_FUNCTION_ARGS) {
	interpolationPrecision = validateInterpolationPrecision(PG_GETARG_INT32(0));

	PG_RETURN_VOID();
}
PG_FUNCTION_INFO_V1(setInterpolationPrecision);
