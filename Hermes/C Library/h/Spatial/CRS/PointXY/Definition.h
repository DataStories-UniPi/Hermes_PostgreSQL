/*
* @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the PointXY data type.
 *
 * @see @ref PointXY
 */

#ifndef HE_POINTXY_DEFINITION_H_
#define	HE_POINTXY_DEFINITION_H_

#include <postgres.h>
#include <fmgr.h>

  /**
  * PointXY is auxiliary spatial data type which is
  * used to represent points in topocentric coordinates
  * so that we can later transform them to eyclidean space
  */
typedef struct {
	float8 x; /**< the x coordinate */
	float8 y; /**< the y coordinate */
} PointXY;

PointXY *clonePointXY(const PointXY *point, PointXY *result);
PGDLLEXPORT Datum clonePointXYV1(PG_FUNCTION_ARGS);

PointXY *constructorPointXY(float8 x, float8 y, PointXY *result);
PGDLLEXPORT Datum constructorPointXYV1(PG_FUNCTION_ARGS);

PointXY *inPointXY(const char *str, PointXY *result, char **error);
PGDLLEXPORT Datum inPointXYV1(PG_FUNCTION_ARGS);

char *outPointXY(const PointXY *point);
PGDLLEXPORT Datum outPointXYV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum recvPointXY(PG_FUNCTION_ARGS);

bytea *PointXY2Bytea(const PointXY *point);
PGDLLEXPORT Datum sendPointXY(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum hashPointXY(PG_FUNCTION_ARGS);

bool equalsPointXY(const PointXY *pointA, const PointXY *pointB);
PGDLLEXPORT Datum equalsPointXYV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum getXPointXY(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getYPointXY(PG_FUNCTION_ARGS);

#endif
