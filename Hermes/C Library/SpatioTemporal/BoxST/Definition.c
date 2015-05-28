/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/SpatioTemporal/BoxST/Definition.h"
#include "../../h/Utilities/Utilities.h"
#include <libpq/pqformat.h>
#include <access/hash.h>

BoxST *cloneBoxST(const BoxST *box, BoxST *result) {
	BoxST *ret = result ? result : (BoxST *) palloc(sizeof(*ret));

	*ret = *box;

	return ret;
}

PGDLLEXPORT Datum cloneBoxSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(cloneBoxST((BoxST *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(cloneBoxSTV1);

BoxST *constructorBoxST(Timestamp ti, Timestamp te, int32 lx, int32 ly, int32 hx, int32 hy, BoxST *result, char **error) {
	BoxST *ret = NULL;

	if (error)
		*error = NULL;

	if (ti > te || lx > hx || ly > hy) {
		if (error)
			*error = "invalid time sequence or geometry for BoxST";
		return NULL ;
	}

	ret = result ? result : (BoxST *) palloc(sizeof(*ret));

	ret->t.i = ti;
	ret->t.e = te;
	ret->sp.l.x = lx;
	ret->sp.l.y = ly;
	ret->sp.h.x = hx;
	ret->sp.h.y = hy;

	return ret;
}

PGDLLEXPORT Datum constructorBoxSTV1(PG_FUNCTION_ARGS) {
	char *error;
	BoxST *result = constructorBoxST(PG_GETARG_TIMESTAMP(0), PG_GETARG_TIMESTAMP(1), PG_GETARG_INT32(2), PG_GETARG_INT32(3), PG_GETARG_INT32(4), PG_GETARG_INT32(5), NULL, &error);

	if (error)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("%s", error)));

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(constructorBoxSTV1);

BoxST *constructorHighBoxST(const Period *t, const BoxSP *sp, BoxST *result, char **error) {
	BoxST *ret = NULL;

	if (error)
		*error = NULL;

	if (t->i > t->e || sp->l.x > sp->h.x || sp->l.y > sp->h.y) {
		if (error)
			*error = "invalid time sequence or geometry for BoxST";
		return NULL ;
	}

	ret = result ? result : (BoxST *) palloc(sizeof(*ret));

	ret->t = *t;
	ret->sp = *sp;

	return ret;
}

PGDLLEXPORT Datum constructorHighBoxSTV1(PG_FUNCTION_ARGS) {
	char *error;
	BoxST *result = constructorHighBoxST((Period *) PG_GETARG_POINTER(0), (BoxSP *) PG_GETARG_POINTER(1), NULL, &error);

	if (error)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("%s", error)));

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(constructorHighBoxSTV1);

BoxST *inBoxST(const char *str, BoxST *result, char **error) {
	char tis[27], tes[27];
	Timestamp ti, te;
	int32 lx, ly, hx, hy;
	BoxST *ret = NULL;

	if (error)
		*error = NULL;

	if (sscanf(str, " '%26[^']' '%26[^']' %d %d %d %d ", tis, tes, &lx, &ly, &hx, &hy) != 6) {
		if (error)
			asprintfHermes(error, "invalid input syntax for BoxST: \"%s\"", str);
		return NULL ;
	}

	ti = DatumGetTimestamp(DirectFunctionCall3(timestamp_in, CStringGetDatum(tis), PointerGetDatum(0), Int32GetDatum(-1)));
	te = DatumGetTimestamp(DirectFunctionCall3(timestamp_in, CStringGetDatum(tes), PointerGetDatum(0), Int32GetDatum(-1)));

	if (ti > te || lx > hx || ly > hy) {
		if (error)
			*error = "invalid time sequence or geometry for BoxST";
		return NULL ;
	}

	ret = result ? result : (BoxST *) palloc(sizeof(*ret));

	ret->t.i = ti;
	ret->t.e = te;
	ret->sp.l.x = lx;
	ret->sp.l.y = ly;
	ret->sp.h.x = hx;
	ret->sp.h.y = hy;

	return ret;
}

PGDLLEXPORT Datum inBoxSTV1(PG_FUNCTION_ARGS) {
	char *error;
	BoxST *result = inBoxST(PG_GETARG_CSTRING(0), NULL, &error);

	if (error)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("%s", error)));

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(inBoxSTV1);

char *outBoxST(const BoxST *box) {
	char *result = NULL;

	asprintfHermes(&result, "'%s' '%s' %d %d %d %d", DatumGetCString(DirectFunctionCall1(timestamp_out, TimestampGetDatum(box->t.i))), DatumGetCString(DirectFunctionCall1(timestamp_out, TimestampGetDatum(box->t.e))), box->sp.l.x, box->sp.l.y, box->sp.h.x, box->sp.h.y);

	return result;
}

PGDLLEXPORT Datum outBoxSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_CSTRING(outBoxST((BoxST *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(outBoxSTV1);

PGDLLEXPORT Datum recvBoxST(PG_FUNCTION_ARGS) {
	StringInfo msg = (StringInfo) PG_GETARG_POINTER(0);
	BoxST *result = (BoxST *) palloc(sizeof(*result));

	result->t.i = getmsgTimestampHermes(msg);
	result->t.e = getmsgTimestampHermes(msg);
	result->sp.l.x = pq_getmsgint(msg, sizeof(result->sp.l.x));
	result->sp.l.y = pq_getmsgint(msg, sizeof(result->sp.l.y));
	result->sp.h.x = pq_getmsgint(msg, sizeof(result->sp.h.x));
	result->sp.h.y = pq_getmsgint(msg, sizeof(result->sp.h.y));

	if (result->t.i > result->t.e || result->sp.l.x > result->sp.h.x || result->sp.l.y > result->sp.h.y) {
		pfree(result);
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("invalid time sequence or geometry for BoxST")));
	}

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(recvBoxST);

bytea *BoxST2Bytea(const BoxST *box) {
	StringInfoData buf;

	pq_begintypsend(&buf);

	sendTimestampHermes(&buf, box->t.i);
	sendTimestampHermes(&buf, box->t.e);
	pq_sendint(&buf, box->sp.l.x, sizeof(box->sp.l.x));
	pq_sendint(&buf, box->sp.l.y, sizeof(box->sp.l.y));
	pq_sendint(&buf, box->sp.h.x, sizeof(box->sp.h.x));
	pq_sendint(&buf, box->sp.h.y, sizeof(box->sp.h.y));

	return pq_endtypsend(&buf);
}

PGDLLEXPORT Datum sendBoxST(PG_FUNCTION_ARGS) {
	PG_RETURN_BYTEA_P(BoxST2Bytea((BoxST *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(sendBoxST);

PGDLLEXPORT Datum hashBoxST(PG_FUNCTION_ARGS) {
	PG_RETURN_DATUM(DirectFunctionCall1(hashvarlena, PointerGetDatum(BoxST2Bytea((BoxST *) PG_GETARG_POINTER(0)))));
}
PG_FUNCTION_INFO_V1(hashBoxST);

bool equalsBoxST(const BoxST *boxA, const BoxST *boxB) {
	return boxA->t.i == boxB->t.i && boxA->t.e == boxB->t.e && boxA->sp.l.x == boxB->sp.l.x && boxA->sp.l.y == boxB->sp.l.y && boxA->sp.h.x == boxB->sp.h.x && boxA->sp.h.y == boxB->sp.h.y;
}

PGDLLEXPORT Datum equalsBoxSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(equalsBoxST((BoxST *) PG_GETARG_POINTER(0),(BoxST *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(equalsBoxSTV1);

PGDLLEXPORT Datum getTBoxST(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(&((BoxST *) PG_GETARG_POINTER(0))->t);
}
PG_FUNCTION_INFO_V1(getTBoxST);

PGDLLEXPORT Datum getTiBoxST(PG_FUNCTION_ARGS) {
	PG_RETURN_TIMESTAMP(((BoxST *) PG_GETARG_POINTER(0))->t.i);
}
PG_FUNCTION_INFO_V1(getTiBoxST);

PGDLLEXPORT Datum getTeBoxST(PG_FUNCTION_ARGS) {
	PG_RETURN_TIMESTAMP(((BoxST *) PG_GETARG_POINTER(0))->t.e);
}
PG_FUNCTION_INFO_V1(getTeBoxST);

PGDLLEXPORT Datum getSpBoxST(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(&((BoxST *) PG_GETARG_POINTER(0))->sp);
}
PG_FUNCTION_INFO_V1(getSpBoxST);

PGDLLEXPORT Datum getLBoxST(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(&((BoxST *) PG_GETARG_POINTER(0))->sp.l);
}
PG_FUNCTION_INFO_V1(getLBoxST);

PGDLLEXPORT Datum getLxBoxST(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(((BoxST *) PG_GETARG_POINTER(0))->sp.l.x);
}
PG_FUNCTION_INFO_V1(getLxBoxST);

PGDLLEXPORT Datum getLyBoxST(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(((BoxST *) PG_GETARG_POINTER(0))->sp.l.y);
}
PG_FUNCTION_INFO_V1(getLyBoxST);

PGDLLEXPORT Datum getHBoxST(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(&((BoxST *) PG_GETARG_POINTER(0))->sp.h);
}
PG_FUNCTION_INFO_V1(getHBoxST);

PGDLLEXPORT Datum getHxBoxST(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(((BoxST *) PG_GETARG_POINTER(0))->sp.h.x);
}
PG_FUNCTION_INFO_V1(getHxBoxST);

PGDLLEXPORT Datum getHyBoxST(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(((BoxST *) PG_GETARG_POINTER(0))->sp.h.y);
}
PG_FUNCTION_INFO_V1(getHyBoxST);
