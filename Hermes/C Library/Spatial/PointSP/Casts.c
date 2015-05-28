/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/Spatial/PointSP/Casts.h"

PointXY *PointSP2PointXY(const PointSP *point, PointXY *result) {
	PointXY *ret = result ? result : (PointXY *) palloc(sizeof(*ret));

	ret->x = point->x;
	ret->y = point->y;

	return ret;
}

PGDLLEXPORT Datum PointSP2PointXYV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(PointSP2PointXY((PointSP *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(PointSP2PointXYV1);

BoxSP *PointSP2BoxSP(const PointSP *point, BoxSP *result) {
	BoxSP *ret = result ? result : (BoxSP *) palloc(sizeof(*ret));

	ret->l.x = point->x;
	ret->l.y = point->y;
	ret->h.x = point->x;
	ret->h.y = point->y;

	return ret;
}

PGDLLEXPORT Datum PointSP2BoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(PointSP2BoxSP((PointSP *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(PointSP2BoxSPV1);

Vector2D *PointSP2Vector2D(const PointSP *point, Vector2D *result) {
	Vector2D *ret = result ? result : (Vector2D *) palloc(sizeof(*ret));

	ret->x = point->x;
	ret->y = point->y;

	return ret;
}

PGDLLEXPORT Datum PointSP2Vector2DV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(PointSP2Vector2D((PointSP *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(PointSP2Vector2DV1);
