/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_VECTOR2D_ARITHMETIC_OPERATORS_H_
#define HE_VECTOR2D_ARITHMETIC_OPERATORS_H_

/*
 *	References
 *		http://en.wikipedia.org/wiki/Euclidean_vector
 */

#include "Definition.h"

Vector2D *additionVector2D(const Vector2D *vectorA, const Vector2D *vectorB, Vector2D *result);
Vector2D *subtractionVector2D(const Vector2D *vectorA, const Vector2D *vectorB, Vector2D *result);
Vector2D *multiplicationScalarVector2D(const Vector2D *vector, float8 n, Vector2D *result);
Vector2D *divisionScalarVector2D(const Vector2D *vector, float8 n, Vector2D *result);

float8 dotProductVector2D(const Vector2D *vectorA, const Vector2D *vectorB);
float8 perpDotProductVector2D(const Vector2D *vectorA, const Vector2D *vectorB);

float8 normVector2D(const Vector2D *vector);
float8 sqNormVector2D(const Vector2D *vector);

Vector2D *normalizeVector2D(const Vector2D *vector, Vector2D *result, float8 *norm);

float8 distanceVector2D(const Vector2D *vectorA, const Vector2D *vectorB);
float8 sqDistanceVector2D(const Vector2D *vectorA, const Vector2D *vectorB);

float8 angleVector2D(const Vector2D *vectorA, const Vector2D *vectorB);

PGDLLEXPORT Datum additionVector2DV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum subtractionVector2DV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum multiplicationScalarVector2DV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum divisionScalarVector2DV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum dotProductVector2DV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum perpDotProductVector2DV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum normVector2DV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum sqNormVector2DV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum normalizeVector2DV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum distanceVector2DV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum sqDistanceVector2DV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum angleVector2DV1(PG_FUNCTION_ARGS);

#endif
