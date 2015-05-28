/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/Math/Vector3D/ArithmeticOperators.h"
#include <math.h>

Vector3D *additionVector3D(const Vector3D *vectorA, const Vector3D *vectorB, Vector3D *result) {
	Vector3D *ret = result ? result : (Vector3D *) palloc(sizeof(*ret));

	ret->x = vectorA->x + vectorB->x;
	ret->y = vectorA->y + vectorB->y;
	ret->z = vectorA->z + vectorB->z;

	return ret;
}

Vector3D *subtractionVector3D(const Vector3D *vectorA, const Vector3D *vectorB, Vector3D *result) {
	Vector3D *ret = result ? result : (Vector3D *) palloc(sizeof(*ret));

	ret->x = vectorA->x - vectorB->x;
	ret->y = vectorA->y - vectorB->y;
	ret->z = vectorA->z - vectorB->z;

	return ret;
}

Vector3D *multiplicationScalarVector3D(const Vector3D *vector, float8 n, Vector3D *result) {
	Vector3D *ret = result ? result : (Vector3D *) palloc(sizeof(*ret));

	ret->x = vector->x * n;
	ret->y = vector->y * n;
	ret->z = vector->z * n;

	return ret;
}

Vector3D *divisionScalarVector3D(const Vector3D *vector, float8 n, Vector3D *result) {
	Vector3D *ret = result ? result : (Vector3D *) palloc(sizeof(*ret));

	ret->x = vector->x / n;
	ret->y = vector->y / n;
	ret->z = vector->z / n;

	return ret;
}

float8 dotProductVector3D(const Vector3D *vectorA, const Vector3D *vectorB) {
	return vectorA->x * vectorB->x + vectorA->y * vectorB->y + vectorA->z * vectorB->z;
}

Vector3D *crossProductVector3D(const Vector3D *vectorA, const Vector3D *vectorB, Vector3D *result) {
	float8 x, y, z;
	Vector3D *ret = result ? result : (Vector3D *) palloc(sizeof(*ret));

	x = vectorA->y * vectorB->z - vectorA->z * vectorB->y;
	y = vectorA->z * vectorB->x - vectorA->x * vectorB->z;
	z = vectorA->x * vectorB->y - vectorA->y * vectorB->x;

	ret->x = x;
	ret->y = y;
	ret->z = z;

	return ret;
}

float8 normVector3D(const Vector3D *vector) {
	return sqrt(dotProductVector3D(vector, vector));
}

float8 sqNormVector3D(const Vector3D *vector) {
	return dotProductVector3D(vector, vector);
}

Vector3D *normalizeVector3D(const Vector3D *vector, Vector3D *result, float8 *norm) {
	float8 n;
	Vector3D *ret = result ? result : (Vector3D *) palloc(sizeof(*ret));

	n = normVector3D(vector);

	if (norm)
		*norm = n;

	if (n == 0.0) {
		ret->x = ret->y = ret->z = 0.0;
	} else {
		ret->x = vector->x / n;
		ret->y = vector->y / n;
		ret->z = vector->z / n;
	}

	return ret;
}

float8 distanceVector3D(const Vector3D *vectorA, const Vector3D *vectorB) {
	Vector3D tmp;

	subtractionVector3D(vectorA, vectorB, &tmp);

	return normVector3D(&tmp);
}

float8 sqDistanceVector3D(const Vector3D *vectorA, const Vector3D *vectorB) {
	Vector3D tmp;

	subtractionVector3D(vectorA, vectorB, &tmp);

	return sqNormVector3D(&tmp);
}

PGDLLEXPORT Datum additionVector3DV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(additionVector3D((Vector3D *) PG_GETARG_POINTER(0), (Vector3D *) PG_GETARG_POINTER(1), NULL));
}
PG_FUNCTION_INFO_V1(additionVector3DV1);

PGDLLEXPORT Datum subtractionVector3DV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(subtractionVector3D((Vector3D *) PG_GETARG_POINTER(0), (Vector3D *) PG_GETARG_POINTER(1), NULL));
}
PG_FUNCTION_INFO_V1(subtractionVector3DV1);

PGDLLEXPORT Datum multiplicationScalarVector3DV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(multiplicationScalarVector3D((Vector3D *) PG_GETARG_POINTER(0), PG_GETARG_FLOAT8(1), NULL));
}
PG_FUNCTION_INFO_V1(multiplicationScalarVector3DV1);

PGDLLEXPORT Datum divisionScalarVector3DV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(divisionScalarVector3D((Vector3D *) PG_GETARG_POINTER(0), PG_GETARG_FLOAT8(1), NULL));
}
PG_FUNCTION_INFO_V1(divisionScalarVector3DV1);

PGDLLEXPORT Datum dotProductVector3DV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(dotProductVector3D((Vector3D *) PG_GETARG_POINTER(0), (Vector3D *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(dotProductVector3DV1);

PGDLLEXPORT Datum crossProductVector3DV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(crossProductVector3D((Vector3D *) PG_GETARG_POINTER(0), (Vector3D *) PG_GETARG_POINTER(1), NULL));
}
PG_FUNCTION_INFO_V1(crossProductVector3DV1);

PGDLLEXPORT Datum normVector3DV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(normVector3D((Vector3D *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(normVector3DV1);

PGDLLEXPORT Datum sqNormVector3DV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(sqNormVector3D((Vector3D *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(sqNormVector3DV1);

PGDLLEXPORT Datum normalizeVector3DV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(normalizeVector3D((Vector3D *) PG_GETARG_POINTER(0), NULL, NULL));
}
PG_FUNCTION_INFO_V1(normalizeVector3DV1);

PGDLLEXPORT Datum distanceVector3DV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(distanceVector3D((Vector3D *) PG_GETARG_POINTER(0), (Vector3D *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(distanceVector3DV1);

PGDLLEXPORT Datum sqDistanceVector3DV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(sqDistanceVector3D((Vector3D *) PG_GETARG_POINTER(0), (Vector3D *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(sqDistanceVector3DV1);
