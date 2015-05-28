/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../../h/Spatial/CRS/PointXY/Definition.h"
#include "../../../h/Utilities/Utilities.h"
#include <libpq/pqformat.h>
#include <access/hash.h>

PointXY *clonePointXY(const PointXY *point, PointXY *result) {
	PointXY *ret = result ? result : (PointXY *) palloc(sizeof(*ret));

	*ret = *point;

	return ret;
}

PGDLLEXPORT Datum clonePointXYV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(clonePointXY((PointXY *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(clonePointXYV1);

PointXY *constructorPointXY(float8 x, float8 y, PointXY *result) {
	PointXY *ret = result ? result : (PointXY *) palloc(sizeof(*ret));

	ret->x = x;
	ret->y = y;

	return ret;
}

PGDLLEXPORT Datum constructorPointXYV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(constructorPointXY(PG_GETARG_FLOAT8(0), PG_GETARG_FLOAT8(1), NULL));
}
PG_FUNCTION_INFO_V1(constructorPointXYV1);

PointXY *inPointXY(const char *str, PointXY *result, char **error) {
	PointXY *ret = result ? result : (PointXY *) palloc(sizeof(*ret));

	if (error)
		*error = NULL;

	if (sscanf(str, " %lf %lf ", &ret->x, &ret->y) != 2) {
		if (error)
			asprintfHermes(error, "invalid input syntax for PointXY: \"%s\"", str);

		pfree(ret);
		return NULL ;
	}

	return ret;
}

PGDLLEXPORT Datum inPointXYV1(PG_FUNCTION_ARGS) {
	char *error;
	PointXY *result = inPointXY(PG_GETARG_CSTRING(0), NULL, &error);

	if (error)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("%s", error)));

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(inPointXYV1);

char *outPointXY(const PointXY *point) {
	char *result = NULL;

	asprintfHermes(&result, "%f %f", point->x, point->y);

	return result;
}

PGDLLEXPORT Datum outPointXYV1(PG_FUNCTION_ARGS) {
	PG_RETURN_CSTRING(outPointXY((PointXY *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(outPointXYV1);

PGDLLEXPORT Datum recvPointXY(PG_FUNCTION_ARGS) {
	StringInfo msg = (StringInfo) PG_GETARG_POINTER(0);
	PointXY *result = (PointXY *) palloc(sizeof(*result));

	result->x = pq_getmsgfloat8(msg);
	result->y = pq_getmsgfloat8(msg);

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(recvPointXY);

bytea *PointXY2Bytea(const PointXY *point) {
	StringInfoData buf;

	pq_begintypsend(&buf);

	pq_sendfloat8(&buf, ELIMINATE_MINUS_ZERO(point->x));
	pq_sendfloat8(&buf, ELIMINATE_MINUS_ZERO(point->y));

	return pq_endtypsend(&buf);
}

PGDLLEXPORT Datum sendPointXY(PG_FUNCTION_ARGS) {
	PG_RETURN_BYTEA_P(PointXY2Bytea((PointXY *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(sendPointXY);

PGDLLEXPORT Datum hashPointXY(PG_FUNCTION_ARGS) {
	PG_RETURN_DATUM(DirectFunctionCall1(hashvarlena, PointerGetDatum(PointXY2Bytea((PointXY *) PG_GETARG_POINTER(0)))));
}
PG_FUNCTION_INFO_V1(hashPointXY);

bool equalsPointXY(const PointXY *pointA, const PointXY *pointB) {
	return pointA->x == pointB->x && pointA->y == pointB->y;
}

PGDLLEXPORT Datum equalsPointXYV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(equalsPointXY((PointXY *) PG_GETARG_POINTER(0), (PointXY *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(equalsPointXYV1);

PGDLLEXPORT Datum getXPointXY(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(((PointXY *) PG_GETARG_POINTER(0))->x);
}
PG_FUNCTION_INFO_V1(getXPointXY);

PGDLLEXPORT Datum getYPointXY(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(((PointXY *) PG_GETARG_POINTER(0))->y);
}
PG_FUNCTION_INFO_V1(getYPointXY);
