/*
* @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the conversions between data type.
 *
 * @see @ref PointLL
 * @see @ref PointSP
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

/** @brief Transforms the point from lon-lat coordinates to x,y
 *
 *	@relates PointXY
 *  @relates PointLL
 *
 *	@param[in] *point the point lon-lat coordinates
 *	@param[in] *lrp the reference point
 *	@param[out] *result I have no idea. We should ask marios
 *
 *	@return the point transformed to a PointXY
 */
PointXY *ll2xy(const PointLL *point, const PointLL *lrp, PointXY *result);

/** @brief Transforms the point from x-y coordinates to lon-lat
 *
 *  @relates PointLL
 *  @relates PointXY
 *
 *	@param[in] *point the point x-y coordinates
 *	@param[in] *lrp the reference point
 *	@param[out] *result I have no idea. We should ask marios
 *
 *	@return the point transformed to a PointLL
 */
PointLL *xy2ll(const PointXY *point, const PointLL *lrp, PointLL *result);

PGDLLEXPORT Datum plh2xyzV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum xyz2plhV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum xyz2enuV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum enu2xyzV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum plh2enuV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum enu2plhV1(PG_FUNCTION_ARGS);

/** @brief Transforms the point from lon-lat coordinates to x,y
 *
 *	@relates PointXY
 *  @relates PointLL
 *
 */
PGDLLEXPORT Datum ll2xyV1(PG_FUNCTION_ARGS);

/** @brief Transforms the point from x-y coordinates to lon-lat
 *
 *  @relates PointLL
 *  @relates PointXY
 *
 */
PGDLLEXPORT Datum xy2llV1(PG_FUNCTION_ARGS);

#endif
