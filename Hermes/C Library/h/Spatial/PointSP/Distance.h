/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_POINTSP_DISTANCE_H_
#define HE_POINTSP_DISTANCE_H_

#include "Definition.h"

float8 sqDistancePointSP_PointSP(const PointSP *pointA, const PointSP *pointB);

float8 distancePointSP_PointSP(const PointSP *pointA, const PointSP *pointB);
PGDLLEXPORT Datum distancePointSP_PointSPV1(PG_FUNCTION_ARGS);

float8 distanceManhattanPointSP_PointSP(const PointSP *pointA, const PointSP *pointB);
PGDLLEXPORT Datum distanceManhattanPointSP_PointSPV1(PG_FUNCTION_ARGS);

float8 distanceChebyshevPointSP_PointSP(const PointSP *pointA, const PointSP *pointB);
PGDLLEXPORT Datum distanceChebyshevPointSP_PointSPV1(PG_FUNCTION_ARGS);

#endif
