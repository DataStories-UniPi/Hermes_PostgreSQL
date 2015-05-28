/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../../h/Spatial/CRS/PointENU/Definition.h"
#include "../../../h/Utilities/Utilities.h"
#include <libpq/pqformat.h>
#include <access/hash.h>

PointENU *clonePointENU(const PointENU *point, PointENU *result) {
	PointENU *ret = result ? result : (PointENU *) palloc(sizeof(*ret));

	*ret = *point;

	return ret;
}

PGDLLEXPORT Datum clonePointENUV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(clonePointENU((PointENU *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(clonePointENUV1);

PointENU *constructorPointENU(float8 e, float8 n, float8 u, PointENU *result) {
	PointENU *ret = result ? result : (PointENU *) palloc(sizeof(*ret));

	ret->e = e;
	ret->n = n;
	ret->u = u;

	return ret;
}

PGDLLEXPORT Datum constructorPointENUV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(constructorPointENU(PG_GETARG_FLOAT8(0), PG_GETARG_FLOAT8(1), PG_GETARG_FLOAT8(2), NULL));
}
PG_FUNCTION_INFO_V1(constructorPointENUV1);

PointENU *inPointENU(const char *str, PointENU *result, char **error) {
	float8 e, n, u;
	PointENU *ret = NULL;

	if (error)
		*error = NULL;

	if (sscanf(str, " %lf %lf %lf ", &e, &n, &u) != 3) {
		if (error)
			asprintfHermes(error, "invalid input syntax for PointENU: \"%s\"", str);
		return NULL ;
	}

	ret = result ? result : (PointENU *) palloc(sizeof(*ret));

	ret->e = e;
	ret->n = n;
	ret->u = u;

	return ret;
}

PGDLLEXPORT Datum inPointENUV1(PG_FUNCTION_ARGS) {
	char *error;
	PointENU *result = inPointENU(PG_GETARG_CSTRING(0), NULL, &error);

	if (error)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("%s", error)));

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(inPointENUV1);

char *outPointENU(const PointENU *point) {
	char *result = NULL;

	asprintfHermes(&result, "%f %f %f", point->e, point->n, point->u);

	return result;
}

PGDLLEXPORT Datum outPointENUV1(PG_FUNCTION_ARGS) {
	PG_RETURN_CSTRING(outPointENU((PointENU *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(outPointENUV1);

PGDLLEXPORT Datum recvPointENU(PG_FUNCTION_ARGS) {
	StringInfo msg = (StringInfo) PG_GETARG_POINTER(0);
	PointENU *result = (PointENU *) palloc(sizeof(*result));

	result->e = pq_getmsgfloat8(msg);
	result->n = pq_getmsgfloat8(msg);
	result->u = pq_getmsgfloat8(msg);

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(recvPointENU);

bytea *PointENU2Bytea(const PointENU *point) {
	StringInfoData buf;

	pq_begintypsend(&buf);

	pq_sendfloat8(&buf, ELIMINATE_MINUS_ZERO(point->e));
	pq_sendfloat8(&buf, ELIMINATE_MINUS_ZERO(point->n));
	pq_sendfloat8(&buf, ELIMINATE_MINUS_ZERO(point->u));

	return pq_endtypsend(&buf);
}

PGDLLEXPORT Datum sendPointENU(PG_FUNCTION_ARGS) {
	PG_RETURN_BYTEA_P(PointENU2Bytea((PointENU *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(sendPointENU);

PGDLLEXPORT Datum hashPointENU(PG_FUNCTION_ARGS) {
	PG_RETURN_DATUM(DirectFunctionCall1(hashvarlena, PointerGetDatum(PointENU2Bytea((PointENU *) PG_GETARG_POINTER(0)))));
}
PG_FUNCTION_INFO_V1(hashPointENU);

bool equalsPointENU(const PointENU *pointA, const PointENU *pointB) {
	return pointA->e == pointB->e && pointA->n == pointB->n && pointA->u == pointB->u;
}

PGDLLEXPORT Datum equalsPointENUV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(equalsPointENU((PointENU *) PG_GETARG_POINTER(0),(PointENU *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(equalsPointENUV1);

PGDLLEXPORT Datum getEPointENU(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(((PointENU *) PG_GETARG_POINTER(0))->e);
}
PG_FUNCTION_INFO_V1(getEPointENU);

PGDLLEXPORT Datum getNPointENU(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(((PointENU *) PG_GETARG_POINTER(0))->n);
}
PG_FUNCTION_INFO_V1(getNPointENU);

PGDLLEXPORT Datum getUPointENU(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(((PointENU *) PG_GETARG_POINTER(0))->u);
}
PG_FUNCTION_INFO_V1(getUPointENU);
