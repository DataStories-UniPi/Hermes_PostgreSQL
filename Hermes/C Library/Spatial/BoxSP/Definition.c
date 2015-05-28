/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/Spatial/BoxSP/Definition.h"
#include "../../h/Utilities/Utilities.h"
#include <libpq/pqformat.h>
#include <access/hash.h>

BoxSP *cloneBoxSP(const BoxSP *box, BoxSP *result) {
	BoxSP *ret = result ? result : (BoxSP *) palloc(sizeof(*ret));

	*ret = *box;

	return ret;
}

PGDLLEXPORT Datum cloneBoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(cloneBoxSP((BoxSP *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(cloneBoxSPV1);

BoxSP *constructorBoxSP(int32 lx, int32 ly, int32 hx, int32 hy, BoxSP *result, char **error) {
	BoxSP *ret = NULL;

	if (error)
		*error = NULL;

	if (lx > hx || ly > hy) {
		if (error)
			*error = "invalid geometry for BoxSP";
		return NULL ;
	}

	ret = result ? result : (BoxSP *) palloc(sizeof(*ret));

	ret->l.x = lx;
	ret->l.y = ly;
	ret->h.x = hx;
	ret->h.y = hy;

	return ret;
}

PGDLLEXPORT Datum constructorBoxSPV1(PG_FUNCTION_ARGS) {
	char *error;
	BoxSP *result = constructorBoxSP(PG_GETARG_INT32(0), PG_GETARG_INT32(1), PG_GETARG_INT32(2), PG_GETARG_INT32(3), NULL, &error);

	if (error)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("%s", error)));

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(constructorBoxSPV1);

BoxSP *constructorHighBoxSP(const PointSP *l, const PointSP *h, BoxSP *result, char **error) {
	BoxSP *ret = NULL;

	if (error)
		*error = NULL;

	if (l->x > h->x || l->y > h->y) {
		if (error)
			*error = "invalid geometry for BoxSP";
		return NULL ;
	}

	ret = result ? result : (BoxSP *) palloc(sizeof(*ret));

	ret->l = *l;
	ret->h = *h;

	return ret;
}

PGDLLEXPORT Datum constructorHighBoxSPV1(PG_FUNCTION_ARGS) {
	char *error;
	BoxSP *result = constructorHighBoxSP((PointSP *) PG_GETARG_POINTER(0), (PointSP *) PG_GETARG_POINTER(1), NULL, &error);

	if (error)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("%s", error)));

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(constructorHighBoxSPV1);

BoxSP *inBoxSP(const char *str, BoxSP *result, char **error) {
	int32 lx, ly, hx, hy;
	BoxSP *ret = NULL;

	if (error)
		*error = NULL;

	if (sscanf(str, " %d %d %d %d ", &lx, &ly, &hx, &hy) != 4) {
		if (error)
			asprintfHermes(error, "invalid input syntax for BoxSP: \"%s\"", str);
		return NULL ;
	}
	if (lx > hx || ly > hy) {
		if (error)
			*error = "invalid geometry for BoxSP";
		return NULL ;
	}

	ret = result ? result : (BoxSP *) palloc(sizeof(*ret));

	ret->l.x = lx;
	ret->l.y = ly;
	ret->h.x = hx;
	ret->h.y = hy;

	return ret;
}

PGDLLEXPORT Datum inBoxSPV1(PG_FUNCTION_ARGS) {
	char *error;
	BoxSP *result = inBoxSP(PG_GETARG_CSTRING(0), NULL, &error);

	if (error)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("%s", error)));

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(inBoxSPV1);

char *outBoxSP(const BoxSP *box) {
	char *result = NULL;

	asprintfHermes(&result, "%d %d %d %d", box->l.x, box->l.y, box->h.x, box->h.y);

	return result;
}

PGDLLEXPORT Datum outBoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_CSTRING(outBoxSP((BoxSP *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(outBoxSPV1);

PGDLLEXPORT Datum recvBoxSP(PG_FUNCTION_ARGS) {
	StringInfo msg = (StringInfo) PG_GETARG_POINTER(0);
	BoxSP *result = (BoxSP *) palloc(sizeof(*result));

	result->l.x = pq_getmsgint(msg, sizeof(result->l.x));
	result->l.y = pq_getmsgint(msg, sizeof(result->l.y));
	result->h.x = pq_getmsgint(msg, sizeof(result->h.x));
	result->h.y = pq_getmsgint(msg, sizeof(result->h.y));

	if (result->l.x > result->h.x || result->l.y > result->h.y) {
		pfree(result);
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("invalid geometry for BoxSP")));
	}

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(recvBoxSP);

bytea *BoxSP2Bytea(const BoxSP *box) {
	StringInfoData buf;

	pq_begintypsend(&buf);

	pq_sendint(&buf, box->l.x, sizeof(box->l.x));
	pq_sendint(&buf, box->l.y, sizeof(box->l.y));
	pq_sendint(&buf, box->h.x, sizeof(box->h.x));
	pq_sendint(&buf, box->h.y, sizeof(box->h.y));

	return pq_endtypsend(&buf);
}

PGDLLEXPORT Datum sendBoxSP(PG_FUNCTION_ARGS) {
	PG_RETURN_BYTEA_P(BoxSP2Bytea((BoxSP *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(sendBoxSP);

PGDLLEXPORT Datum hashBoxSP(PG_FUNCTION_ARGS) {
	PG_RETURN_DATUM(DirectFunctionCall1(hashvarlena, PointerGetDatum(BoxSP2Bytea((BoxSP *) PG_GETARG_POINTER(0)))));
}
PG_FUNCTION_INFO_V1(hashBoxSP);

bool equalsBoxSP(const BoxSP *boxA, const BoxSP *boxB) {
	return boxA->l.x == boxB->l.x && boxA->l.y == boxB->l.y && boxA->h.x == boxB->h.x && boxA->h.y == boxB->h.y;
}

PGDLLEXPORT Datum equalsBoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(equalsBoxSP((BoxSP *) PG_GETARG_POINTER(0),(BoxSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(equalsBoxSPV1);

PGDLLEXPORT Datum getLBoxSP(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(&((BoxSP *) PG_GETARG_POINTER(0))->l);
}
PG_FUNCTION_INFO_V1(getLBoxSP);

PGDLLEXPORT Datum getLxBoxSP(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(((BoxSP *) PG_GETARG_POINTER(0))->l.x);
}
PG_FUNCTION_INFO_V1(getLxBoxSP);

PGDLLEXPORT Datum getLyBoxSP(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(((BoxSP *) PG_GETARG_POINTER(0))->l.y);
}
PG_FUNCTION_INFO_V1(getLyBoxSP);

PGDLLEXPORT Datum getHBoxSP(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(&((BoxSP *) PG_GETARG_POINTER(0))->h);
}
PG_FUNCTION_INFO_V1(getHBoxSP);

PGDLLEXPORT Datum getHxBoxSP(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(((BoxSP *) PG_GETARG_POINTER(0))->h.x);
}
PG_FUNCTION_INFO_V1(getHxBoxSP);

PGDLLEXPORT Datum getHyBoxSP(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(((BoxSP *) PG_GETARG_POINTER(0))->h.y);
}
PG_FUNCTION_INFO_V1(getHyBoxSP);
