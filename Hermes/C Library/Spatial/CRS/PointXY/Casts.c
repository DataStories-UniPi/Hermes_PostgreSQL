/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../../h/Spatial/CRS/PointXY/Casts.h"
#include "../../../h/Utilities/MSVCWorkarounds.h"
#include <math.h>

PointSP *PointXY2PointSP(const PointXY *point, PointSP *result) {
	PointSP *ret = result ? result : (PointSP *) palloc(sizeof(*ret));

	ret->x = (int32) round(point->x);
	ret->y = (int32) round(point->y);

	return ret;
}

PGDLLEXPORT Datum PointXY2PointSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(PointXY2PointSP((PointXY *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(PointXY2PointSPV1);

Vector2D *PointXY2Vector2D(const PointXY *point, Vector2D *result) {
	Vector2D *ret = result ? result : (Vector2D *) palloc(sizeof(*ret));

	ret->x = point->x;
	ret->y = point->y;

	return ret;
}

PGDLLEXPORT Datum PointXY2Vector2DV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(PointXY2Vector2D((PointXY *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(PointXY2Vector2DV1);
