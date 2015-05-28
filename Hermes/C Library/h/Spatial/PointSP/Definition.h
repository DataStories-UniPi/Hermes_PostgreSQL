/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_POINTSP_DEFINITION_H_
#define	HE_POINTSP_DEFINITION_H_

#include <postgres.h>
#include <fmgr.h>

typedef struct {
	int32 x;
	int32 y;
} PointSP;

PointSP *clonePointSP(const PointSP *point, PointSP *result);
PGDLLEXPORT Datum clonePointSPV1(PG_FUNCTION_ARGS);

PointSP *constructorPointSP(int32 x, int32 y, PointSP *result);
PGDLLEXPORT Datum constructorPointSPV1(PG_FUNCTION_ARGS);

PointSP *inPointSP(const char *str, PointSP *result, char **error);
PGDLLEXPORT Datum inPointSPV1(PG_FUNCTION_ARGS);

char *outPointSP(const PointSP *point);
PGDLLEXPORT Datum outPointSPV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum recvPointSP(PG_FUNCTION_ARGS);

bytea *PointSP2Bytea(const PointSP *point);
PGDLLEXPORT Datum sendPointSP(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum hashPointSP(PG_FUNCTION_ARGS);

bool equalsPointSP(const PointSP *pointA, const PointSP *pointB);
PGDLLEXPORT Datum equalsPointSPV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum getXPointSP(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getYPointSP(PG_FUNCTION_ARGS);

#endif
