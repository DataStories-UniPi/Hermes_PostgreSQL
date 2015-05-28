/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_ELLIPSOID_DEFINITION_H_
#define	HE_ELLIPSOID_DEFINITION_H_

#include <postgres.h>
#include <fmgr.h>

typedef struct {
	float8 a;
	float8 rf;

	float8 b;
	float8 e1stSq;
	float8 e2ndSq;

	float8 r1;
} Ellipsoid;

extern const Ellipsoid WGS84;

Ellipsoid *cloneEllipsoid(const Ellipsoid *ellipsoid, Ellipsoid *result);
PGDLLEXPORT Datum cloneEllipsoidV1(PG_FUNCTION_ARGS);

Ellipsoid *constructorEllipsoid(float8 a, float8 rf, Ellipsoid *result);
PGDLLEXPORT Datum constructorEllipsoidV1(PG_FUNCTION_ARGS);

Ellipsoid *inEllipsoid(const char *str, Ellipsoid *result, char **error);
PGDLLEXPORT Datum inEllipsoidV1(PG_FUNCTION_ARGS);

char *outEllipsoid(const Ellipsoid *ellipsoid);
PGDLLEXPORT Datum outEllipsoidV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum recvEllipsoid(PG_FUNCTION_ARGS);

bytea *Ellipsoid2Bytea(const Ellipsoid *ellipsoid);
PGDLLEXPORT Datum sendEllipsoid(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum hashEllipsoid(PG_FUNCTION_ARGS);

bool equalsEllipsoid(const Ellipsoid *ellipsoidA, const Ellipsoid *ellipsoidB);
PGDLLEXPORT Datum equalsEllipsoidV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum getAEllipsoid(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getRfEllipsoid(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum getBEllipsoid(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getE1stSqEllipsoid(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getE2ndSqEllipsoid(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum getR1Ellipsoid(PG_FUNCTION_ARGS);

#endif
