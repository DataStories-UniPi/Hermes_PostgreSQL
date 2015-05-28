/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../../h/Spatial/CRS/Ellipsoid/Definition.h"
#include "../../../h/Utilities/Utilities.h"
#include <libpq/pqformat.h>
#include <access/hash.h>
#include <math.h>

const Ellipsoid WGS84 = { 6378137.0, 298.257223563, 6356752.3142, 0.00669437999014, 0.00673949674228, 6371008.7714 };

Ellipsoid *cloneEllipsoid(const Ellipsoid *ellipsoid, Ellipsoid *result) {
	Ellipsoid *ret = result ? result : (Ellipsoid *) palloc(sizeof(*ret));

	*ret = *ellipsoid;

	return ret;
}

PGDLLEXPORT Datum cloneEllipsoidV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(cloneEllipsoid((Ellipsoid *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(cloneEllipsoidV1);

Ellipsoid *constructorEllipsoid(float8 a, float8 rf, Ellipsoid *result) {
	Ellipsoid *ret = result ? result : (Ellipsoid *) palloc(sizeof(*ret));

	ret->a = a;
	ret->rf = rf;

	ret->b = ret->a * (1.0 - 1.0 / ret->rf);
	ret->e1stSq = 1.0 - pow(ret->b, 2.0) / pow(ret->a, 2.0);
	ret->e2ndSq = pow(ret->a, 2.0) / pow(ret->b, 2.0) - 1.0;

	ret->r1 = (2.0 * ret->a + ret->b) / 3.0;

	return ret;
}

PGDLLEXPORT Datum constructorEllipsoidV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(constructorEllipsoid(PG_GETARG_FLOAT8(0), PG_GETARG_FLOAT8(1), NULL));
}
PG_FUNCTION_INFO_V1(constructorEllipsoidV1);

Ellipsoid *inEllipsoid(const char *str, Ellipsoid *result, char **error) {
	float8 a, rf;

	if (error)
		*error = NULL;

	if (sscanf(str, " %lf %lf ", &a, &rf) != 2) {
		if (error)
			asprintfHermes(error, "invalid input syntax for Ellipsoid: \"%s\"", str);

		return NULL;
	}

	return constructorEllipsoid(a, rf, result);;
}

PGDLLEXPORT Datum inEllipsoidV1(PG_FUNCTION_ARGS) {
	char *error;
	Ellipsoid *result = inEllipsoid(PG_GETARG_CSTRING(0), NULL, &error);

	if (error)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("%s", error)));

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(inEllipsoidV1);

char *outEllipsoid(const Ellipsoid *ellipsoid) {
	char *result = NULL;

	asprintfHermes(&result, "%f %f", ellipsoid->a, ellipsoid->rf);

	return result;
}

PGDLLEXPORT Datum outEllipsoidV1(PG_FUNCTION_ARGS) {
	PG_RETURN_CSTRING(outEllipsoid((Ellipsoid *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(outEllipsoidV1);

PGDLLEXPORT Datum recvEllipsoid(PG_FUNCTION_ARGS) {
	StringInfo msg = (StringInfo) PG_GETARG_POINTER(0);
	Ellipsoid *result = (Ellipsoid *) palloc(sizeof(*result));

	result->a = pq_getmsgfloat8(msg);
	result->rf = pq_getmsgfloat8(msg);

	result->b = pq_getmsgfloat8(msg);
	result->e1stSq = pq_getmsgfloat8(msg);
	result->e2ndSq = pq_getmsgfloat8(msg);

	result->r1 = pq_getmsgfloat8(msg);

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(recvEllipsoid);

bytea *Ellipsoid2Bytea(const Ellipsoid *ellipsoid) {
	StringInfoData buf;

	pq_begintypsend(&buf);

	pq_sendfloat8(&buf, ELIMINATE_MINUS_ZERO(ellipsoid->a));
	pq_sendfloat8(&buf, ELIMINATE_MINUS_ZERO(ellipsoid->rf));

	pq_sendfloat8(&buf, ELIMINATE_MINUS_ZERO(ellipsoid->b));
	pq_sendfloat8(&buf, ELIMINATE_MINUS_ZERO(ellipsoid->e1stSq));
	pq_sendfloat8(&buf, ELIMINATE_MINUS_ZERO(ellipsoid->e2ndSq));

	pq_sendfloat8(&buf, ELIMINATE_MINUS_ZERO(ellipsoid->r1));

	return pq_endtypsend(&buf);
}

PGDLLEXPORT Datum sendEllipsoid(PG_FUNCTION_ARGS) {
	PG_RETURN_BYTEA_P(Ellipsoid2Bytea((Ellipsoid *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(sendEllipsoid);

PGDLLEXPORT Datum hashEllipsoid(PG_FUNCTION_ARGS) {
	PG_RETURN_DATUM(DirectFunctionCall1(hashvarlena, PointerGetDatum(Ellipsoid2Bytea((Ellipsoid *) PG_GETARG_POINTER(0)))));
}
PG_FUNCTION_INFO_V1(hashEllipsoid);

bool equalsEllipsoid(const Ellipsoid *ellipsoidA, const Ellipsoid *ellipsoidB) {
	return ellipsoidA->a == ellipsoidB->a && ellipsoidA->rf == ellipsoidB->rf && ellipsoidA->b == ellipsoidB->b && ellipsoidA->e1stSq == ellipsoidB->e1stSq && ellipsoidA->e2ndSq == ellipsoidB->e2ndSq && ellipsoidA->r1 == ellipsoidB->r1;
}

PGDLLEXPORT Datum equalsEllipsoidV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(equalsEllipsoid((Ellipsoid *) PG_GETARG_POINTER(0), (Ellipsoid *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(equalsEllipsoidV1);

PGDLLEXPORT Datum getAEllipsoid(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(((Ellipsoid *) PG_GETARG_POINTER(0))->a);
}
PG_FUNCTION_INFO_V1(getAEllipsoid);

PGDLLEXPORT Datum getRfEllipsoid(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(((Ellipsoid *) PG_GETARG_POINTER(0))->rf);
}
PG_FUNCTION_INFO_V1(getRfEllipsoid);

PGDLLEXPORT Datum getBEllipsoid(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(((Ellipsoid *) PG_GETARG_POINTER(0))->b);
}
PG_FUNCTION_INFO_V1(getBEllipsoid);

PGDLLEXPORT Datum getE1stSqEllipsoid(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(((Ellipsoid *) PG_GETARG_POINTER(0))->e1stSq);
}
PG_FUNCTION_INFO_V1(getE1stSqEllipsoid);

PGDLLEXPORT Datum getE2ndSqEllipsoid(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(((Ellipsoid *) PG_GETARG_POINTER(0))->e2ndSq);
}
PG_FUNCTION_INFO_V1(getE2ndSqEllipsoid);

PGDLLEXPORT Datum getR1Ellipsoid(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(((Ellipsoid *) PG_GETARG_POINTER(0))->r1);
}
PG_FUNCTION_INFO_V1(getR1Ellipsoid);
