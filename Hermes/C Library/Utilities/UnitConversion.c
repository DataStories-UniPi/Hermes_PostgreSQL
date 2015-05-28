/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../h/Utilities/UnitConversion.h"

float8 km2metres(float8 km) {
	return km * 1000;
}

float8 metres2km(float8 metres) {
	return metres / 1000;
}

PGDLLEXPORT Datum km2metresV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(km2metres(PG_GETARG_FLOAT8(0)));
}
PG_FUNCTION_INFO_V1(km2metresV1);

PGDLLEXPORT Datum metres2kmV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(metres2km(PG_GETARG_FLOAT8(0)));
}
PG_FUNCTION_INFO_V1(metres2kmV1);

float8 nm2metres(float8 nm) {
	return nm * 1852;
}

float8 metres2nm(float8 metres) {
	return metres / 1852;
}

PGDLLEXPORT Datum nm2metresV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(nm2metres(PG_GETARG_FLOAT8(0)));
}
PG_FUNCTION_INFO_V1(nm2metresV1);

PGDLLEXPORT Datum metres2nmV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(metres2nm(PG_GETARG_FLOAT8(0)));
}
PG_FUNCTION_INFO_V1(metres2nmV1);

float8 kmph2mps(float8 kmph) {
	return kmph * 0.277777778;
}

float8 mps2kmph(float8 mps) {
	return mps / 0.277777778;
}

PGDLLEXPORT Datum kmph2mpsV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(kmph2mps(PG_GETARG_FLOAT8(0)));
}
PG_FUNCTION_INFO_V1(kmph2mpsV1);

PGDLLEXPORT Datum mps2kmphV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(mps2kmph(PG_GETARG_FLOAT8(0)));
}
PG_FUNCTION_INFO_V1(mps2kmphV1);

float8 knots2mps(float8 knots) {
	return knots * 0.514444444;
}

float8 mps2knots(float8 mps) {
	return mps / 0.514444444;
}

PGDLLEXPORT Datum knots2mpsV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(knots2mps(PG_GETARG_FLOAT8(0)));
}
PG_FUNCTION_INFO_V1(knots2mpsV1);

PGDLLEXPORT Datum mps2knotsV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(mps2knots(PG_GETARG_FLOAT8(0)));
}
PG_FUNCTION_INFO_V1(mps2knotsV1);
