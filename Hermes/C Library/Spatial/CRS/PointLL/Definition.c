/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../../h/Spatial/CRS/PointLL/Definition.h"
#include "../../../h/Utilities/Utilities.h"
#include <libpq/pqformat.h>
#include <access/hash.h>

PointLL *clonePointLL(const PointLL *point, PointLL *result) {
	PointLL *ret = result ? result : (PointLL *) palloc(sizeof(*ret));

	*ret = *point;

	return ret;
}

PGDLLEXPORT Datum clonePointLLV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(clonePointLL((PointLL *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(clonePointLLV1);

PointLL *constructorPointLL(float8 lon, float8 lat, PointLL *result, char **error) {
	PointLL *ret = NULL;

	if (error)
		*error = NULL;

	if (lon > 180 || lon < -180 || lat > 90 || lat < -90) {
		if (error)
			*error = "lon and/or lat out of bounds";
		return NULL ;
	}

	ret = result ? result : (PointLL *) palloc(sizeof(*ret));

	ret->lon = lon;
	ret->lat = lat;

	return ret;
}

PGDLLEXPORT Datum constructorPointLLV1(PG_FUNCTION_ARGS) {
	char *error;
	PointLL *result = constructorPointLL(PG_GETARG_FLOAT8(0), PG_GETARG_FLOAT8(1), NULL, &error);

	if (error)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("%s", error)));

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(constructorPointLLV1);

PointLL *inPointLL(const char *str, PointLL *result, char **error) {
	float8 lon, lat;
	PointLL *ret = NULL;

	if (error)
		*error = NULL;

	if (sscanf(str, " %lf %lf ", &lon, &lat) != 2) {
		if (error)
			asprintfHermes(error, "invalid input syntax for PointLL: \"%s\"", str);
		return NULL ;
	}
	if (lon > 180 || lon < -180 || lat > 90 || lat < -90) {
		if (error)
			*error = "lon and/or lat out of bounds";
		return NULL ;
	}

	ret = result ? result : (PointLL *) palloc(sizeof(*ret));

	ret->lon = lon;
	ret->lat = lat;

	return ret;
}

PGDLLEXPORT Datum inPointLLV1(PG_FUNCTION_ARGS) {
	char *error;
	PointLL *result = inPointLL(PG_GETARG_CSTRING(0), NULL, &error);

	if (error)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("%s", error)));

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(inPointLLV1);

char *outPointLL(const PointLL *point) {
	char *result = NULL;

	asprintfHermes(&result, "%f %f", point->lon, point->lat);

	return result;
}

PGDLLEXPORT Datum outPointLLV1(PG_FUNCTION_ARGS) {
	PG_RETURN_CSTRING(outPointLL((PointLL *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(outPointLLV1);

PGDLLEXPORT Datum recvPointLL(PG_FUNCTION_ARGS) {
	StringInfo msg = (StringInfo) PG_GETARG_POINTER(0);
	PointLL *result = (PointLL *) palloc(sizeof(*result));

	result->lon = pq_getmsgfloat8(msg);
	result->lat = pq_getmsgfloat8(msg);

	if (result->lon > 180 || result->lon < -180 || result->lat > 90 || result->lat < -90) {
		pfree(result);
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("lon and/or lat out of bounds")));
	}

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(recvPointLL);

bytea *PointLL2Bytea(const PointLL *point) {
	StringInfoData buf;

	pq_begintypsend(&buf);

	pq_sendfloat8(&buf, ELIMINATE_MINUS_ZERO(point->lon));
	pq_sendfloat8(&buf, ELIMINATE_MINUS_ZERO(point->lat));

	return pq_endtypsend(&buf);
}

PGDLLEXPORT Datum sendPointLL(PG_FUNCTION_ARGS) {
	PG_RETURN_BYTEA_P(PointLL2Bytea((PointLL *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(sendPointLL);

PGDLLEXPORT Datum hashPointLL(PG_FUNCTION_ARGS) {
	PG_RETURN_DATUM(DirectFunctionCall1(hashvarlena, PointerGetDatum(PointLL2Bytea((PointLL *) PG_GETARG_POINTER(0)))));
}
PG_FUNCTION_INFO_V1(hashPointLL);

bool equalsPointLL(const PointLL *pointA, const PointLL *pointB) {
	return pointA->lon == pointB->lon && pointA->lat == pointB->lat;
}

PGDLLEXPORT Datum equalsPointLLV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(equalsPointLL((PointLL *) PG_GETARG_POINTER(0),(PointLL *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(equalsPointLLV1);

PGDLLEXPORT Datum getLonPointLL(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(((PointLL *) PG_GETARG_POINTER(0))->lon);
}
PG_FUNCTION_INFO_V1(getLonPointLL);

PGDLLEXPORT Datum getLatPointLL(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(((PointLL *) PG_GETARG_POINTER(0))->lat);
}
PG_FUNCTION_INFO_V1(getLatPointLL);
