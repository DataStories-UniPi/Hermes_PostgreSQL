/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_VECTOR3D_ARITHMETIC_OPERATORS_H_
#define HE_VECTOR3D_ARITHMETIC_OPERATORS_H_

#include "Definition.h"

Vector3D *additionVector3D(const Vector3D *vectorA, const Vector3D *vectorB, Vector3D *result);
Vector3D *subtractionVector3D(const Vector3D *vectorA, const Vector3D *vectorB, Vector3D *result);
Vector3D *multiplicationScalarVector3D(const Vector3D *vector, float8 n, Vector3D *result);
Vector3D *divisionScalarVector3D(const Vector3D *vector, float8 n, Vector3D *result);

float8 dotProductVector3D(const Vector3D *vectorA, const Vector3D *vectorB);
Vector3D *crossProductVector3D(const Vector3D *vectorA, const Vector3D *vectorB, Vector3D *result);

float8 normVector3D(const Vector3D *vector);
float8 sqNormVector3D(const Vector3D *vector);

Vector3D *normalizeVector3D(const Vector3D *vector, Vector3D *result, float8 *norm);

float8 distanceVector3D(const Vector3D *vectorA, const Vector3D *vectorB);
float8 sqDistanceVector3D(const Vector3D *vectorA, const Vector3D *vectorB);

PGDLLEXPORT Datum additionVector3DV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum subtractionVector3DV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum multiplicationScalarVector3DV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum divisionScalarVector3DV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum dotProductVector3DV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum crossProductVector3DV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum normVector3DV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum sqNormVector3DV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum normalizeVector3DV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum distanceVector3DV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum sqDistanceVector3DV1(PG_FUNCTION_ARGS);

#endif
