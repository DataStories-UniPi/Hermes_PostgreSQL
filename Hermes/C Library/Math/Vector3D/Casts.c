/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/Math/Vector3D/Casts.h"
#include "../../h/Utilities/Utilities.h"
#include "../../h/Utilities/MSVCWorkarounds.h"
#include <math.h>

PointST *Vector3D2PointST(const Vector3D *vector, PointST *result) {
	PointST *ret = result ? result : (PointST *) palloc(sizeof(*ret));

	ret->t = seconds2timestamp(vector->z);
	ret->sp.x = (int32) round(vector->x);
	ret->sp.y = (int32) round(vector->y);

	return ret;
}

PGDLLEXPORT Datum Vector3D2PointSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(Vector3D2PointST((Vector3D *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(Vector3D2PointSTV1);
