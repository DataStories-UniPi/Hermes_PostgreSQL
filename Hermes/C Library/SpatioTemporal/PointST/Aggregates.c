/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/SpatioTemporal/PointST/Definition.h"

PGDLLEXPORT Datum minTPointST_PointST(PG_FUNCTION_ARGS) {
	PointST *pointA = (PointST *) PG_GETARG_POINTER(0);
	PointST *pointB = (PointST *) PG_GETARG_POINTER(1);

	if (pointB->t < pointA->t)
		PG_RETURN_POINTER(pointB);
	else
		PG_RETURN_POINTER(pointA);
}
PG_FUNCTION_INFO_V1(minTPointST_PointST);

PGDLLEXPORT Datum maxTPointST_PointST(PG_FUNCTION_ARGS) {
	PointST *pointA = (PointST *) PG_GETARG_POINTER(0);
	PointST *pointB = (PointST *) PG_GETARG_POINTER(1);

	if (pointB->t > pointA->t)
		PG_RETURN_POINTER(pointB);
	else
		PG_RETURN_POINTER(pointA);
}
PG_FUNCTION_INFO_V1(maxTPointST_PointST);
