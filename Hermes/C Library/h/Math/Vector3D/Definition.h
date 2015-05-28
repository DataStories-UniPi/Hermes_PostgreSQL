/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_VECTOR3D_DEFINITION_H_
#define	HE_VECTOR3D_DEFINITION_H_

#include <postgres.h>
#include <fmgr.h>

typedef struct {
	float8 x;
	float8 y;
	float8 z;
} Vector3D;

Vector3D *cloneVector3D(const Vector3D *vector, Vector3D *result);
PGDLLEXPORT Datum cloneVector3DV1(PG_FUNCTION_ARGS);

Vector3D *constructorVector3D(float8 x, float8 y, float8 z, Vector3D *result);
PGDLLEXPORT Datum constructorVector3DV1(PG_FUNCTION_ARGS);

Vector3D *inVector3D(const char *str, Vector3D *result, char **error);
PGDLLEXPORT Datum inVector3DV1(PG_FUNCTION_ARGS);

char *outVector3D(const Vector3D *vector);
PGDLLEXPORT Datum outVector3DV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum recvVector3D(PG_FUNCTION_ARGS);

bytea *Vector3D2Bytea(const Vector3D *vector);
PGDLLEXPORT Datum sendVector3D(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum hashVector3D(PG_FUNCTION_ARGS);

bool equalsVector3D(const Vector3D *vectorA, const Vector3D *vectorB);
PGDLLEXPORT Datum equalsVector3DV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum getXVector3D(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getYVector3D(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getZVector3D(PG_FUNCTION_ARGS);

#endif
