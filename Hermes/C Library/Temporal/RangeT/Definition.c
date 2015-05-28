/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/Temporal/RangeT/Definition.h"
#include "../../h/Utilities/Utilities.h"
#include <libpq/pqformat.h>
#include <access/hash.h>

RangeT *cloneRangeT(const RangeT *range, RangeT *result) {
	RangeT *ret = result ? result : (RangeT *) palloc(sizeof(*ret));

	*ret = *range;

	return ret;
}

PGDLLEXPORT Datum cloneRangeTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(cloneRangeT((RangeT *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(cloneRangeTV1);

RangeT *constructorRangeT(const Interval *r, Timestamp c, RangeT *result, char **error) {
	RangeT *ret = NULL;

	if (error)
		*error = NULL;

	if (interval2seconds(r) < 0.0) {
		if (error)
			*error = "negative interval for RangeT";
		return NULL ;
	}

	ret = result ? result : (RangeT *) palloc(sizeof(*ret));

	ret->r = *r;
	ret->c = c;

	return ret;
}

PGDLLEXPORT Datum constructorRangeTV1(PG_FUNCTION_ARGS) {
	char *error;
	RangeT *result = constructorRangeT(PG_GETARG_INTERVAL_P(0), PG_GETARG_TIMESTAMP(1), NULL, &error);

	if (error)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("%s", error)));

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(constructorRangeTV1);

RangeT *inRangeT(const char *str, RangeT *result, char **error) {
	char rs[27], cs[27];
	Interval *r = NULL;
	RangeT *ret = NULL;

	if (error)
		*error = NULL;

	if (sscanf(str, " '%26[^']' '%26[^']' ", rs, cs) != 2) {
		if (error)
			asprintfHermes(error, "invalid input syntax for RangeT: \"%s\"", str);
		return NULL ;
	}

	r = DatumGetIntervalP(DirectFunctionCall3(interval_in, CStringGetDatum(rs), PointerGetDatum(0), Int32GetDatum(-1)));

	if (interval2seconds(r) < 0.0) {
		if (error)
			*error = "negative interval for RangeT";

		pfree(r);
		return NULL ;
	}

	ret = result ? result : (RangeT *) palloc(sizeof(*ret));

	ret->r = *r;
	pfree(r);

	ret->c = DatumGetTimestamp(DirectFunctionCall3(timestamp_in, CStringGetDatum(cs), PointerGetDatum(0), Int32GetDatum(-1)));

	return ret;
}

PGDLLEXPORT Datum inRangeTV1(PG_FUNCTION_ARGS) {
	char *error;
	RangeT *result = inRangeT(PG_GETARG_CSTRING(0), NULL, &error);

	if (error)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("%s", error)));

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(inRangeTV1);

char *outRangeT(const RangeT *range) {
	char *result = NULL;

	asprintfHermes(&result, "'%s' '%s'", DatumGetCString(DirectFunctionCall1(interval_out, PointerGetDatum(&range->r))), DatumGetCString(DirectFunctionCall1(timestamp_out, TimestampGetDatum(range->c))) );

	return result;
}

PGDLLEXPORT Datum outRangeTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_CSTRING(outRangeT((RangeT *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(outRangeTV1);

PGDLLEXPORT Datum recvRangeT(PG_FUNCTION_ARGS) {
	StringInfo msg = (StringInfo) PG_GETARG_POINTER(0);
	RangeT *result = (RangeT *) palloc(sizeof(*result));

	result->r.time = getmsgTimeOffsetHermes(msg);
	result->r.day = pq_getmsgint(msg, sizeof(result->r.day));
	result->r.month = pq_getmsgint(msg, sizeof(result->r.month));
	result->c = getmsgTimestampHermes(msg);

	if (interval2seconds(&result->r) < 0.0) {
		pfree(result);
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("negative interval for RangeT")));
	}

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(recvRangeT);

bytea *RangeT2Bytea(const RangeT *range) {
	StringInfoData buf;

	pq_begintypsend(&buf);

	sendTimeOffsetHermes(&buf, range->r.time);
	pq_sendint(&buf, range->r.day, sizeof(range->r.day));
	pq_sendint(&buf, range->r.month, sizeof(range->r.month));
	sendTimestampHermes(&buf, range->c);

	return pq_endtypsend(&buf);
}

PGDLLEXPORT Datum sendRangeT(PG_FUNCTION_ARGS) {
	PG_RETURN_BYTEA_P(RangeT2Bytea((RangeT *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(sendRangeT);

PGDLLEXPORT Datum hashRangeT(PG_FUNCTION_ARGS) {
	PG_RETURN_DATUM(DirectFunctionCall1(hashvarlena, PointerGetDatum(RangeT2Bytea((RangeT *) PG_GETARG_POINTER(0)))));
}
PG_FUNCTION_INFO_V1(hashRangeT);

bool equalsRangeT(const RangeT *rangeA, const RangeT *rangeB) {
	return rangeA->c == rangeB->c && DatumGetBool(DirectFunctionCall2(interval_eq, IntervalPGetDatum(&rangeA->r), IntervalPGetDatum(&rangeB->r))) ;
}

PGDLLEXPORT Datum equalsRangeTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(equalsRangeT((RangeT *) PG_GETARG_POINTER(0),(RangeT *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(equalsRangeTV1);

PGDLLEXPORT Datum getRRangeT(PG_FUNCTION_ARGS) {
	PG_RETURN_INTERVAL_P(&((RangeT *) PG_GETARG_POINTER(0))->r);
}
PG_FUNCTION_INFO_V1(getRRangeT);

PGDLLEXPORT Datum getCRangeT(PG_FUNCTION_ARGS) {
	PG_RETURN_TIMESTAMP(((RangeT *) PG_GETARG_POINTER(0))->c);
}
PG_FUNCTION_INFO_V1(getCRangeT);
