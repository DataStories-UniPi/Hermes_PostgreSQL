/*
* @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the PointSP data type.
 *
 * @see @ref PointSP
 * @see @ref data_types_spatial
 */

#ifndef HE_POINTSP_CASTS_H_
#define	HE_POINTSP_CASTS_H_

#include "Definition.h"
#include "../CRS/PointXY/Definition.h"
#include "../BoxSP/Definition.h"
#include "../../Math/Vector2D/Definition.h"

PointXY *PointSP2PointXY(const PointSP *point, PointXY *result);
PGDLLEXPORT Datum PointSP2PointXYV1(PG_FUNCTION_ARGS);

BoxSP *PointSP2BoxSP(const PointSP *point, BoxSP *result);
PGDLLEXPORT Datum PointSP2BoxSPV1(PG_FUNCTION_ARGS);

Vector2D *PointSP2Vector2D(const PointSP *point, Vector2D *result);
PGDLLEXPORT Datum PointSP2Vector2DV1(PG_FUNCTION_ARGS);

#endif
