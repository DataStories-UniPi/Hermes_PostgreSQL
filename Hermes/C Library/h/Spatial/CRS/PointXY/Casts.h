/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_POINTXY_CASTS_H_
#define	HE_POINTXY_CASTS_H_

#include "Definition.h"
#include "../../PointSP/Definition.h"
#include "../../../Math/Vector2D/Definition.h"

PointSP *PointXY2PointSP(const PointXY *point, PointSP *result);
PGDLLEXPORT Datum PointXY2PointSPV1(PG_FUNCTION_ARGS);

Vector2D *PointXY2Vector2D(const PointXY *point, Vector2D *result);
PGDLLEXPORT Datum PointXY2Vector2DV1(PG_FUNCTION_ARGS);

#endif
