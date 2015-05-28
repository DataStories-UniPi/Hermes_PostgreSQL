/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/Temporal/Period/Definition.h"
#include "../../h/Utilities/Utilities.h"
#include <libpq/pqformat.h>
#include <access/hash.h>

Period *clonePeriod(const Period *period, Period *result) {
	Period *ret = result ? result : (Period *) palloc(sizeof(*ret));

	*ret = *period;

	return ret;
}

PGDLLEXPORT Datum clonePeriodV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(clonePeriod((Period *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(clonePeriodV1);

Period *constructorPeriod(Timestamp i, Timestamp e, Period *result, char **error) {
	Period *ret = NULL;

	if (error)
		*error = NULL;

	if (i > e) {
		if (error)
			*error = "invalid time sequence for Period";
		return NULL ;
	}

	ret = result ? result : (Period *) palloc(sizeof(*ret));

	ret->i = i;
	ret->e = e;

	return ret;
}

PGDLLEXPORT Datum constructorPeriodV1(PG_FUNCTION_ARGS) {
	char *error;
	Period *result = constructorPeriod(PG_GETARG_TIMESTAMP(0), PG_GETARG_TIMESTAMP(1), NULL, &error);

	if (error)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("%s", error)));

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(constructorPeriodV1);

Period *inPeriod(const char *str, Period *result, char **error) {
	char is[27], es[27];
	Timestamp i, e;
	Period *ret = NULL;

	if (error)
		*error = NULL;

	if (sscanf(str, " '%26[^']' '%26[^']' ", is, es) != 2) {
		if (error)
			asprintfHermes(error, "invalid input syntax for Period: \"%s\"", str);
		return NULL ;
	}

	i = DatumGetTimestamp(DirectFunctionCall3(timestamp_in, CStringGetDatum(is), PointerGetDatum(0), Int32GetDatum(-1)));
	e = DatumGetTimestamp(DirectFunctionCall3(timestamp_in, CStringGetDatum(es), PointerGetDatum(0), Int32GetDatum(-1)));

	if (i > e) {
		if (error)
			*error = "invalid time sequence for Period";
		return NULL ;
	}

	ret = result ? result : (Period *) palloc(sizeof(*ret));

	ret->i = i;
	ret->e = e;

	return ret;
}

PGDLLEXPORT Datum inPeriodV1(PG_FUNCTION_ARGS) {
	char *error;
	Period *result = inPeriod(PG_GETARG_CSTRING(0), NULL, &error);

	if (error)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("%s", error)));

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(inPeriodV1);

char *outPeriod(const Period *period) {
	char *result = NULL;

	asprintfHermes(&result, "'%s' '%s'", DatumGetCString(DirectFunctionCall1(timestamp_out, TimestampGetDatum(period->i))), DatumGetCString(DirectFunctionCall1(timestamp_out, TimestampGetDatum(period->e))) );

	return result;
}

PGDLLEXPORT Datum outPeriodV1(PG_FUNCTION_ARGS) {
	PG_RETURN_CSTRING(outPeriod((Period *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(outPeriodV1);

PGDLLEXPORT Datum recvPeriod(PG_FUNCTION_ARGS) {
	StringInfo msg = (StringInfo) PG_GETARG_POINTER(0);
	Period *result = (Period *) palloc(sizeof(*result));

	result->i = getmsgTimestampHermes(msg);
	result->e = getmsgTimestampHermes(msg);

	if (result->i > result->e) {
		pfree(result);
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("invalid time sequence for Period")));
	}

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(recvPeriod);

bytea *Period2Bytea(const Period *period) {
	StringInfoData buf;

	pq_begintypsend(&buf);

	sendTimestampHermes(&buf, period->i);
	sendTimestampHermes(&buf, period->e);

	return pq_endtypsend(&buf);
}

PGDLLEXPORT Datum sendPeriod(PG_FUNCTION_ARGS) {
	PG_RETURN_BYTEA_P(Period2Bytea((Period *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(sendPeriod);

PGDLLEXPORT Datum hashPeriod(PG_FUNCTION_ARGS) {
	PG_RETURN_DATUM(DirectFunctionCall1(hashvarlena, PointerGetDatum(Period2Bytea((Period *) PG_GETARG_POINTER(0)))));
}
PG_FUNCTION_INFO_V1(hashPeriod);

bool equalsPeriod(const Period *periodA, const Period *periodB) {
	return periodA->i == periodB->i && periodA->e == periodB->e;
}

PGDLLEXPORT Datum equalsPeriodV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(equalsPeriod((Period *) PG_GETARG_POINTER(0),(Period *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(equalsPeriodV1);

PGDLLEXPORT Datum getIPeriod(PG_FUNCTION_ARGS) {
	PG_RETURN_TIMESTAMP(((Period *) PG_GETARG_POINTER(0))->i);
}
PG_FUNCTION_INFO_V1(getIPeriod);

PGDLLEXPORT Datum getEPeriod(PG_FUNCTION_ARGS) {
	PG_RETURN_TIMESTAMP(((Period *) PG_GETARG_POINTER(0))->e);
}
PG_FUNCTION_INFO_V1(getEPeriod);
