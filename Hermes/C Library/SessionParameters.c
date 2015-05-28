/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "h/SessionParameters.h"

float8 zeroTolerance = 0.000001;

PGDLLEXPORT Datum getZeroTolerance(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(zeroTolerance);
}
PG_FUNCTION_INFO_V1(getZeroTolerance);

PGDLLEXPORT Datum setZeroTolerance(PG_FUNCTION_ARGS) {
	float8 input = PG_GETARG_FLOAT8(0);

	if (input < 0)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("zeroTolerance cannot be negative")));

	zeroTolerance = input;

	PG_RETURN_VOID();
}
PG_FUNCTION_INFO_V1(setZeroTolerance);

PGDLLEXPORT Datum setDefaultZeroTolerance(PG_FUNCTION_ARGS) {
	zeroTolerance = 0.000001;

	PG_RETURN_VOID();
}
PG_FUNCTION_INFO_V1(setDefaultZeroTolerance);

int32 verbosityLevel = 0;

PGDLLEXPORT Datum isVerbose(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(verbosityLevel > 0);
}
PG_FUNCTION_INFO_V1(isVerbose);

PGDLLEXPORT Datum toggleVerbose(PG_FUNCTION_ARGS) {
	verbosityLevel = !verbosityLevel;

	PG_RETURN_VOID();
}
PG_FUNCTION_INFO_V1(toggleVerbose);

PGDLLEXPORT Datum getVerbosityLevel(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(verbosityLevel);
}
PG_FUNCTION_INFO_V1(getVerbosityLevel);

PGDLLEXPORT Datum setVerbosityLevel(PG_FUNCTION_ARGS) {
	verbosityLevel = PG_GETARG_INT32(0);

	PG_RETURN_VOID();
}
PG_FUNCTION_INFO_V1(setVerbosityLevel);

int32 currentDatasetID = 0;

PGDLLEXPORT Datum getCurrentDatasetID(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(currentDatasetID);
}
PG_FUNCTION_INFO_V1(getCurrentDatasetID);

PGDLLEXPORT Datum setCurrentDatasetID(PG_FUNCTION_ARGS) {
	currentDatasetID = PG_GETARG_INT32(0);

	PG_RETURN_VOID();
}
PG_FUNCTION_INFO_V1(setCurrentDatasetID);
