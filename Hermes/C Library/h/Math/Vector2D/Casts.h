/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_VECTOR2D_CASTS_H_
#define	HE_VECTOR2D_CASTS_H_

#include "Definition.h"
#include "../../Spatial/PointSP/Definition.h"

PointSP *Vector2D2PointSP(const Vector2D *vector, PointSP *result);
PGDLLEXPORT Datum Vector2D2PointSPV1(PG_FUNCTION_ARGS);

#endif
