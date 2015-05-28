/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/Spatial/PointSP/RelationalOperators.h"

bool northPointSP_PointSP(const PointSP *pointA, const PointSP *pointB) {
	return pointA->y > pointB->y;
}

PGDLLEXPORT Datum northPointSP_PointSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(northPointSP_PointSP((PointSP *) PG_GETARG_POINTER(0), (PointSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(northPointSP_PointSPV1);

bool southPointSP_PointSP(const PointSP *pointA, const PointSP *pointB) {
	return pointA->y < pointB->y;
}

PGDLLEXPORT Datum southPointSP_PointSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(southPointSP_PointSP((PointSP *) PG_GETARG_POINTER(0), (PointSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(southPointSP_PointSPV1);

bool eastPointSP_PointSP(const PointSP *pointA, const PointSP *pointB) {
	return pointA->x > pointB->x;
}

PGDLLEXPORT Datum eastPointSP_PointSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(eastPointSP_PointSP((PointSP *) PG_GETARG_POINTER(0), (PointSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(eastPointSP_PointSPV1);

bool westPointSP_PointSP(const PointSP *pointA, const PointSP *pointB) {
	return pointA->x < pointB->x;
}

PGDLLEXPORT Datum westPointSP_PointSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(westPointSP_PointSP((PointSP *) PG_GETARG_POINTER(0), (PointSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(westPointSP_PointSPV1);
