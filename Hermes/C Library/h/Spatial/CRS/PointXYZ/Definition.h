/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_POINTXYZ_DEFINITION_H_
#define	HE_POINTXYZ_DEFINITION_H_

#include <postgres.h>
#include <fmgr.h>

typedef struct {
	float8 x;
	float8 y;
	float8 z;
} PointXYZ;

PointXYZ *clonePointXYZ(const PointXYZ *point, PointXYZ *result);
PGDLLEXPORT Datum clonePointXYZV1(PG_FUNCTION_ARGS);

PointXYZ *constructorPointXYZ(float8 x, float8 y, float8 z, PointXYZ *result);
PGDLLEXPORT Datum constructorPointXYZV1(PG_FUNCTION_ARGS);

PointXYZ *inPointXYZ(const char *str, PointXYZ *result, char **error);
PGDLLEXPORT Datum inPointXYZV1(PG_FUNCTION_ARGS);

char *outPointXYZ(const PointXYZ *point);
PGDLLEXPORT Datum outPointXYZV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum recvPointXYZ(PG_FUNCTION_ARGS);

bytea *PointXYZ2Bytea(const PointXYZ *point);
PGDLLEXPORT Datum sendPointXYZ(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum hashPointXYZ(PG_FUNCTION_ARGS);

bool equalsPointXYZ(const PointXYZ *pointA, const PointXYZ *pointB);
PGDLLEXPORT Datum equalsPointXYZV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum getXPointXYZ(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getYPointXYZ(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getZPointXYZ(PG_FUNCTION_ARGS);

#endif
