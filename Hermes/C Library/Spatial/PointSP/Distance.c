/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/Spatial/PointSP/Distance.h"
#include <math.h>

float8 sqDistancePointSP_PointSP(const PointSP *pointA, const PointSP *pointB) {
	return pow(pointA->x - pointB->x, 2.0) + pow(pointA->y - pointB->y, 2.0);
}

float8 distancePointSP_PointSP(const PointSP *pointA, const PointSP *pointB) {
	return sqrt(pow(pointA->x - pointB->x, 2.0) + pow(pointA->y - pointB->y, 2.0));
}

PGDLLEXPORT Datum distancePointSP_PointSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(distancePointSP_PointSP((PointSP *) PG_GETARG_POINTER(0), (PointSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(distancePointSP_PointSPV1);

float8 distanceManhattanPointSP_PointSP(const PointSP *pointA, const PointSP *pointB) {
	return abs(pointA->x - pointB->x) + abs(pointA->y - pointB->y);
}

PGDLLEXPORT Datum distanceManhattanPointSP_PointSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(distanceManhattanPointSP_PointSP((PointSP *) PG_GETARG_POINTER(0), (PointSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(distanceManhattanPointSP_PointSPV1);

float8 distanceChebyshevPointSP_PointSP(const PointSP *pointA, const PointSP *pointB) {
	int32 Dx, Dy;

	Dx = abs(pointA->x - pointB->x);
	Dy = abs(pointA->y - pointB->y);

	return Max(Dx, Dy);
}

PGDLLEXPORT Datum distanceChebyshevPointSP_PointSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(distanceChebyshevPointSP_PointSP((PointSP *) PG_GETARG_POINTER(0), (PointSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(distanceChebyshevPointSP_PointSPV1);
