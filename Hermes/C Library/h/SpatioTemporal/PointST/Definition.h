/*
* @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the PointST data type.
 *
 * @see @ref PointST
 */

#ifndef HE_POINTST_DEFINITION_H_
#define	HE_POINTST_DEFINITION_H_

#include "../../Spatial/PointSP/Definition.h"
#include <utils/timestamp.h>

/**
 * PointST is a spatio-temporal data type and is
 * compromised of a Timestamp "t" and a PointSP "sp"
 * Consider it a point in time and space
 *
 */
typedef struct {
	Timestamp t; /**< the timestamp of the recorded point */
	PointSP sp; /**< the coordinates of the recorded point */
} PointST;

PointST *clonePointST(const PointST *point, PointST *result);
PGDLLEXPORT Datum clonePointSTV1(PG_FUNCTION_ARGS);

PointST *constructorPointST(Timestamp t, int32 x, int32 y, PointST *result);
PGDLLEXPORT Datum constructorPointSTV1(PG_FUNCTION_ARGS);

PointST *constructorHighPointST(Timestamp t, const PointSP *sp, PointST *result);
PGDLLEXPORT Datum constructorHighPointSTV1(PG_FUNCTION_ARGS);

PointST *inPointST(const char *str, PointST *result, char **error);
PGDLLEXPORT Datum inPointSTV1(PG_FUNCTION_ARGS);

char *outPointST(const PointST *point);
PGDLLEXPORT Datum outPointSTV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum recvPointST(PG_FUNCTION_ARGS);

bytea *PointST2Bytea(const PointST *point);
PGDLLEXPORT Datum sendPointST(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum hashPointST(PG_FUNCTION_ARGS);

bool equalsPointST(const PointST *pointA, const PointST *pointB);
PGDLLEXPORT Datum equalsPointSTV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum getTPointST(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getSpPointST(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getXPointST(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getYPointST(PG_FUNCTION_ARGS);

#endif
