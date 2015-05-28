/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../h/Temporal/SessionParameters.h"
#include "../h/Utilities/Utilities.h"

float8 temporalTolerance = 1; //1 second

PGDLLEXPORT Datum getTemporalTolerance(PG_FUNCTION_ARGS) {
	PG_RETURN_INTERVAL_P(seconds2interval(temporalTolerance, NULL));
}
PG_FUNCTION_INFO_V1(getTemporalTolerance);

PGDLLEXPORT Datum setTemporalTolerance(PG_FUNCTION_ARGS) {
	float8 input = interval2seconds(PG_GETARG_INTERVAL_P(0));

	if (input < 0)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("temporalTolerance cannot be negative")));

	temporalTolerance = input;

	PG_RETURN_VOID();
}
PG_FUNCTION_INFO_V1(setTemporalTolerance);

PGDLLEXPORT Datum setTemporalToleranceToSecond(PG_FUNCTION_ARGS) {
	temporalTolerance = 1;

	PG_RETURN_VOID();
}
PG_FUNCTION_INFO_V1(setTemporalToleranceToSecond);
