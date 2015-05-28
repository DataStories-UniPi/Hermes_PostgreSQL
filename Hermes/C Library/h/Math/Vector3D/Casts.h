/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_VECTOR3D_CASTS_H_
#define	HE_VECTOR3D_CASTS_H_

#include "Definition.h"
#include "../../SpatioTemporal/PointST/Definition.h"

PointST *Vector3D2PointST(const Vector3D *vector, PointST *result);
PGDLLEXPORT Datum Vector3D2PointSTV1(PG_FUNCTION_ARGS);

#endif
