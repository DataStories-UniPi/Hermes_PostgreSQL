/**
 *	@file
 *	@author Marios Vodas (mvodas@gmail.com).
 *	@brief The implementation of SegmentST
 *
 *	@details These functions are documented in the @ref SegmentST/Definition.h
 *
 */

#include "../../h/SpatioTemporal/SegmentST/Definition.h"
#include "../../h/Utilities/Utilities.h"
#include <libpq/pqformat.h>
#include <access/hash.h>

SegmentST *cloneSegmentST(const SegmentST *segment, SegmentST *result) {
	SegmentST *ret = result ? result : (SegmentST *) palloc(sizeof(*ret));

	*ret = *segment;

	return ret;
}

PGDLLEXPORT Datum cloneSegmentSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(cloneSegmentST((SegmentST *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(cloneSegmentSTV1);

SegmentST *constructorSegmentST(Timestamp ti, int32 ix, int32 iy, Timestamp te, int32 ex, int32 ey, SegmentST *result, char **error) {
	SegmentST *ret = NULL;

	if (error)
		*error = NULL;

	if (ti >= te) {
		if (error)
			*error = "invalid time sequence for SegmentST";
		return NULL ;
	}

	ret = result ? result : (SegmentST *) palloc(sizeof(*ret));

	ret->t.i = ti;
	ret->t.e = te;
	ret->sp.i.x = ix;
	ret->sp.i.y = iy;
	ret->sp.e.x = ex;
	ret->sp.e.y = ey;

	return ret;
}

PGDLLEXPORT Datum constructorSegmentSTV1(PG_FUNCTION_ARGS) {
	char *error;
	SegmentST *result = constructorSegmentST(PG_GETARG_TIMESTAMP(0), PG_GETARG_INT32(1), PG_GETARG_INT32(2), PG_GETARG_TIMESTAMP(3), PG_GETARG_INT32(4), PG_GETARG_INT32(5), NULL, &error);

	if (error)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("%s", error)));

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(constructorSegmentSTV1);

SegmentST *constructorHighSegmentST(const PointST *i, const PointST *e, SegmentST *result, char **error) {
	SegmentST *ret = NULL;

	if (error)
		*error = NULL;

	if (i->t >= e->t) {
		if (error)
			*error = "invalid time sequence for SegmentST";
		return NULL ;
	}

	ret = result ? result : (SegmentST *) palloc(sizeof(*ret));

	ret->t.i = i->t;
	ret->t.e = e->t;
	ret->sp.i = i->sp;
	ret->sp.e = e->sp;

	return ret;
}

PGDLLEXPORT Datum constructorHighSegmentSTV1(PG_FUNCTION_ARGS) {
	char *error;
	SegmentST *result = constructorHighSegmentST((PointST *) PG_GETARG_POINTER(0), (PointST *) PG_GETARG_POINTER(1), NULL, &error);

	if (error)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("%s", error)));

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(constructorHighSegmentSTV1);

SegmentST *inSegmentST(const char *str, SegmentST *result, char **error) {
	char tis[27], tes[27];
	Timestamp ti, te;
	int32 ix, iy, ex, ey;
	SegmentST *ret = NULL;

	if (error)
		*error = NULL;

	if (sscanf(str, " '%26[^']' %d %d '%26[^']' %d %d ", tis, &ix, &iy, tes, &ex, &ey) != 6) {
		if (error)
			asprintfHermes(error, "invalid input syntax for SegmentST: \"%s\"", str);
		return NULL ;
	}

	ti = DatumGetTimestamp(DirectFunctionCall3(timestamp_in, CStringGetDatum(tis), PointerGetDatum(0), Int32GetDatum(-1)));
	te = DatumGetTimestamp(DirectFunctionCall3(timestamp_in, CStringGetDatum(tes), PointerGetDatum(0), Int32GetDatum(-1)));

	if (ti >= te) {
		if (error)
			*error = "invalid time sequence for SegmentST";
		return NULL ;
	}

	ret = result ? result : (SegmentST *) palloc(sizeof(*ret));

	ret->t.i = ti;
	ret->t.e = te;
	ret->sp.i.x = ix;
	ret->sp.i.y = iy;
	ret->sp.e.x = ex;
	ret->sp.e.y = ey;

	return ret;
}

PGDLLEXPORT Datum inSegmentSTV1(PG_FUNCTION_ARGS) {
	char *error;
	SegmentST *result = inSegmentST(PG_GETARG_CSTRING(0), NULL, &error);

	if (error)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("%s", error)));

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(inSegmentSTV1);

char *outSegmentST(const SegmentST *segment) {
	char *result = NULL;

	asprintfHermes(&result, "'%s' %d %d '%s' %d %d", DatumGetCString(DirectFunctionCall1(timestamp_out, TimestampGetDatum(segment->t.i))), segment->sp.i.x, segment->sp.i.y, DatumGetCString(DirectFunctionCall1(timestamp_out, TimestampGetDatum(segment->t.e))), segment->sp.e.x, segment->sp.e.y);

	return result;
}

PGDLLEXPORT Datum outSegmentSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_CSTRING(outSegmentST((SegmentST *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(outSegmentSTV1);

PGDLLEXPORT Datum recvSegmentST(PG_FUNCTION_ARGS) {
	StringInfo msg = (StringInfo) PG_GETARG_POINTER(0);
	SegmentST *result = (SegmentST *) palloc(sizeof(*result));

	result->t.i = getmsgTimestampHermes(msg);
	result->t.e = getmsgTimestampHermes(msg);
	result->sp.i.x = pq_getmsgint(msg, sizeof(result->sp.i.x));
	result->sp.i.y = pq_getmsgint(msg, sizeof(result->sp.i.y));
	result->sp.e.x = pq_getmsgint(msg, sizeof(result->sp.e.x));
	result->sp.e.y = pq_getmsgint(msg, sizeof(result->sp.e.y));

	if (result->t.i >= result->t.e) {
		pfree(result);
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("invalid time sequence for SegmentST")));
	}

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(recvSegmentST);

bytea *SegmentST2Bytea(const SegmentST *segment) {
	StringInfoData buf;

	pq_begintypsend(&buf);

	sendTimestampHermes(&buf, segment->t.i);
	sendTimestampHermes(&buf, segment->t.e);
	pq_sendint(&buf, segment->sp.i.x, sizeof(segment->sp.i.x));
	pq_sendint(&buf, segment->sp.i.y, sizeof(segment->sp.i.y));
	pq_sendint(&buf, segment->sp.e.x, sizeof(segment->sp.e.x));
	pq_sendint(&buf, segment->sp.e.y, sizeof(segment->sp.e.y));

	return pq_endtypsend(&buf);
}

PGDLLEXPORT Datum sendSegmentST(PG_FUNCTION_ARGS) {
	PG_RETURN_BYTEA_P(SegmentST2Bytea((SegmentST *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(sendSegmentST);

PGDLLEXPORT Datum hashSegmentST(PG_FUNCTION_ARGS) {
	PG_RETURN_DATUM(DirectFunctionCall1(hashvarlena, PointerGetDatum(SegmentST2Bytea((SegmentST *) PG_GETARG_POINTER(0)))));
}
PG_FUNCTION_INFO_V1(hashSegmentST);

bool equalsSegmentST(const SegmentST *segmentA, const SegmentST *segmentB) {
	return segmentA->t.i == segmentB->t.i && segmentA->t.e == segmentB->t.e && segmentA->sp.i.x == segmentB->sp.i.x && segmentA->sp.i.y == segmentB->sp.i.y && segmentA->sp.e.x == segmentB->sp.e.x && segmentA->sp.e.y == segmentB->sp.e.y;
}

PGDLLEXPORT Datum equalsSegmentSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(equalsSegmentST((SegmentST *) PG_GETARG_POINTER(0), (SegmentST *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(equalsSegmentSTV1);

PGDLLEXPORT Datum getTSegmentST(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(&((SegmentST *) PG_GETARG_POINTER(0))->t);
}
PG_FUNCTION_INFO_V1(getTSegmentST);

PGDLLEXPORT Datum getTiSegmentST(PG_FUNCTION_ARGS) {
	PG_RETURN_TIMESTAMP(((SegmentST *) PG_GETARG_POINTER(0))->t.i);
}
PG_FUNCTION_INFO_V1(getTiSegmentST);

PGDLLEXPORT Datum getTeSegmentST(PG_FUNCTION_ARGS) {
	PG_RETURN_TIMESTAMP(((SegmentST *) PG_GETARG_POINTER(0))->t.e);
}
PG_FUNCTION_INFO_V1(getTeSegmentST);

PGDLLEXPORT Datum getSpSegmentST(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(&((SegmentST *) PG_GETARG_POINTER(0))->sp);
}
PG_FUNCTION_INFO_V1(getSpSegmentST);

PGDLLEXPORT Datum getISegmentST(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(&((SegmentST *) PG_GETARG_POINTER(0))->sp.i);
}
PG_FUNCTION_INFO_V1(getISegmentST);

PGDLLEXPORT Datum getIxSegmentST(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(((SegmentST *) PG_GETARG_POINTER(0))->sp.i.x);
}
PG_FUNCTION_INFO_V1(getIxSegmentST);

PGDLLEXPORT Datum getIySegmentST(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(((SegmentST *) PG_GETARG_POINTER(0))->sp.i.y);
}
PG_FUNCTION_INFO_V1(getIySegmentST);

PGDLLEXPORT Datum getESegmentST(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(&((SegmentST *) PG_GETARG_POINTER(0))->sp.e);
}
PG_FUNCTION_INFO_V1(getESegmentST);

PGDLLEXPORT Datum getExSegmentST(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(((SegmentST *) PG_GETARG_POINTER(0))->sp.e.x);
}
PG_FUNCTION_INFO_V1(getExSegmentST);

PGDLLEXPORT Datum getEySegmentST(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(((SegmentST *) PG_GETARG_POINTER(0))->sp.e.y);
}
PG_FUNCTION_INFO_V1(getEySegmentST);
