/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../h/Spatial/SessionParameters.h"

float8 spatialTolerance = 1; //1 metre

PGDLLEXPORT Datum getSpatialTolerance(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(spatialTolerance);
}
PG_FUNCTION_INFO_V1(getSpatialTolerance);

PGDLLEXPORT Datum setSpatialTolerance(PG_FUNCTION_ARGS) {
	float8 input = PG_GETARG_FLOAT8(0);

	if (input < 0)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("spatialTolerance cannot be negative")));

	spatialTolerance = input;

	PG_RETURN_VOID();
}
PG_FUNCTION_INFO_V1(setSpatialTolerance);

PGDLLEXPORT Datum setSpatialToleranceToCentimetre(PG_FUNCTION_ARGS) {
	spatialTolerance = 0.01;

	PG_RETURN_VOID();
}
PG_FUNCTION_INFO_V1(setSpatialToleranceToCentimetre);

PGDLLEXPORT Datum setSpatialToleranceToMetre(PG_FUNCTION_ARGS) {
	spatialTolerance = 1;

	PG_RETURN_VOID();
}
PG_FUNCTION_INFO_V1(setSpatialToleranceToMetre);

PGDLLEXPORT Datum setSpatialToleranceToKilometre(PG_FUNCTION_ARGS) {
	spatialTolerance = 1000;

	PG_RETURN_VOID();
}
PG_FUNCTION_INFO_V1(setSpatialToleranceToKilometre);

PGDLLEXPORT Datum setSpatialToleranceToFoot(PG_FUNCTION_ARGS) {
	spatialTolerance = 0.3048;

	PG_RETURN_VOID();
}
PG_FUNCTION_INFO_V1(setSpatialToleranceToFoot);
