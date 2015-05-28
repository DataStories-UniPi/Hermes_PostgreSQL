/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../../h/Spatial/CRS/PointXYZ/Definition.h"
#include "../../../h/Utilities/Utilities.h"
#include <libpq/pqformat.h>
#include <access/hash.h>

PointXYZ *clonePointXYZ(const PointXYZ *point, PointXYZ *result) {
	PointXYZ *ret = result ? result : (PointXYZ *) palloc(sizeof(*ret));

	*ret = *point;

	return ret;
}

PGDLLEXPORT Datum clonePointXYZV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(clonePointXYZ((PointXYZ *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(clonePointXYZV1);

PointXYZ *constructorPointXYZ(float8 x, float8 y, float8 z, PointXYZ *result) {
	PointXYZ *ret = result ? result : (PointXYZ *) palloc(sizeof(*ret));

	ret->x = x;
	ret->y = y;
	ret->z = z;

	return ret;
}

PGDLLEXPORT Datum constructorPointXYZV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(constructorPointXYZ(PG_GETARG_FLOAT8(0), PG_GETARG_FLOAT8(1), PG_GETARG_FLOAT8(2), NULL));
}
PG_FUNCTION_INFO_V1(constructorPointXYZV1);

PointXYZ *inPointXYZ(const char *str, PointXYZ *result, char **error) {
	float8 x, y, z;
	PointXYZ *ret = NULL;

	if (error)
		*error = NULL;

	if (sscanf(str, " %lf %lf %lf ", &x, &y, &z) != 3) {
		if (error)
			asprintfHermes(error, "invalid input syntax for PointXYZ: \"%s\"", str);
		return NULL ;
	}

	ret = result ? result : (PointXYZ *) palloc(sizeof(*ret));

	ret->x = x;
	ret->y = y;
	ret->z = z;

	return ret;
}

PGDLLEXPORT Datum inPointXYZV1(PG_FUNCTION_ARGS) {
	char *error;
	PointXYZ *result = inPointXYZ(PG_GETARG_CSTRING(0), NULL, &error);

	if (error)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("%s", error)));

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(inPointXYZV1);

char *outPointXYZ(const PointXYZ *point) {
	char *result = NULL;

	asprintfHermes(&result, "%f %f %f", point->x, point->y, point->z);

	return result;
}

PGDLLEXPORT Datum outPointXYZV1(PG_FUNCTION_ARGS) {
	PG_RETURN_CSTRING(outPointXYZ((PointXYZ *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(outPointXYZV1);

PGDLLEXPORT Datum recvPointXYZ(PG_FUNCTION_ARGS) {
	StringInfo msg = (StringInfo) PG_GETARG_POINTER(0);
	PointXYZ *result = (PointXYZ *) palloc(sizeof(*result));

	result->x = pq_getmsgfloat8(msg);
	result->y = pq_getmsgfloat8(msg);
	result->z = pq_getmsgfloat8(msg);

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(recvPointXYZ);

bytea *PointXYZ2Bytea(const PointXYZ *point) {
	StringInfoData buf;

	pq_begintypsend(&buf);

	pq_sendfloat8(&buf, ELIMINATE_MINUS_ZERO(point->x));
	pq_sendfloat8(&buf, ELIMINATE_MINUS_ZERO(point->y));
	pq_sendfloat8(&buf, ELIMINATE_MINUS_ZERO(point->z));

	return pq_endtypsend(&buf);
}

PGDLLEXPORT Datum sendPointXYZ(PG_FUNCTION_ARGS) {
	PG_RETURN_BYTEA_P(PointXYZ2Bytea((PointXYZ *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(sendPointXYZ);

PGDLLEXPORT Datum hashPointXYZ(PG_FUNCTION_ARGS) {
	PG_RETURN_DATUM(DirectFunctionCall1(hashvarlena, PointerGetDatum(PointXYZ2Bytea((PointXYZ *) PG_GETARG_POINTER(0)))));
}
PG_FUNCTION_INFO_V1(hashPointXYZ);

bool equalsPointXYZ(const PointXYZ *pointA, const PointXYZ *pointB) {
	return pointA->x == pointB->x && pointA->y == pointB->y && pointA->z == pointB->z;
}

PGDLLEXPORT Datum equalsPointXYZV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(equalsPointXYZ((PointXYZ *) PG_GETARG_POINTER(0),(PointXYZ *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(equalsPointXYZV1);

PGDLLEXPORT Datum getXPointXYZ(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(((PointXYZ *) PG_GETARG_POINTER(0))->x);
}
PG_FUNCTION_INFO_V1(getXPointXYZ);

PGDLLEXPORT Datum getYPointXYZ(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(((PointXYZ *) PG_GETARG_POINTER(0))->y);
}
PG_FUNCTION_INFO_V1(getYPointXYZ);

PGDLLEXPORT Datum getZPointXYZ(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(((PointXYZ *) PG_GETARG_POINTER(0))->z);
}
PG_FUNCTION_INFO_V1(getZPointXYZ);
