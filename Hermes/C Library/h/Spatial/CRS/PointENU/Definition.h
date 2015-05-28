/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_POINTENU_DEFINITION_H_
#define	HE_POINTENU_DEFINITION_H_

#include <postgres.h>
#include <fmgr.h>

typedef struct {
	float8 e;
	float8 n;
	float8 u;
} PointENU;

PointENU *clonePointENU(const PointENU *point, PointENU *result);
PGDLLEXPORT Datum clonePointENUV1(PG_FUNCTION_ARGS);

PointENU *constructorPointENU(float8 e, float8 n, float8 u, PointENU *result);
PGDLLEXPORT Datum constructorPointENUV1(PG_FUNCTION_ARGS);

PointENU *inPointENU(const char *str, PointENU *result, char **error);
PGDLLEXPORT Datum inPointENUV1(PG_FUNCTION_ARGS);

char *outPointENU(const PointENU *point);
PGDLLEXPORT Datum outPointENUV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum recvPointENU(PG_FUNCTION_ARGS);

bytea *PointENU2Bytea(const PointENU *point);
PGDLLEXPORT Datum sendPointENU(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum hashPointENU(PG_FUNCTION_ARGS);

bool equalsPointENU(const PointENU *pointA, const PointENU *pointB);
PGDLLEXPORT Datum equalsPointENUV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum getEPointENU(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getNPointENU(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getUPointENU(PG_FUNCTION_ARGS);

#endif
