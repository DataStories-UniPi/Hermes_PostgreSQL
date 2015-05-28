/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/Math/Vector2D/ArithmeticOperators.h"
#include <math.h>

Vector2D *additionVector2D(const Vector2D *vectorA, const Vector2D *vectorB, Vector2D *result) {
	Vector2D *ret = result ? result : (Vector2D *) palloc(sizeof(*ret));

	ret->x = vectorA->x + vectorB->x;
	ret->y = vectorA->y + vectorB->y;

	return ret;
}

Vector2D *subtractionVector2D(const Vector2D *vectorA, const Vector2D *vectorB, Vector2D *result) {
	Vector2D *ret = result ? result : (Vector2D *) palloc(sizeof(*ret));

	ret->x = vectorA->x - vectorB->x;
	ret->y = vectorA->y - vectorB->y;

	return ret;
}

Vector2D *multiplicationScalarVector2D(const Vector2D *vector, float8 n, Vector2D *result) {
	Vector2D *ret = result ? result : (Vector2D *) palloc(sizeof(*ret));

	ret->x = vector->x * n;
	ret->y = vector->y * n;

	return ret;
}

Vector2D *divisionScalarVector2D(const Vector2D *vector, float8 n, Vector2D *result) {
	Vector2D *ret = result ? result : (Vector2D *) palloc(sizeof(*ret));

	ret->x = vector->x / n;
	ret->y = vector->y / n;

	return ret;
}

float8 dotProductVector2D(const Vector2D *vectorA, const Vector2D *vectorB) {
	return vectorA->x * vectorB->x + vectorA->y * vectorB->y;
}

float8 perpDotProductVector2D(const Vector2D *vectorA, const Vector2D *vectorB) {
	return vectorA->x * vectorB->y - vectorA->y * vectorB->x;
}

float8 normVector2D(const Vector2D *vector) {
	return sqrt(dotProductVector2D(vector, vector));
}

float8 sqNormVector2D(const Vector2D *vector) {
	return dotProductVector2D(vector, vector);
}

Vector2D *normalizeVector2D(const Vector2D *vector, Vector2D *result, float8 *norm) {
	float8 n;
	Vector2D *ret = result ? result : (Vector2D *) palloc(sizeof(*ret));

	n = normVector2D(vector);

	if (norm)
		*norm = n;

	if (n == 0.0) {
		ret->x = ret->y = 0.0;
	} else {
		ret->x = vector->x / n;
		ret->y = vector->y / n;
	}

	return ret;
}

float8 distanceVector2D(const Vector2D *vectorA, const Vector2D *vectorB) {
	Vector2D tmp;

	subtractionVector2D(vectorA, vectorB, &tmp);

	return normVector2D(&tmp);
}

float8 sqDistanceVector2D(const Vector2D *vectorA, const Vector2D *vectorB) {
	Vector2D tmp;

	subtractionVector2D(vectorA, vectorB, &tmp);

	return sqNormVector2D(&tmp);
}

float8 angleVector2D(const Vector2D *vectorA, const Vector2D *vectorB) {
	return acos(dotProductVector2D(vectorA, vectorB) / (normVector2D(vectorA) * normVector2D(vectorB)));

	/*
	 //this way if any vector is the zero vector it will return 90 degrees
	 //the above way will return NaN, because of the division by 0.0
	 Vector2D *ne = NULL;
	 Vector2D *no = NULL;

	 normalizeVector2D(vectorA, &ne);
	 normalizeVector2D(vectorB, &no);

	 return acos(dotProductVector2D(ne, no));
	 */
}

PGDLLEXPORT Datum additionVector2DV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(additionVector2D((Vector2D *) PG_GETARG_POINTER(0), (Vector2D *) PG_GETARG_POINTER(1), NULL));
}
PG_FUNCTION_INFO_V1(additionVector2DV1);

PGDLLEXPORT Datum subtractionVector2DV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(subtractionVector2D((Vector2D *) PG_GETARG_POINTER(0), (Vector2D *) PG_GETARG_POINTER(1), NULL));
}
PG_FUNCTION_INFO_V1(subtractionVector2DV1);

PGDLLEXPORT Datum multiplicationScalarVector2DV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(multiplicationScalarVector2D((Vector2D *) PG_GETARG_POINTER(0), PG_GETARG_FLOAT8(1), NULL));
}
PG_FUNCTION_INFO_V1(multiplicationScalarVector2DV1);

PGDLLEXPORT Datum divisionScalarVector2DV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(divisionScalarVector2D((Vector2D *) PG_GETARG_POINTER(0), PG_GETARG_FLOAT8(1), NULL));
}
PG_FUNCTION_INFO_V1(divisionScalarVector2DV1);

PGDLLEXPORT Datum dotProductVector2DV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(dotProductVector2D((Vector2D *) PG_GETARG_POINTER(0), (Vector2D *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(dotProductVector2DV1);

PGDLLEXPORT Datum perpDotProductVector2DV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(perpDotProductVector2D((Vector2D *) PG_GETARG_POINTER(0), (Vector2D *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(perpDotProductVector2DV1);

PGDLLEXPORT Datum normVector2DV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(normVector2D((Vector2D *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(normVector2DV1);

PGDLLEXPORT Datum sqNormVector2DV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(sqNormVector2D((Vector2D *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(sqNormVector2DV1);

PGDLLEXPORT Datum distanceVector2DV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(distanceVector2D((Vector2D *) PG_GETARG_POINTER(0), (Vector2D *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(distanceVector2DV1);

PGDLLEXPORT Datum sqDistanceVector2DV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(sqDistanceVector2D((Vector2D *) PG_GETARG_POINTER(0), (Vector2D *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(sqDistanceVector2DV1);

PGDLLEXPORT Datum normalizeVector2DV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(normalizeVector2D((Vector2D *) PG_GETARG_POINTER(0), NULL, NULL));
}
PG_FUNCTION_INFO_V1(normalizeVector2DV1);

PGDLLEXPORT Datum angleVector2DV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(angleVector2D((Vector2D *) PG_GETARG_POINTER(0), (Vector2D *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(angleVector2DV1);
