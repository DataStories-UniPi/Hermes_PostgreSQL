/**
 *	@file
 *	@author Marios Vodas (mvodas@gmail.com).
 *	@brief The implementation of BoxSP
 *
 *	@details These functions are documented in the @ref BoxSP/Interactions.h
 *
 */

#include "../../h/Spatial/BoxSP/Interactions.h"

BoxSP *intersectionBoxSP_BoxSP(const BoxSP *boxA, const BoxSP *boxB, BoxSP *result) {
	int32 lx, ly, hx, hy;
	BoxSP *ret = NULL;

	lx = Max(boxA->l.x, boxB->l.x);
	ly = Max(boxA->l.y, boxB->l.y);
	hx = Min(boxA->h.x, boxB->h.x);
	hy = Min(boxA->h.y, boxB->h.y);

	if (lx > hx || ly > hy) {
		return NULL ;
	}

	ret = result ? result : (BoxSP *) palloc(sizeof(*ret));

	ret->l.x = lx;
	ret->l.y = ly;
	ret->h.x = hx;
	ret->h.y = hy;

	return ret;
}

PGDLLEXPORT Datum intersectionBoxSP_BoxSPV1(PG_FUNCTION_ARGS) {
	BoxSP *result = intersectionBoxSP_BoxSP((BoxSP *) PG_GETARG_POINTER(0), (BoxSP *) PG_GETARG_POINTER(1), NULL );

	if (result == NULL )
		PG_RETURN_NULL()
		;

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(intersectionBoxSP_BoxSPV1);

BoxSP *unionBoxSP_PointSP(const BoxSP *box, const PointSP *point, BoxSP *result) {
	BoxSP *ret = result ? result : (BoxSP *) palloc(sizeof(*ret));

	ret->l.x = Min(box->l.x, point->x);
	ret->l.y = Min(box->l.y, point->y);
	ret->h.x = Max(box->h.x, point->x);
	ret->h.y = Max(box->h.y, point->y);

	return ret;
}

PGDLLEXPORT Datum unionBoxSP_PointSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(unionBoxSP_PointSP((BoxSP *) PG_GETARG_POINTER(0), (PointSP *) PG_GETARG_POINTER(1), NULL));
}
PG_FUNCTION_INFO_V1(unionBoxSP_PointSPV1);

BoxSP *unionBoxSP_SegmentSP(const BoxSP *box, const SegmentSP *segment, BoxSP *result) {
	BoxSP *ret = result ? result : (BoxSP *) palloc(sizeof(*ret));

	ret->l.x = Min(box->l.x, Min(segment->i.x, segment->e.x));
	ret->l.y = Min(box->l.y, Min(segment->i.y, segment->e.y));
	ret->h.x = Max(box->h.x, Max(segment->i.x, segment->e.x));
	ret->h.y = Max(box->h.y, Max(segment->i.y, segment->e.y));

	return ret;
}

PGDLLEXPORT Datum unionBoxSP_SegmentSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(unionBoxSP_SegmentSP((BoxSP *) PG_GETARG_POINTER(0), (SegmentSP *) PG_GETARG_POINTER(1), NULL));
}
PG_FUNCTION_INFO_V1(unionBoxSP_SegmentSPV1);

BoxSP *unionBoxSP_BoxSP(const BoxSP *boxA, const BoxSP *boxB, BoxSP *result) {
	BoxSP *ret = result ? result : (BoxSP *) palloc(sizeof(*ret));

	ret->l.x = Min(boxA->l.x, boxB->l.x);
	ret->l.y = Min(boxA->l.y, boxB->l.y);
	ret->h.x = Max(boxA->h.x, boxB->h.x);
	ret->h.y = Max(boxA->h.y, boxB->h.y);

	return ret;
}

PGDLLEXPORT Datum unionBoxSP_BoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(unionBoxSP_BoxSP((BoxSP *) PG_GETARG_POINTER(0), (BoxSP *) PG_GETARG_POINTER(1), NULL));
}
PG_FUNCTION_INFO_V1(unionBoxSP_BoxSPV1);
