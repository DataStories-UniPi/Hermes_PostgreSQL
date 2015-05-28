/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_VECTOR2D_DEFINITION_H_
#define	HE_VECTOR2D_DEFINITION_H_

#include <postgres.h>
#include <fmgr.h>

typedef struct {
	float8 x;
	float8 y;
} Vector2D;

Vector2D *cloneVector2D(const Vector2D *vector, Vector2D *result);
PGDLLEXPORT Datum cloneVector2DV1(PG_FUNCTION_ARGS);

Vector2D *constructorVector2D(float8 x, float8 y, Vector2D *result);
PGDLLEXPORT Datum constructorVector2DV1(PG_FUNCTION_ARGS);

Vector2D *inVector2D(const char *str, Vector2D *result, char **error);
PGDLLEXPORT Datum inVector2DV1(PG_FUNCTION_ARGS);

char *outVector2D(const Vector2D *vector);
PGDLLEXPORT Datum outVector2DV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum recvVector2D(PG_FUNCTION_ARGS);

bytea *Vector2D2Bytea(const Vector2D *vector);
PGDLLEXPORT Datum sendVector2D(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum hashVector2D(PG_FUNCTION_ARGS);

bool equalsVector2D(const Vector2D *vectorA, const Vector2D *vectorB);
PGDLLEXPORT Datum equalsVector2DV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum getXVector2D(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getYVector2D(PG_FUNCTION_ARGS);

#endif
