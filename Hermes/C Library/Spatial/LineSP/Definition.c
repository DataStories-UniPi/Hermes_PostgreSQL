/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/Spatial/LineSP/Definition.h"
#include "../../h/Utilities/Utilities.h"
#include <libpq/pqformat.h>
#include <access/hash.h>

LineSP *cloneLineSP(const LineSP *line, LineSP *result) {
	LineSP *ret = result ? result : (LineSP *) palloc(sizeof(*ret));

	*ret = *line;

	return ret;
}

PGDLLEXPORT Datum cloneLineSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(cloneLineSP((LineSP *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(cloneLineSPV1);

LineSP *constructorLineSP(float8 A, float8 B, float8 C, LineSP *result) {
	LineSP *ret = result ? result : (LineSP *) palloc(sizeof(*ret));

	ret->A = A;
	ret->B = B;
	ret->C = C;

	return ret;
}

PGDLLEXPORT Datum constructorLineSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(constructorLineSP(PG_GETARG_FLOAT8(0), PG_GETARG_FLOAT8(1), PG_GETARG_FLOAT8(2), NULL));
}
PG_FUNCTION_INFO_V1(constructorLineSPV1);

LineSP *inLineSP(const char *str, LineSP *result, char **error) {
	float8 A, B, C;
	LineSP *ret = NULL;

	if (error)
		*error = NULL;

	if (sscanf(str, " %lf %lf %lf ", &A, &B, &C) != 3) {
		if (error)
			asprintfHermes(error, "invalid input syntax for LineSP: \"%s\"", str);
		return NULL ;
	}

	ret = result ? result : (LineSP *) palloc(sizeof(*ret));

	ret->A = A;
	ret->B = B;
	ret->C = C;

	return ret;
}

PGDLLEXPORT Datum inLineSPV1(PG_FUNCTION_ARGS) {
	char *error;
	LineSP *result = inLineSP(PG_GETARG_CSTRING(0), NULL, &error);

	if (error)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("%s", error)));

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(inLineSPV1);

char *outLineSP(const LineSP *line) {
	char *result = NULL;

	asprintfHermes(&result, "%f %f %f", line->A, line->B, line->C);

	return result;
}

PGDLLEXPORT Datum outLineSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_CSTRING(outLineSP((LineSP *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(outLineSPV1);

PGDLLEXPORT Datum recvLineSP(PG_FUNCTION_ARGS) {
	StringInfo msg = (StringInfo) PG_GETARG_POINTER(0);
	LineSP *result = (LineSP *) palloc(sizeof(*result));

	result->A = pq_getmsgfloat8(msg);
	result->B = pq_getmsgfloat8(msg);
	result->C = pq_getmsgfloat8(msg);

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(recvLineSP);

bytea *LineSP2Bytea(const LineSP *line) {
	StringInfoData buf;

	pq_begintypsend(&buf);

	pq_sendfloat8(&buf, ELIMINATE_MINUS_ZERO(line->A));
	pq_sendfloat8(&buf, ELIMINATE_MINUS_ZERO(line->B));
	pq_sendfloat8(&buf, ELIMINATE_MINUS_ZERO(line->C));

	return pq_endtypsend(&buf);
}

PGDLLEXPORT Datum sendLineSP(PG_FUNCTION_ARGS) {
	PG_RETURN_BYTEA_P(LineSP2Bytea((LineSP *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(sendLineSP);

PGDLLEXPORT Datum hashLineSP(PG_FUNCTION_ARGS) {
	PG_RETURN_DATUM(DirectFunctionCall1(hashvarlena, PointerGetDatum(LineSP2Bytea((LineSP *) PG_GETARG_POINTER(0)))));
}
PG_FUNCTION_INFO_V1(hashLineSP);

bool equalsLineSP(const LineSP *lineA, const LineSP *lineB) {
	return lineA->A == lineB->A && lineA->B == lineB->B && lineA->C == lineB->C;
}

PGDLLEXPORT Datum equalsLineSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(equalsLineSP((LineSP *) PG_GETARG_POINTER(0),(LineSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(equalsLineSPV1);

PGDLLEXPORT Datum getALineSP(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(((LineSP *) PG_GETARG_POINTER(0))->A);
}
PG_FUNCTION_INFO_V1(getALineSP);

PGDLLEXPORT Datum getBLineSP(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(((LineSP *) PG_GETARG_POINTER(0))->B);
}
PG_FUNCTION_INFO_V1(getBLineSP);

PGDLLEXPORT Datum getCLineSP(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(((LineSP *) PG_GETARG_POINTER(0))->C);
}
PG_FUNCTION_INFO_V1(getCLineSP);
