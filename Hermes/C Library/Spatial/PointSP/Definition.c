/**
 *	@file
 *	@author Marios Vodas (mvodas@gmail.com).
 *	@brief The implementantion of PointSP
 *
 *	@details This function is documented in the @ref PointSP/Definition.h
 *
 */

#include "../../h/Spatial/PointSP/Definition.h"
#include "../../h/Utilities/Utilities.h"
#include <libpq/pqformat.h>
#include <access/hash.h>

PointSP *clonePointSP(const PointSP *point, PointSP *result) {
	PointSP *ret = result ? result : (PointSP *) palloc(sizeof(*ret));

	*ret = *point;

	return ret;
}

PGDLLEXPORT Datum clonePointSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(clonePointSP((PointSP *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(clonePointSPV1);

PointSP *constructorPointSP(int32 x, int32 y, PointSP *result) {
	PointSP *ret = result ? result : (PointSP *) palloc(sizeof(*ret));

	ret->x = x;
	ret->y = y;

	return ret;
}

PGDLLEXPORT Datum constructorPointSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(constructorPointSP(PG_GETARG_INT32(0), PG_GETARG_INT32(1), NULL));
}
PG_FUNCTION_INFO_V1(constructorPointSPV1);

PointSP *inPointSP(const char *str, PointSP *result, char **error) {
	PointSP *ret = result ? result : (PointSP *) palloc(sizeof(*ret));

	if (error)
		*error = NULL;

	if (sscanf(str, " %d %d ", &ret->x, &ret->y) != 2) {
		if (error)
			asprintfHermes(error, "invalid input syntax for PointSP: \"%s\"", str);

		pfree(ret);
		return NULL ;
	}

	return ret;
}

PGDLLEXPORT Datum inPointSPV1(PG_FUNCTION_ARGS) {
	char *error;
	PointSP *result = inPointSP(PG_GETARG_CSTRING(0), NULL, &error);

	if (error)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("%s", error)));

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(inPointSPV1);

char *outPointSP(const PointSP *point) {
	char *result = NULL;

	asprintfHermes(&result, "%d %d", point->x, point->y);

	return result;
}

PGDLLEXPORT Datum outPointSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_CSTRING(outPointSP((PointSP *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(outPointSPV1);

PGDLLEXPORT Datum recvPointSP(PG_FUNCTION_ARGS) {
	StringInfo msg = (StringInfo) PG_GETARG_POINTER(0);
	PointSP *result = (PointSP *) palloc(sizeof(*result));

	result->x = pq_getmsgint(msg, sizeof(result->x));
	result->y = pq_getmsgint(msg, sizeof(result->y));

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(recvPointSP);

bytea *PointSP2Bytea(const PointSP *point) {
	StringInfoData buf;

	pq_begintypsend(&buf);

	pq_sendint(&buf, point->x, sizeof(point->x));
	pq_sendint(&buf, point->y, sizeof(point->y));

	return pq_endtypsend(&buf);
}

PGDLLEXPORT Datum sendPointSP(PG_FUNCTION_ARGS) {
	PG_RETURN_BYTEA_P(PointSP2Bytea((PointSP *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(sendPointSP);

PGDLLEXPORT Datum hashPointSP(PG_FUNCTION_ARGS) {
	PG_RETURN_DATUM(DirectFunctionCall1(hashvarlena, PointerGetDatum(PointSP2Bytea((PointSP *) PG_GETARG_POINTER(0)))));
}
PG_FUNCTION_INFO_V1(hashPointSP);

bool equalsPointSP(const PointSP *pointA, const PointSP *pointB) {
	return pointA->x == pointB->x && pointA->y == pointB->y;
}

PGDLLEXPORT Datum equalsPointSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(equalsPointSP((PointSP *) PG_GETARG_POINTER(0), (PointSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(equalsPointSPV1);

PGDLLEXPORT Datum getXPointSP(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(((PointSP *) PG_GETARG_POINTER(0))->x);
}
PG_FUNCTION_INFO_V1(getXPointSP);

PGDLLEXPORT Datum getYPointSP(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(((PointSP *) PG_GETARG_POINTER(0))->y);
}
PG_FUNCTION_INFO_V1(getYPointSP);
