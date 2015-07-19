/**
 *	@file
 *	@author Marios Vodas (mvodas@gmail.com).
 *	@brief The implementation of RangeST
 *
 *	@details These functions are documented in the @ref RangeST/Definition.h
 *
 */

#include "../../h/SpatioTemporal/RangeST/Definition.h"
#include "../../h/Utilities/Utilities.h"
#include <libpq/pqformat.h>
#include <access/hash.h>

RangeST *cloneRangeST(const RangeST *range, RangeST *result) {
	RangeST *ret = result ? result : (RangeST *) palloc(sizeof(*ret));

	*ret = *range;

	return ret;
}

PGDLLEXPORT Datum cloneRangeSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(cloneRangeST((RangeST *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(cloneRangeSTV1);

RangeST *constructorRangeST(const Interval *tr, Timestamp tc, int32 spr, int32 cx, int32 cy, RangeST *result, char **error) {
	RangeST *ret = NULL;

	if (error)
		*error = NULL;

	if (interval2seconds(tr) < 0.0 || spr < 0) {
		if (error)
			*error = "invalid temporal or spatial component for RangeST";
		return NULL ;
	}

	ret = result ? result : (RangeST *) palloc(sizeof(*ret));

	ret->t.r = *tr;
	ret->t.c = tc;
	ret->sp.r = spr;
	ret->sp.c.x = cx;
	ret->sp.c.y = cy;

	return ret;
}

PGDLLEXPORT Datum constructorRangeSTV1(PG_FUNCTION_ARGS) {
	char *error;
	RangeST *result = constructorRangeST(PG_GETARG_INTERVAL_P(0), PG_GETARG_TIMESTAMP(1), PG_GETARG_INT32(2), PG_GETARG_INT32(3), PG_GETARG_INT32(4), NULL, &error);

	if (error)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("%s", error)));

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(constructorRangeSTV1);

RangeST *constructorHighRangeST(const RangeT *t, const RangeSP *sp, RangeST *result, char **error) {
	RangeST *ret = NULL;

	if (error)
		*error = NULL;

	if (interval2seconds(&t->r) < 0.0 || sp->r < 0) {
		if (error)
			*error = "invalid temporal or spatial component for RangeST";
		return NULL ;
	}

	ret = result ? result : (RangeST *) palloc(sizeof(*ret));

	ret->t = *t;
	ret->sp = *sp;

	return ret;
}

PGDLLEXPORT Datum constructorHighRangeSTV1(PG_FUNCTION_ARGS) {
	char *error;
	RangeST *result = constructorHighRangeST((RangeT *) PG_GETARG_POINTER(0), (RangeSP *) PG_GETARG_POINTER(1), NULL, &error);

	if (error)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("%s", error)));

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(constructorHighRangeSTV1);

RangeST *inRangeST(const char *str, RangeST *result, char **error) {
	char trs[27], tcs[27];
	Interval *tr;
	int32 spr, cx, cy;
	RangeST *ret = NULL;

	if (error)
		*error = NULL;

	if (sscanf(str, " '%26[^']' '%26[^']' %d %d %d ", trs, tcs, &spr, &cx, &cy) != 5) {
		if (error)
			asprintfHermes(error, "invalid input syntax for RangeST: \"%s\"", str);
		return NULL ;
	}

	tr = DatumGetIntervalP(DirectFunctionCall3(interval_in, CStringGetDatum(trs), PointerGetDatum(0), Int32GetDatum(-1)));

	if (interval2seconds(tr) < 0.0 || spr < 0) {
		if (error)
			*error = "invalid temporal or spatial component for RangeST";

		pfree(tr);
		return NULL ;
	}

	ret = result ? result : (RangeST *) palloc(sizeof(*ret));

	ret->t.r = *tr;
	pfree(tr);

	ret->t.c = DatumGetTimestamp(DirectFunctionCall3(timestamp_in, CStringGetDatum(tcs), PointerGetDatum(0), Int32GetDatum(-1)));
	ret->sp.r = spr;
	ret->sp.c.x = cx;
	ret->sp.c.y = cy;

	return ret;
}

PGDLLEXPORT Datum inRangeSTV1(PG_FUNCTION_ARGS) {
	char *error;
	RangeST *result = inRangeST(PG_GETARG_CSTRING(0), NULL, &error);

	if (error)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("%s", error)));

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(inRangeSTV1);

char *outRangeST(const RangeST *range) {
	char *result = NULL;

	asprintfHermes(&result, "'%s' '%s' %d %d %d", DatumGetCString(DirectFunctionCall1(interval_out, PointerGetDatum(&range->t.r))), DatumGetCString(DirectFunctionCall1(timestamp_out, TimestampGetDatum(range->t.c))), range->sp.r, range->sp.c.x, range->sp.c.y);

	return result;
}

PGDLLEXPORT Datum outRangeSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_CSTRING(outRangeST((RangeST *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(outRangeSTV1);

PGDLLEXPORT Datum recvRangeST(PG_FUNCTION_ARGS) {
	StringInfo msg = (StringInfo) PG_GETARG_POINTER(0);
	RangeST *result = (RangeST *) palloc(sizeof(*result));

	result->t.r.time = getmsgTimeOffsetHermes(msg);
	result->t.r.day = pq_getmsgint(msg, sizeof(result->t.r.day));
	result->t.r.month = pq_getmsgint(msg, sizeof(result->t.r.month));
	result->t.c = getmsgTimestampHermes(msg);
	result->sp.r = pq_getmsgint(msg, sizeof(result->sp.r));
	result->sp.c.x = pq_getmsgint(msg, sizeof(result->sp.c.x));
	result->sp.c.y = pq_getmsgint(msg, sizeof(result->sp.c.y));

	if (interval2seconds(&result->t.r) < 0.0 || result->sp.r < 0) {
		pfree(result);
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("invalid temporal or spatial component for RangeST")));
	}

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(recvRangeST);

bytea *RangeST2Bytea(const RangeST *range) {
	StringInfoData buf;

	pq_begintypsend(&buf);

	sendTimeOffsetHermes(&buf, range->t.r.time);
	pq_sendint(&buf, range->t.r.day, sizeof(range->t.r.day));
	pq_sendint(&buf, range->t.r.month, sizeof(range->t.r.month));
	sendTimestampHermes(&buf, range->t.c);
	pq_sendint(&buf, range->sp.r, sizeof(range->sp.r));
	pq_sendint(&buf, range->sp.c.x, sizeof(range->sp.c.x));
	pq_sendint(&buf, range->sp.c.y, sizeof(range->sp.c.y));

	return pq_endtypsend(&buf);
}

PGDLLEXPORT Datum sendRangeST(PG_FUNCTION_ARGS) {
	PG_RETURN_BYTEA_P(RangeST2Bytea((RangeST *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(sendRangeST);

PGDLLEXPORT Datum hashRangeST(PG_FUNCTION_ARGS) {
	PG_RETURN_DATUM(DirectFunctionCall1(hashvarlena, PointerGetDatum(RangeST2Bytea((RangeST *) PG_GETARG_POINTER(0)))));
}
PG_FUNCTION_INFO_V1(hashRangeST);

bool equalsRangeST(const RangeST *rangeA, const RangeST *rangeB) {
	return rangeA->t.c == rangeB->t.c && DatumGetBool(DirectFunctionCall2(interval_eq, IntervalPGetDatum(&rangeA->t.r), IntervalPGetDatum(&rangeB->t.r))) && rangeA->sp.r == rangeB->sp.r && rangeA->sp.c.x == rangeB->sp.c.x && rangeA->sp.c.y == rangeB->sp.c.y;
}

PGDLLEXPORT Datum equalsRangeSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(equalsRangeST((RangeST *) PG_GETARG_POINTER(0),(RangeST *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(equalsRangeSTV1);

PGDLLEXPORT Datum getTRangeST(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(&((RangeST *) PG_GETARG_POINTER(0))->t);
}
PG_FUNCTION_INFO_V1(getTRangeST);

PGDLLEXPORT Datum getTrRangeST(PG_FUNCTION_ARGS) {
	PG_RETURN_INTERVAL_P(&((RangeST *) PG_GETARG_POINTER(0))->t.r);
}
PG_FUNCTION_INFO_V1(getTrRangeST);

PGDLLEXPORT Datum getTcRangeST(PG_FUNCTION_ARGS) {
	PG_RETURN_TIMESTAMP(((RangeST *) PG_GETARG_POINTER(0))->t.c);
}
PG_FUNCTION_INFO_V1(getTcRangeST);

PGDLLEXPORT Datum getSpRangeST(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(&((RangeST *) PG_GETARG_POINTER(0))->sp);
}
PG_FUNCTION_INFO_V1(getSpRangeST);

PGDLLEXPORT Datum getSprRangeST(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(((RangeST *) PG_GETARG_POINTER(0))->sp.r);
}
PG_FUNCTION_INFO_V1(getSprRangeST);

PGDLLEXPORT Datum getSpcRangeST(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(&((RangeST *) PG_GETARG_POINTER(0))->sp.c);
}
PG_FUNCTION_INFO_V1(getSpcRangeST);

PGDLLEXPORT Datum getCxRangeST(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(((RangeST *) PG_GETARG_POINTER(0))->sp.c.x);
}
PG_FUNCTION_INFO_V1(getCxRangeST);

PGDLLEXPORT Datum getCyRangeST(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(((RangeST *) PG_GETARG_POINTER(0))->sp.c.y);
}
PG_FUNCTION_INFO_V1(getCyRangeST);
