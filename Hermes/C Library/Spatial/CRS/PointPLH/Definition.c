/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../../h/Spatial/CRS/PointPLH/Definition.h"
#include "../../../h/Utilities/Utilities.h"
#include <libpq/pqformat.h>
#include <access/hash.h>

PointPLH *clonePointPLH(const PointPLH *point, PointPLH *result) {
	PointPLH *ret = result ? result : (PointPLH *) palloc(sizeof(*ret));

	*ret = *point;

	return ret;
}

PGDLLEXPORT Datum clonePointPLHV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(clonePointPLH((PointPLH *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(clonePointPLHV1);

PointPLH *constructorPointPLH(float8 phi, float8 lambda, float8 h, PointPLH *result) {
	PointPLH *ret = result ? result : (PointPLH *) palloc(sizeof(*ret));

	ret->phi = phi;
	ret->lambda = lambda;
	ret->h = h;

	return ret;
}

PGDLLEXPORT Datum constructorPointPLHV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(constructorPointPLH(PG_GETARG_FLOAT8(0), PG_GETARG_FLOAT8(1), PG_GETARG_FLOAT8(2), NULL));
}
PG_FUNCTION_INFO_V1(constructorPointPLHV1);

PointPLH *inPointPLH(const char *str, PointPLH *result, char **error) {
	float8 phi, lambda, h;
	PointPLH *ret = NULL;

	if (error)
		*error = NULL;

	if (sscanf(str, " %lf %lf %lf ", &phi, &lambda, &h) != 3) {
		if (error)
			asprintfHermes(error, "invalid input syntax for PointPLH: \"%s\"", str);
		return NULL ;
	}

	ret = result ? result : (PointPLH *) palloc(sizeof(*ret));

	ret->phi = phi;
	ret->lambda = lambda;
	ret->h = h;

	return ret;
}

PGDLLEXPORT Datum inPointPLHV1(PG_FUNCTION_ARGS) {
	char *error;
	PointPLH *result = inPointPLH(PG_GETARG_CSTRING(0), NULL, &error);

	if (error)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("%s", error)));

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(inPointPLHV1);

char *outPointPLH(const PointPLH *point) {
	char *result = NULL;

	asprintfHermes(&result, "%f %f %f", point->phi, point->lambda, point->h);

	return result;
}

PGDLLEXPORT Datum outPointPLHV1(PG_FUNCTION_ARGS) {
	PG_RETURN_CSTRING(outPointPLH((PointPLH *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(outPointPLHV1);

PGDLLEXPORT Datum recvPointPLH(PG_FUNCTION_ARGS) {
	StringInfo msg = (StringInfo) PG_GETARG_POINTER(0);
	PointPLH *result = (PointPLH *) palloc(sizeof(*result));

	result->phi = pq_getmsgfloat8(msg);
	result->lambda = pq_getmsgfloat8(msg);
	result->h = pq_getmsgfloat8(msg);

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(recvPointPLH);

bytea *PointPLH2Bytea(const PointPLH *point) {
	StringInfoData buf;

	pq_begintypsend(&buf);

	pq_sendfloat8(&buf, ELIMINATE_MINUS_ZERO(point->phi));
	pq_sendfloat8(&buf, ELIMINATE_MINUS_ZERO(point->lambda));
	pq_sendfloat8(&buf, ELIMINATE_MINUS_ZERO(point->h));

	return pq_endtypsend(&buf);
}

PGDLLEXPORT Datum sendPointPLH(PG_FUNCTION_ARGS) {
	PG_RETURN_BYTEA_P(PointPLH2Bytea((PointPLH *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(sendPointPLH);

PGDLLEXPORT Datum hashPointPLH(PG_FUNCTION_ARGS) {
	PG_RETURN_DATUM(DirectFunctionCall1(hashvarlena, PointerGetDatum(PointPLH2Bytea((PointPLH *) PG_GETARG_POINTER(0)))));
}
PG_FUNCTION_INFO_V1(hashPointPLH);

bool equalsPointPLH(const PointPLH *pointA, const PointPLH *pointB) {
	return pointA->phi == pointB->phi && pointA->lambda == pointB->lambda && pointA->h == pointB->h;
}

PGDLLEXPORT Datum equalsPointPLHV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(equalsPointPLH((PointPLH *) PG_GETARG_POINTER(0),(PointPLH *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(equalsPointPLHV1);

PGDLLEXPORT Datum getPhiPointPLH(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(((PointPLH *) PG_GETARG_POINTER(0))->phi);
}
PG_FUNCTION_INFO_V1(getPhiPointPLH);

PGDLLEXPORT Datum getLambdaPointPLH(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(((PointPLH *) PG_GETARG_POINTER(0))->lambda);
}
PG_FUNCTION_INFO_V1(getLambdaPointPLH);

PGDLLEXPORT Datum getHPointPLH(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(((PointPLH *) PG_GETARG_POINTER(0))->h);
}
PG_FUNCTION_INFO_V1(getHPointPLH);
