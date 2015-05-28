/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/Math/Vector2D/Definition.h"
#include "../../h/Utilities/Utilities.h"
#include <libpq/pqformat.h>
#include <access/hash.h>

Vector2D *cloneVector2D(const Vector2D *vector, Vector2D *result) {
	Vector2D *ret = result ? result : (Vector2D *) palloc(sizeof(*ret));

	*ret = *vector;

	return ret;
}

PGDLLEXPORT Datum cloneVector2DV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(cloneVector2D((Vector2D *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(cloneVector2DV1);

Vector2D *constructorVector2D(float8 x, float8 y, Vector2D *result) {
	Vector2D *ret = result ? result : (Vector2D *) palloc(sizeof(*ret));

	ret->x = x;
	ret->y = y;

	return ret;
}

PGDLLEXPORT Datum constructorVector2DV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(constructorVector2D(PG_GETARG_FLOAT8(0), PG_GETARG_FLOAT8(1), NULL));
}
PG_FUNCTION_INFO_V1(constructorVector2DV1);

Vector2D *inVector2D(const char *str, Vector2D *result, char **error) {
	float8 x, y;
	Vector2D *ret = NULL;

	if (error)
		*error = NULL;

	if (sscanf(str, " %lf %lf ", &x, &y) != 2) {
		if (error)
			asprintfHermes(error, "invalid input syntax for Vector2D: \"%s\"", str);
		return NULL ;
	}

	ret = result ? result : (Vector2D *) palloc(sizeof(*ret));

	ret->x = x;
	ret->y = y;

	return ret;
}

PGDLLEXPORT Datum inVector2DV1(PG_FUNCTION_ARGS) {
	char *error = NULL;
	Vector2D *result = inVector2D(PG_GETARG_CSTRING(0), NULL, &error);

	if (error)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("%s", error)));

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(inVector2DV1);

char *outVector2D(const Vector2D *vector) {
	char *result = NULL;

	asprintfHermes(&result, "%f %f", vector->x, vector->y);

	return result;
}

PGDLLEXPORT Datum outVector2DV1(PG_FUNCTION_ARGS) {
	PG_RETURN_CSTRING(outVector2D((Vector2D *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(outVector2DV1);

PGDLLEXPORT Datum recvVector2D(PG_FUNCTION_ARGS) {
	StringInfo msg = (StringInfo) PG_GETARG_POINTER(0);
	Vector2D *result = (Vector2D *) palloc(sizeof(*result));

	result->x = pq_getmsgfloat8(msg);
	result->y = pq_getmsgfloat8(msg);

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(recvVector2D);

bytea *Vector2D2Bytea(const Vector2D *vector) {
	StringInfoData buf;

	pq_begintypsend(&buf);

	pq_sendfloat8(&buf, ELIMINATE_MINUS_ZERO(vector->x));
	pq_sendfloat8(&buf, ELIMINATE_MINUS_ZERO(vector->y));

	return pq_endtypsend(&buf);
}

PGDLLEXPORT Datum sendVector2D(PG_FUNCTION_ARGS) {
	PG_RETURN_BYTEA_P(Vector2D2Bytea((Vector2D *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(sendVector2D);

PGDLLEXPORT Datum hashVector2D(PG_FUNCTION_ARGS) {
	PG_RETURN_DATUM(DirectFunctionCall1(hashvarlena, PointerGetDatum(Vector2D2Bytea((Vector2D *) PG_GETARG_POINTER(0)))));
}
PG_FUNCTION_INFO_V1(hashVector2D);

bool equalsVector2D(const Vector2D *vectorA, const Vector2D *vectorB) {
	return vectorA->x == vectorB->x && vectorA->y == vectorB->y;
}

PGDLLEXPORT Datum equalsVector2DV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(equalsVector2D((Vector2D *) PG_GETARG_POINTER(0),(Vector2D *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(equalsVector2DV1);

PGDLLEXPORT Datum getXVector2D(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(((Vector2D *) PG_GETARG_POINTER(0))->x);
}
PG_FUNCTION_INFO_V1(getXVector2D);

PGDLLEXPORT Datum getYVector2D(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(((Vector2D *) PG_GETARG_POINTER(0))->y);
}
PG_FUNCTION_INFO_V1(getYVector2D);
