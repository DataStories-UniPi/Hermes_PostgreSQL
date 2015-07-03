/**
 *	@file
 *	@author Marios Vodas (mvodas@gmail.com).
 *	@brief The implementation of PointST
 *
 *	@details These functions are documented in the @ref PointST/Definition.h
 *
 */
#include "../../h/SpatioTemporal/PointST/Definition.h"
#include "../../h/Utilities/Utilities.h"
#include <libpq/pqformat.h>
#include <access/hash.h>

PointST *clonePointST(const PointST *point, PointST *result) {
	PointST *ret = result ? result : (PointST *) palloc(sizeof(*ret));

	*ret = *point;

	return ret;
}

PGDLLEXPORT Datum clonePointSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(clonePointST((PointST *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(clonePointSTV1);

PointST *constructorPointST(Timestamp t, int32 x, int32 y, PointST *result) {
	PointST *ret = result ? result : (PointST *) palloc(sizeof(*ret));

	ret->t = t;
	ret->sp.x = x;
	ret->sp.y = y;

	return ret;
}

PGDLLEXPORT Datum constructorPointSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(constructorPointST(PG_GETARG_TIMESTAMP(0), PG_GETARG_INT32(1), PG_GETARG_INT32(2), NULL));
}
PG_FUNCTION_INFO_V1(constructorPointSTV1);

PointST *constructorHighPointST(Timestamp t, const PointSP *sp, PointST *result) {
	PointST *ret = result ? result : (PointST *) palloc(sizeof(*ret));

	ret->t = t;
	ret->sp = *sp;

	return ret;
}

PGDLLEXPORT Datum constructorHighPointSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(constructorHighPointST(PG_GETARG_TIMESTAMP(0), (PointSP *) PG_GETARG_POINTER(1), NULL));
}
PG_FUNCTION_INFO_V1(constructorHighPointSTV1);

PointST *inPointST(const char *str, PointST *result, char **error) {
	char ts[27];
	PointST *ret = result ? result : (PointST *) palloc(sizeof(*ret));

	if (error)
		*error = NULL;

	if (sscanf(str, " '%26[^']' %d %d ", ts, &ret->sp.x, &ret->sp.y) != 3) {
		if (error)
			asprintfHermes(error, "invalid input syntax for PointST: \"%s\"", str);

		pfree(ret);
		return NULL ;
	}

	ret->t = DatumGetTimestamp(DirectFunctionCall3(timestamp_in, CStringGetDatum(ts), PointerGetDatum(0), Int32GetDatum(-1)));

	return ret;
}

PGDLLEXPORT Datum inPointSTV1(PG_FUNCTION_ARGS) {
	char *error;
	PointST *result = inPointST(PG_GETARG_CSTRING(0), NULL, &error);

	if (error)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("%s", error)));

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(inPointSTV1);

char *outPointST(const PointST *point) {
	char *result = NULL;

	asprintfHermes(&result, "'%s' %d %d", DatumGetCString(DirectFunctionCall1(timestamp_out, TimestampGetDatum(point->t))), point->sp.x, point->sp.y);

	return result;
}

PGDLLEXPORT Datum outPointSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_CSTRING(outPointST((PointST *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(outPointSTV1);

PGDLLEXPORT Datum recvPointST(PG_FUNCTION_ARGS) {
	StringInfo msg = (StringInfo) PG_GETARG_POINTER(0);
	PointST *result = (PointST *) palloc(sizeof(*result));

	result->t = getmsgTimestampHermes(msg);
	result->sp.x = pq_getmsgint(msg, sizeof(result->sp.x));
	result->sp.y = pq_getmsgint(msg, sizeof(result->sp.y));

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(recvPointST);

bytea *PointST2Bytea(const PointST *point) {
	StringInfoData buf;

	pq_begintypsend(&buf);

	sendTimestampHermes(&buf, point->t);
	pq_sendint(&buf, point->sp.x, sizeof(point->sp.x));
	pq_sendint(&buf, point->sp.y, sizeof(point->sp.y));

	return pq_endtypsend(&buf);
}

PGDLLEXPORT Datum sendPointST(PG_FUNCTION_ARGS) {
	PG_RETURN_BYTEA_P(PointST2Bytea((PointST *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(sendPointST);

PGDLLEXPORT Datum hashPointST(PG_FUNCTION_ARGS) {
	PG_RETURN_DATUM(DirectFunctionCall1(hashvarlena, PointerGetDatum(PointST2Bytea((PointST *) PG_GETARG_POINTER(0)))));
}
PG_FUNCTION_INFO_V1(hashPointST);

bool equalsPointST(const PointST *pointA, const PointST *pointB) {
	return pointA->t == pointB->t && pointA->sp.x == pointB->sp.x && pointA->sp.y == pointB->sp.y;
}

PGDLLEXPORT Datum equalsPointSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(equalsPointST((PointST *) PG_GETARG_POINTER(0), (PointST *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(equalsPointSTV1);

PGDLLEXPORT Datum getTPointST(PG_FUNCTION_ARGS) {
	PG_RETURN_TIMESTAMP(((PointST *) PG_GETARG_POINTER(0))->t);
}
PG_FUNCTION_INFO_V1(getTPointST);

PGDLLEXPORT Datum getSpPointST(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(&((PointST *) PG_GETARG_POINTER(0))->sp);
}
PG_FUNCTION_INFO_V1(getSpPointST);

PGDLLEXPORT Datum getXPointST(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(((PointST *) PG_GETARG_POINTER(0))->sp.x);
}
PG_FUNCTION_INFO_V1(getXPointST);

PGDLLEXPORT Datum getYPointST(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(((PointST *) PG_GETARG_POINTER(0))->sp.y);
}
PG_FUNCTION_INFO_V1(getYPointST);
