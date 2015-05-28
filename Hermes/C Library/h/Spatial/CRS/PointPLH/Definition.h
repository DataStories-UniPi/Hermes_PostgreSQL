/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_POINTPLH_DEFINITION_H_
#define	HE_POINTPLH_DEFINITION_H_

#include <postgres.h>
#include <fmgr.h>

typedef struct {
	float8 phi;
	float8 lambda;
	float8 h;
} PointPLH;

PointPLH *clonePointPLH(const PointPLH *point, PointPLH *result);
PGDLLEXPORT Datum clonePointPLHV1(PG_FUNCTION_ARGS);

PointPLH *constructorPointPLH(float8 phi, float8 lambda, float8 h, PointPLH *result);
PGDLLEXPORT Datum constructorPointPLHV1(PG_FUNCTION_ARGS);

PointPLH *inPointPLH(const char *str, PointPLH *result, char **error);
PGDLLEXPORT Datum inPointPLHV1(PG_FUNCTION_ARGS);

char *outPointPLH(const PointPLH *point);
PGDLLEXPORT Datum outPointPLHV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum recvPointPLH(PG_FUNCTION_ARGS);

bytea *PointPLH2Bytea(const PointPLH *point);
PGDLLEXPORT Datum sendPointPLH(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum hashPointPLH(PG_FUNCTION_ARGS);

bool equalsPointPLH(const PointPLH *pointA, const PointPLH *pointB);
PGDLLEXPORT Datum equalsPointPLHV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum getPhiPointPLH(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getLambdaPointPLH(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getHPointPLH(PG_FUNCTION_ARGS);

#endif
