/**
 *	@file
 *	@author Marios Vodas (mvodas@gmail.com).
 *	@brief The implementantion of SegmentSP
 *
 *	@details These functions are documented in the @ref SegmentSP/Properties.h
 *
 */

#include "../../h/Spatial/SegmentSP/Properties.h"
#include "../../h/Spatial/PointSP/Casts.h"
#include "../../h/Math/Vector2D/ArithmeticOperators.h"
#include <math.h>

float8 lengthSegmentSP(const SegmentSP *segment) {
	return sqrt(pow(segment->e.x - segment->i.x, 2.0) + pow(segment->e.y - segment->i.y, 2.0));
}

Vector2D *directionSegmentSP(const SegmentSP *segment, Vector2D *result, float8 *norm) {
	Vector2D i, e;

	PointSP2Vector2D(&segment->i, &i);
	PointSP2Vector2D(&segment->e, &e);

	subtractionVector2D(&e, &i, &e);

	return normalizeVector2D(&e, result, norm);
}

PGDLLEXPORT Datum lengthSegmentSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(lengthSegmentSP((SegmentSP *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(lengthSegmentSPV1);

PGDLLEXPORT Datum directionSegmentSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(directionSegmentSP((SegmentSP *) PG_GETARG_POINTER(0), NULL, NULL));
}
PG_FUNCTION_INFO_V1(directionSegmentSPV1);

PointXY *centerSegmentSP(const SegmentSP *segment, PointXY *result) {
	PointXY *ret = result ? result : (PointXY *) palloc(sizeof(*ret));

	ret->x = (segment->i.x + segment->e.x) / 2.0;
	ret->y = (segment->i.y + segment->e.y) / 2.0;

	return ret;
}

float8 centerXSegmentSP(const SegmentSP *segment) {
	return (segment->i.x + segment->e.x) / 2.0;
}

float8 centerYSegmentSP(const SegmentSP *segment) {
	return (segment->i.y + segment->e.y) / 2.0;
}

PGDLLEXPORT Datum centerSegmentSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(centerSegmentSP((SegmentSP *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(centerSegmentSPV1);

PGDLLEXPORT Datum centerXSegmentSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(centerXSegmentSP((SegmentSP *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(centerXSegmentSPV1);

PGDLLEXPORT Datum centerYSegmentSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(centerYSegmentSP((SegmentSP *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(centerYSegmentSPV1);
