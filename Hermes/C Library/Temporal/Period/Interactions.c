/**
 *	@file
 *	@author Marios Vodas (mvodas@gmail.com).
 *	@brief The implementantion of Period
 *
 *	@details These functions are documented in the @ref Period/Interactions.h
 *
 */

#include "../../h/Temporal/Period/Interactions.h"

Period *intersectionPeriod_Period(const Period *periodA, const Period *periodB, Period *result) {
	Timestamp ti, te;
	Period *ret = NULL;

	ti = Max(periodA->i, periodB->i);
	te = Min(periodA->e, periodB->e);

	if (ti > te)
		return NULL;

	ret = result ? result : (Period *) palloc(sizeof(*ret));

	ret->i = ti;
	ret->e = te;

	return ret;
}

PGDLLEXPORT Datum intersectionPeriod_PeriodV1(PG_FUNCTION_ARGS) {
	Period *result = intersectionPeriod_Period((Period *) PG_GETARG_POINTER(0), (Period *) PG_GETARG_POINTER(1), NULL);

	if (result == NULL)
		PG_RETURN_NULL();

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(intersectionPeriod_PeriodV1);

Period *unionPeriod_Timestamp(const Period *period, Timestamp ts, Period *result) {
	Period *ret = result ? result : (Period *) palloc(sizeof(*ret));

	ret->i = Min(period->i, ts);
	ret->e = Max(period->e, ts);

	return ret;
}

PGDLLEXPORT Datum unionPeriod_TimestampV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(unionPeriod_Timestamp((Period *) PG_GETARG_POINTER(0), PG_GETARG_TIMESTAMP(1), NULL));
}
PG_FUNCTION_INFO_V1(unionPeriod_TimestampV1);

Period *unionPeriod_Period(const Period *periodA, const Period *periodB, Period *result) {
	Period *ret = result ? result : (Period *) palloc(sizeof(*ret));

	ret->i = Min(periodA->i, periodB->i);
	ret->e = Max(periodA->e, periodB->e);

	return ret;
}

PGDLLEXPORT Datum unionPeriod_PeriodV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(unionPeriod_Period((Period *) PG_GETARG_POINTER(0), (Period *) PG_GETARG_POINTER(1), NULL));
}
PG_FUNCTION_INFO_V1(unionPeriod_PeriodV1);
