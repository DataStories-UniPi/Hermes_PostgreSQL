/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/Spatial/SegmentSP/Definition.h"
#include "../../h/Utilities/Utilities.h"
#include <libpq/pqformat.h>
#include <access/hash.h>

SegmentSP *cloneSegmentSP(const SegmentSP *segment, SegmentSP *result) {
	SegmentSP *ret = result ? result : (SegmentSP *) palloc(sizeof(*ret));

	*ret = *segment;

	return ret;
}

PGDLLEXPORT Datum cloneSegmentSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(cloneSegmentSP((SegmentSP *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(cloneSegmentSPV1);

SegmentSP *constructorSegmentSP(int32 ix, int32 iy, int32 ex, int32 ey, SegmentSP *result, char **error) {
	SegmentSP *ret = NULL;

	if (error)
		*error = NULL;

	if (ix == ex && iy == ey) {
		if (error)
			*error = "invalid geometry for SegmentSP";
		return NULL ;
	}

	ret = result ? result : (SegmentSP *) palloc(sizeof(*ret));

	ret->i.x = ix;
	ret->i.y = iy;
	ret->e.x = ex;
	ret->e.y = ey;

	return ret;
}

PGDLLEXPORT Datum constructorSegmentSPV1(PG_FUNCTION_ARGS) {
	char *error;
	SegmentSP *result = constructorSegmentSP(PG_GETARG_INT32(0), PG_GETARG_INT32(1), PG_GETARG_INT32(2), PG_GETARG_INT32(3), NULL, &error);

	if (error)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("%s", error)));

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(constructorSegmentSPV1);

SegmentSP *constructorHighSegmentSP(const PointSP *i, const PointSP *e, SegmentSP *result, char **error) {
	SegmentSP *ret = NULL;

	if (error)
		*error = NULL;

	if (i->x == e->x && i->y == e->y) {
		if (error)
			*error = "invalid geometry for SegmentSP";
		return NULL ;
	}

	ret = result ? result : (SegmentSP *) palloc(sizeof(*ret));

	ret->i = *i;
	ret->e = *e;

	return ret;
}

PGDLLEXPORT Datum constructorHighSegmentSPV1(PG_FUNCTION_ARGS) {
	char *error;
	SegmentSP *result = constructorHighSegmentSP((PointSP *) PG_GETARG_POINTER(0), (PointSP *) PG_GETARG_POINTER(1), NULL, &error);

	if (error)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("%s", error)));

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(constructorHighSegmentSPV1);

SegmentSP *inSegmentSP(const char *str, SegmentSP *result, char **error) {
	int32 ix, iy, ex, ey;
	SegmentSP *ret = NULL;

	if (error)
		*error = NULL;

	if (sscanf(str, " %d %d %d %d ", &ix, &iy, &ex, &ey) != 4) {
		if (error)
			asprintfHermes(error, "invalid input syntax for SegmentSP: \"%s\"", str);
		return NULL ;
	}
	if (ix == ex && iy == ey) {
		if (error)
			*error = "invalid geometry for SegmentSP";
		return NULL ;
	}

	ret = result ? result : (SegmentSP *) palloc(sizeof(*ret));

	ret->i.x = ix;
	ret->i.y = iy;
	ret->e.x = ex;
	ret->e.y = ey;

	return ret;
}

PGDLLEXPORT Datum inSegmentSPV1(PG_FUNCTION_ARGS) {
	char *error;
	SegmentSP *result = inSegmentSP(PG_GETARG_CSTRING(0), NULL, &error);

	if (error)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("%s", error)));

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(inSegmentSPV1);

char *outSegmentSP(const SegmentSP *segment) {
	char *result = NULL;

	asprintfHermes(&result, "%d %d %d %d", segment->i.x, segment->i.y, segment->e.x, segment->e.y);

	return result;
}

PGDLLEXPORT Datum outSegmentSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_CSTRING(outSegmentSP((SegmentSP *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(outSegmentSPV1);

PGDLLEXPORT Datum recvSegmentSP(PG_FUNCTION_ARGS) {
	StringInfo msg = (StringInfo) PG_GETARG_POINTER(0);
	SegmentSP *result = (SegmentSP *) palloc(sizeof(*result));

	result->i.x = pq_getmsgint(msg, sizeof(result->i.x));
	result->i.y = pq_getmsgint(msg, sizeof(result->i.y));
	result->e.x = pq_getmsgint(msg, sizeof(result->e.x));
	result->e.y = pq_getmsgint(msg, sizeof(result->e.y));

	if (result->i.x == result->e.x && result->i.y == result->e.y) {
		pfree(result);
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("invalid geometry for SegmentSP")));
	}

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(recvSegmentSP);

bytea *SegmentSP2Bytea(const SegmentSP *segment) {
	StringInfoData buf;

	pq_begintypsend(&buf);

	pq_sendint(&buf, segment->i.x, sizeof(segment->i.x));
	pq_sendint(&buf, segment->i.y, sizeof(segment->i.y));
	pq_sendint(&buf, segment->e.x, sizeof(segment->e.x));
	pq_sendint(&buf, segment->e.y, sizeof(segment->e.y));

	return pq_endtypsend(&buf);
}

PGDLLEXPORT Datum sendSegmentSP(PG_FUNCTION_ARGS) {
	PG_RETURN_BYTEA_P(SegmentSP2Bytea((SegmentSP *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(sendSegmentSP);

PGDLLEXPORT Datum hashSegmentSP(PG_FUNCTION_ARGS) {
	PG_RETURN_DATUM(DirectFunctionCall1(hashvarlena, PointerGetDatum(SegmentSP2Bytea((SegmentSP *) PG_GETARG_POINTER(0)))));
}
PG_FUNCTION_INFO_V1(hashSegmentSP);

bool equalsSegmentSP(const SegmentSP *segmentA, const SegmentSP *segmentB) {
	return segmentA->i.x == segmentB->i.x && segmentA->i.y == segmentB->i.y && segmentA->e.x == segmentB->e.x && segmentA->e.y == segmentB->e.y;
}

PGDLLEXPORT Datum equalsSegmentSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(equalsSegmentSP((SegmentSP *) PG_GETARG_POINTER(0),(SegmentSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(equalsSegmentSPV1);

PGDLLEXPORT Datum getISegmentSP(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(&((SegmentSP *) PG_GETARG_POINTER(0))->i);
}
PG_FUNCTION_INFO_V1(getISegmentSP);

PGDLLEXPORT Datum getIxSegmentSP(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(((SegmentSP *) PG_GETARG_POINTER(0))->i.x);
}
PG_FUNCTION_INFO_V1(getIxSegmentSP);

PGDLLEXPORT Datum getIySegmentSP(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(((SegmentSP *) PG_GETARG_POINTER(0))->i.y);
}
PG_FUNCTION_INFO_V1(getIySegmentSP);

PGDLLEXPORT Datum getESegmentSP(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(&((SegmentSP *) PG_GETARG_POINTER(0))->e);
}
PG_FUNCTION_INFO_V1(getESegmentSP);

PGDLLEXPORT Datum getExSegmentSP(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(((SegmentSP *) PG_GETARG_POINTER(0))->e.x);
}
PG_FUNCTION_INFO_V1(getExSegmentSP);

PGDLLEXPORT Datum getEySegmentSP(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(((SegmentSP *) PG_GETARG_POINTER(0))->e.y);
}
PG_FUNCTION_INFO_V1(getEySegmentSP);
