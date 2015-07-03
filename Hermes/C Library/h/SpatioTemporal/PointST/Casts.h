/*
* @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the PointST data type.
 *
 * @see @ref PointST
 * @see @ref data_types_spatio_temp
 */

#ifndef HE_POINTST_CASTS_H_
#define HE_POINTST_CASTS_H_

#include "Definition.h"
#include "../BoxST/Definition.h"
#include "../../Math/Vector3D/Definition.h"

BoxST *PointST2BoxST(const PointST *point, BoxST *result);
PGDLLEXPORT Datum PointST2BoxSTV1(PG_FUNCTION_ARGS);

Vector3D *PointST2Vector3D(const PointST *point, Vector3D *result);
PGDLLEXPORT Datum PointST2Vector3DV1(PG_FUNCTION_ARGS);

#endif
