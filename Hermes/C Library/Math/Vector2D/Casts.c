/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/Math/Vector2D/Casts.h"
#include "../../h/Utilities/MSVCWorkarounds.h"
#include <math.h>

PointSP *Vector2D2PointSP(const Vector2D *vector, PointSP *result) {
	PointSP *ret = result ? result : (PointSP *) palloc(sizeof(*ret));

	ret->x = (int32) round(vector->x);
	ret->y = (int32) round(vector->y);

	return ret;
}

PGDLLEXPORT Datum Vector2D2PointSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(Vector2D2PointSP((Vector2D *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(Vector2D2PointSPV1);
