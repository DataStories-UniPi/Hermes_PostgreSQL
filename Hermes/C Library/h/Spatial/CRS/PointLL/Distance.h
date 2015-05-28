/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_POINTLL_DISTANCE_H_
#define HE_POINTLL_DISTANCE_H_

#include "Definition.h"

float8 distanceSpherePointLL_PointLL(const PointLL *pointA, const PointLL *pointB);
PGDLLEXPORT Datum distanceSpherePointLL_PointLLV1(PG_FUNCTION_ARGS);

#endif
