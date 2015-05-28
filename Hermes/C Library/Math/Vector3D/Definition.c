/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/Math/Vector3D/Definition.h"
#include "../../h/Utilities/Utilities.h"
#include <libpq/pqformat.h>
#include <access/hash.h>

Vector3D *cloneVector3D(const Vector3D *vector, Vector3D *result) {
	Vector3D *ret = result ? result : (Vector3D *) palloc(sizeof(*ret));

	*ret = *vector;

	return ret;
}

PGDLLEXPORT Datum cloneVector3DV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(cloneVector3D((Vector3D *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(cloneVector3DV1);

Vector3D *constructorVector3D(float8 x, float8 y, float8 z, Vector3D *result) {
	Vector3D *ret = result ? result : (Vector3D *) palloc(sizeof(*ret));

	ret->x = x;
	ret->y = y;
	ret->z = z;

	return ret;
}

PGDLLEXPORT Datum constructorVector3DV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(constructorVector3D(PG_GETARG_FLOAT8(0), PG_GETARG_FLOAT8(1), PG_GETARG_FLOAT8(2), NULL));
}
PG_FUNCTION_INFO_V1(constructorVector3DV1);

Vector3D *inVector3D(const char *str, Vector3D *result, char **error) {
	float8 x, y, z;
	Vector3D *ret = NULL;

	if (error)
		*error = NULL;

	if (sscanf(str, " %lf %lf %lf ", &x, &y, &z) != 3) {
		if (error)
			asprintfHermes(error, "invalid input syntax for Vector3D: \"%s\"", str);
		return NULL ;
	}

	ret = result ? result : (Vector3D *) palloc(sizeof(*ret));

	ret->x = x;
	ret->y = y;
	ret->z = z;

	return ret;
}

PGDLLEXPORT Datum inVector3DV1(PG_FUNCTION_ARGS) {
	char *error;
	Vector3D *result = inVector3D(PG_GETARG_CSTRING(0), NULL, &error);

	if (error)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("%s", error)));

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(inVector3DV1);

char *outVector3D(const Vector3D *vector) {
	char *result = NULL;

	asprintfHermes(&result, "%f %f %f", vector->x, vector->y, vector->z);

	return result;
}

PGDLLEXPORT Datum outVector3DV1(PG_FUNCTION_ARGS) {
	PG_RETURN_CSTRING(outVector3D((Vector3D *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(outVector3DV1);

PGDLLEXPORT Datum recvVector3D(PG_FUNCTION_ARGS) {
	StringInfo msg = (StringInfo) PG_GETARG_POINTER(0);
	Vector3D *result = (Vector3D *) palloc(sizeof(*result));

	result->x = pq_getmsgfloat8(msg);
	result->y = pq_getmsgfloat8(msg);
	result->z = pq_getmsgfloat8(msg);

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(recvVector3D);

bytea *Vector3D2Bytea(const Vector3D *vector) {
	StringInfoData result;

	pq_begintypsend(&result);

	pq_sendfloat8(&result, ELIMINATE_MINUS_ZERO(vector->x));
	pq_sendfloat8(&result, ELIMINATE_MINUS_ZERO(vector->y));
	pq_sendfloat8(&result, ELIMINATE_MINUS_ZERO(vector->z));

	return pq_endtypsend(&result);
}

PGDLLEXPORT Datum sendVector3D(PG_FUNCTION_ARGS) {
	PG_RETURN_BYTEA_P(Vector3D2Bytea((Vector3D *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(sendVector3D);

PGDLLEXPORT Datum hashVector3D(PG_FUNCTION_ARGS) {
	PG_RETURN_DATUM(DirectFunctionCall1(hashvarlena, PointerGetDatum(Vector3D2Bytea((Vector3D *) PG_GETARG_POINTER(0)))));
}
PG_FUNCTION_INFO_V1(hashVector3D);

bool equalsVector3D(const Vector3D *vectorA, const Vector3D *vectorB) {
	return vectorA->x == vectorB->x && vectorA->y == vectorB->y && vectorA->z == vectorB->z;
}

PGDLLEXPORT Datum equalsVector3DV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(equalsVector3D((Vector3D *) PG_GETARG_POINTER(0),(Vector3D *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(equalsVector3DV1);

PGDLLEXPORT Datum getXVector3D(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(((Vector3D *) PG_GETARG_POINTER(0))->x);
}
PG_FUNCTION_INFO_V1(getXVector3D);

PGDLLEXPORT Datum getYVector3D(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(((Vector3D *) PG_GETARG_POINTER(0))->y);
}
PG_FUNCTION_INFO_V1(getYVector3D);

PGDLLEXPORT Datum getZVector3D(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(((Vector3D *) PG_GETARG_POINTER(0))->z);
}
PG_FUNCTION_INFO_V1(getZVector3D);
