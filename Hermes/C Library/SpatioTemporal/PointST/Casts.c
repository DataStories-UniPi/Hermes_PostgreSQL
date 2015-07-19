/**
 *	@file
 *	@author Marios Vodas (mvodas@gmail.com).
 *	@brief The implementation of PointST
 *
 *	@details These functions are documented in the @ref PointST/Casts.h
 *
 */

#include "../../h/SpatioTemporal/PointST/Casts.h"
#include "../../h/Utilities/Utilities.h"

BoxST *PointST2BoxST(const PointST *point, BoxST *result) {
	BoxST *ret = result ? result : (BoxST *) palloc(sizeof(*ret));

	ret->t.i = point->t;
	ret->t.e = point->t;
	ret->sp.l.x = point->sp.x;
	ret->sp.l.y = point->sp.y;
	ret->sp.h.x = point->sp.x;
	ret->sp.h.y = point->sp.y;

	return ret;
}

PGDLLEXPORT Datum PointST2BoxSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(PointST2BoxST((PointST *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(PointST2BoxSTV1);

Vector3D *PointST2Vector3D(const PointST *point, Vector3D *result) {
	Vector3D *ret = result ? result : (Vector3D *) palloc(sizeof(*ret));

	ret->x = point->sp.x;
	ret->y = point->sp.y;
	ret->z = timestamp2seconds(point->t);

	return ret;
}

PGDLLEXPORT Datum PointST2Vector3DV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(PointST2Vector3D((PointST *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(PointST2Vector3DV1);
