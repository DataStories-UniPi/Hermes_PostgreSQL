/**
 *	@file
 *	@author Marios Vodas (mvodas@gmail.com).
 *	@brief The implementantion of BoxST
 *
 *	@details These functions are documented in the @ref BoxST/Interactions.h
 *
 */
#include "../../h/SpatioTemporal/BoxST/Interactions.h"

BoxST *intersectionBoxST_BoxST(const BoxST *boxA, const BoxST *boxB, BoxST *result) {
	Timestamp ti, te;
	int32 lx, ly, hx, hy;
	BoxST *ret = NULL;

	ti = Max(boxA->t.i, boxB->t.i);
	te = Min(boxA->t.e, boxB->t.e);
	lx = Max(boxA->sp.l.x, boxB->sp.l.x);
	ly = Max(boxA->sp.l.y, boxB->sp.l.y);
	hx = Min(boxA->sp.h.x, boxB->sp.h.x);
	hy = Min(boxA->sp.h.y, boxB->sp.h.y);

	if (ti > te || lx > hx || ly > hy) {
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

PGDLLEXPORT Datum intersectionBoxST_BoxSTV1(PG_FUNCTION_ARGS) {
	BoxST *result = intersectionBoxST_BoxST((BoxST *) PG_GETARG_POINTER(0), (BoxST *) PG_GETARG_POINTER(1), NULL );

	if (result == NULL )
		PG_RETURN_NULL()
		;

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(intersectionBoxST_BoxSTV1);

BoxST *unionBoxST_PointST(const BoxST *box, const PointST *point, BoxST *result) {
	BoxST *ret = result ? result : (BoxST *) palloc(sizeof(*ret));

	ret->t.i = Min(box->t.i, point->t);
	ret->t.e = Max(box->t.e, point->t);
	ret->sp.l.x = Min(box->sp.l.x, point->sp.x);
	ret->sp.l.y = Min(box->sp.l.y, point->sp.y);
	ret->sp.h.x = Max(box->sp.h.x, point->sp.x);
	ret->sp.h.y = Max(box->sp.h.y, point->sp.y);

	return ret;
}

PGDLLEXPORT Datum unionBoxST_PointSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(unionBoxST_PointST((BoxST *) PG_GETARG_POINTER(0), (PointST *) PG_GETARG_POINTER(1), NULL));
}
PG_FUNCTION_INFO_V1(unionBoxST_PointSTV1);

BoxST *unionBoxST_SegmentST(const BoxST *box, const SegmentST *segment, BoxST *result) {
	BoxST *ret = result ? result : (BoxST *) palloc(sizeof(*ret));

	ret->t.i = Min(box->t.i, segment->t.i);
	ret->t.e = Max(box->t.e, segment->t.e);
	ret->sp.l.x = Min(box->sp.l.x, Min(segment->sp.i.x, segment->sp.e.x));
	ret->sp.l.y = Min(box->sp.l.y, Min(segment->sp.i.y, segment->sp.e.y));
	ret->sp.h.x = Max(box->sp.h.x, Max(segment->sp.i.x, segment->sp.e.x));
	ret->sp.h.y = Max(box->sp.h.y, Max(segment->sp.i.y, segment->sp.e.y));

	return ret;
}

PGDLLEXPORT Datum unionBoxST_SegmentSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(unionBoxST_SegmentST((BoxST *) PG_GETARG_POINTER(0), (SegmentST *) PG_GETARG_POINTER(1), NULL));
}
PG_FUNCTION_INFO_V1(unionBoxST_SegmentSTV1);

BoxST *unionBoxST_BoxST(const BoxST *boxA, const BoxST *boxB, BoxST *result) {
	BoxST *ret = result ? result : (BoxST *) palloc(sizeof(*ret));

	ret->t.i = Min(boxA->t.i, boxB->t.i);
	ret->t.e = Max(boxA->t.e, boxB->t.e);
	ret->sp.l.x = Min(boxA->sp.l.x, boxB->sp.l.x);
	ret->sp.l.y = Min(boxA->sp.l.y, boxB->sp.l.y);
	ret->sp.h.x = Max(boxA->sp.h.x, boxB->sp.h.x);
	ret->sp.h.y = Max(boxA->sp.h.y, boxB->sp.h.y);

	return ret;
}

PGDLLEXPORT Datum unionBoxST_BoxSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(unionBoxST_BoxST((BoxST *) PG_GETARG_POINTER(0), (BoxST *) PG_GETARG_POINTER(1), NULL));
}
PG_FUNCTION_INFO_V1(unionBoxST_BoxSTV1);
