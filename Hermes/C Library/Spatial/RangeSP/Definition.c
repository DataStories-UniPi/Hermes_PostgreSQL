/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/Spatial/RangeSP/Definition.h"
#include "../../h/Utilities/Utilities.h"
#include <libpq/pqformat.h>
#include <access/hash.h>

RangeSP *cloneRangeSP(const RangeSP *range, RangeSP *result) {
	RangeSP *ret = result ? result : (RangeSP *) palloc(sizeof(*ret));

	*ret = *range;

	return ret;
}

PGDLLEXPORT Datum cloneRangeSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(cloneRangeSP((RangeSP *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(cloneRangeSPV1);

RangeSP *constructorRangeSP(int32 r, int32 cx, int32 cy, RangeSP *result, char **error) {
	RangeSP *ret = NULL;

	if (error)
		*error = NULL;

	if (r < 0) {
		if (error)
			*error = "invalid geometry for RangeSP";
		return NULL ;
	}

	ret = result ? result : (RangeSP *) palloc(sizeof(*ret));

	ret->r = r;
	ret->c.x = cx;
	ret->c.y = cy;

	return ret;
}

PGDLLEXPORT Datum constructorRangeSPV1(PG_FUNCTION_ARGS) {
	char *error;
	RangeSP *result = constructorRangeSP(PG_GETARG_INT32(0), PG_GETARG_INT32(1), PG_GETARG_INT32(2), NULL, &error);

	if (error)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("%s", error)));

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(constructorRangeSPV1);

RangeSP *constructorHighRangeSP(int32 r, const PointSP *c, RangeSP *result, char **error) {
	RangeSP *ret = NULL;

	if (error)
		*error = NULL;

	if (r < 0) {
		if (error)
			*error = "invalid geometry for RangeSP";
		return NULL ;
	}

	ret = result ? result : (RangeSP *) palloc(sizeof(*ret));

	ret->r = r;
	ret->c = *c;

	return ret;
}

PGDLLEXPORT Datum constructorHighRangeSPV1(PG_FUNCTION_ARGS) {
	char *error;
	RangeSP *result = constructorHighRangeSP(PG_GETARG_INT32(0), (PointSP *) PG_GETARG_POINTER(1), NULL, &error);

	if (error)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("%s", error)));

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(constructorHighRangeSPV1);

RangeSP *inRangeSP(const char *str, RangeSP *result, char **error) {
	int32 r, cx, cy;
	RangeSP *ret = NULL;

	if (error)
		*error = NULL;

	if (sscanf(str, " %d %d %d ", &r, &cx, &cy) != 3) {
		if (error)
			asprintfHermes(error, "invalid input syntax for RangeSP: \"%s\"", str);
		return NULL ;
	}
	if (r < 0) {
		if (error)
			*error = "invalid geometry for RangeSP";
		return NULL ;
	}

	ret = result ? result : (RangeSP *) palloc(sizeof(*ret));

	ret->r = r;
	ret->c.x = cx;
	ret->c.y = cy;

	return ret;
}

PGDLLEXPORT Datum inRangeSPV1(PG_FUNCTION_ARGS) {
	char *error;
	RangeSP *result = inRangeSP(PG_GETARG_CSTRING(0), NULL, &error);

	if (error)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("%s", error)));

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(inRangeSPV1);

char *outRangeSP(const RangeSP *range) {
	char *result = NULL;

	asprintfHermes(&result, "%d %d %d", range->r, range->c.x, range->c.y);

	return result;
}

PGDLLEXPORT Datum outRangeSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_CSTRING(outRangeSP((RangeSP *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(outRangeSPV1);

PGDLLEXPORT Datum recvRangeSP(PG_FUNCTION_ARGS) {
	StringInfo msg = (StringInfo) PG_GETARG_POINTER(0);
	RangeSP *result = (RangeSP *) palloc(sizeof(*result));

	result->r = pq_getmsgint(msg, sizeof(result->r));
	result->c.x = pq_getmsgint(msg, sizeof(result->c.x));
	result->c.y = pq_getmsgint(msg, sizeof(result->c.y));

	if (result->r < 0) {
		pfree(result);
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("invalid geometry for RangeSP")));
	}

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(recvRangeSP);

bytea *RangeSP2Bytea(const RangeSP *range) {
	StringInfoData buf;

	pq_begintypsend(&buf);

	pq_sendint(&buf, range->r, sizeof(range->r));
	pq_sendint(&buf, range->c.x, sizeof(range->c.x));
	pq_sendint(&buf, range->c.y, sizeof(range->c.y));

	return pq_endtypsend(&buf);
}

PGDLLEXPORT Datum sendRangeSP(PG_FUNCTION_ARGS) {
	PG_RETURN_BYTEA_P(RangeSP2Bytea((RangeSP *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(sendRangeSP);

PGDLLEXPORT Datum hashRangeSP(PG_FUNCTION_ARGS) {
	PG_RETURN_DATUM(DirectFunctionCall1(hashvarlena, PointerGetDatum(RangeSP2Bytea((RangeSP *) PG_GETARG_POINTER(0)))));
}
PG_FUNCTION_INFO_V1(hashRangeSP);

bool equalsRangeSP(const RangeSP *rangeA, const RangeSP *rangeB) {
	return rangeA->r == rangeB->r && rangeA->c.x == rangeB->c.x && rangeA->c.y == rangeB->c.y;
}

PGDLLEXPORT Datum equalsRangeSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(equalsRangeSP((RangeSP *) PG_GETARG_POINTER(0),(RangeSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(equalsRangeSPV1);

PGDLLEXPORT Datum getRRangeSP(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(((RangeSP *) PG_GETARG_POINTER(0))->r);
}
PG_FUNCTION_INFO_V1(getRRangeSP);

PGDLLEXPORT Datum getCRangeSP(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(&((RangeSP *) PG_GETARG_POINTER(0))->c);
}
PG_FUNCTION_INFO_V1(getCRangeSP);

PGDLLEXPORT Datum getXRangeSP(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(((RangeSP *) PG_GETARG_POINTER(0))->c.x);
}
PG_FUNCTION_INFO_V1(getXRangeSP);

PGDLLEXPORT Datum getYRangeSP(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(((RangeSP *) PG_GETARG_POINTER(0))->c.y);
}
PG_FUNCTION_INFO_V1(getYRangeSP);
