/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_CONVERSIONS_H_
#define HE_CONVERSIONS_H_

#include "PointLL/Definition.h"
#include "PointPLH/Definition.h"
#include "PointXYZ/Definition.h"
#include "PointENU/Definition.h"
#include "PointXY/Definition.h"

PointXYZ *plh2xyz(const PointPLH *point, PointXYZ *result);
PointPLH *xyz2plh(const PointXYZ *point, PointPLH *result);
PointENU *xyz2enu(const PointXYZ *point, const PointPLH *lrp, PointENU *result);
PointXYZ *enu2xyz(const PointENU *point, const PointPLH *lrp, PointXYZ *result);
PointENU *plh2enu(const PointPLH *point, const PointPLH *lrp, PointENU *result);
PointPLH *enu2plh(const PointENU *point, const PointPLH *lrp, PointPLH *result);
PointXY *ll2xy(const PointLL *point, const PointLL *lrp, PointXY *result);
PointLL *xy2ll(const PointXY *point, const PointLL *lrp, PointLL *result);

PGDLLEXPORT Datum plh2xyzV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum xyz2plhV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum xyz2enuV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum enu2xyzV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum plh2enuV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum enu2plhV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum ll2xyV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum xy2llV1(PG_FUNCTION_ARGS);

#endif
