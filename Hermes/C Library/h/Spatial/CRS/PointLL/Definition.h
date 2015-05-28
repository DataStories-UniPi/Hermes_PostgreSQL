/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_POINTLL_DEFINITION_H_
#define	HE_POINTLL_DEFINITION_H_

#include <postgres.h>
#include <fmgr.h>

typedef struct {
	float8 lon;
	float8 lat;
} PointLL;

PointLL *clonePointLL(const PointLL *point, PointLL *result);
PGDLLEXPORT Datum clonePointLLV1(PG_FUNCTION_ARGS);

PointLL *constructorPointLL(float8 lon, float8 lat, PointLL *result, char **error);
PGDLLEXPORT Datum constructorPointLLV1(PG_FUNCTION_ARGS);

PointLL *inPointLL(const char *str, PointLL *result, char **error);
PGDLLEXPORT Datum inPointLLV1(PG_FUNCTION_ARGS);

char *outPointLL(const PointLL *point);
PGDLLEXPORT Datum outPointLLV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum recvPointLL(PG_FUNCTION_ARGS);

bytea *PointLL2Bytea(const PointLL *point);
PGDLLEXPORT Datum sendPointLL(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum hashPointLL(PG_FUNCTION_ARGS);

bool equalsPointLL(const PointLL *pointA, const PointLL *pointB);
PGDLLEXPORT Datum equalsPointLLV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum getLonPointLL(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getLatPointLL(PG_FUNCTION_ARGS);

#endif
