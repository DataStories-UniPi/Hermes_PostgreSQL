/**
 *	@file
 *	@author Marios Vodas (mvodas@gmail.com).
 *	@brief The implementantion of Period
 *
 *	@details These functions are documented in the @ref Period/Distance.h
 *
 */

#include "../../h/Temporal/Period/Distance.h"
#include "../../h/Temporal/Period/RelationalOperators.h"
#include "../../h/Utilities/Utilities.h"

Interval *distanceInIntervalPeriod_Timestamp(const Period *period, Timestamp ts, Interval *result) {
	Interval *ret = NULL;

	if (afterPeriod_Timestamp(period, ts)) {
		ret = differenceInInterval(period->i, ts, result);
	} else if (beforePeriod_Timestamp(period, ts)) {
		ret = differenceInInterval(ts, period->e, result);
	} else {
		ret = DatumGetIntervalP(DirectFunctionCall3(interval_in, CStringGetDatum("00:00:00"), PointerGetDatum(0), Int32GetDatum(-1)));

		if (result) {
			*result = *ret;
			pfree(ret);
			ret = result;
		}
	}

	return ret;
}

PGDLLEXPORT Datum distanceInIntervalPeriod_TimestampV1(PG_FUNCTION_ARGS) {
	PG_RETURN_INTERVAL_P(distanceInIntervalPeriod_Timestamp((Period *) PG_GETARG_POINTER(0), PG_GETARG_TIMESTAMP(1), NULL));
}
PG_FUNCTION_INFO_V1(distanceInIntervalPeriod_TimestampV1);

float8 distancePeriod_Timestamp(const Period *period, Timestamp ts) {
	float8 distanceInSeconds;
	Interval *distanceInInterval = distanceInIntervalPeriod_Timestamp(period, ts, NULL );

	distanceInSeconds = interval2seconds(distanceInInterval);
	pfree(distanceInInterval);

	return distanceInSeconds;
}

Interval *distanceInIntervalPeriod_Period(const Period *periodA, const Period *periodB, Interval *result) {
	Interval *ret = NULL;

	if (afterPeriod_Period(periodA, periodB)) {
		ret = differenceInInterval(periodA->i, periodB->e, result);
	} else if (beforePeriod_Period(periodA, periodB)) {
		ret = differenceInInterval(periodB->i, periodA->e, result);
	} else {
		ret = DatumGetIntervalP(DirectFunctionCall3(interval_in, CStringGetDatum("00:00:00"), PointerGetDatum(0), Int32GetDatum(-1)));

		if (result) {
			*result = *ret;
			pfree(ret);
			ret = result;
		}
	}

	return ret;
}

PGDLLEXPORT Datum distanceInIntervalPeriod_PeriodV1(PG_FUNCTION_ARGS) {
	PG_RETURN_INTERVAL_P(distanceInIntervalPeriod_Period((Period *) PG_GETARG_POINTER(0), (Period *) PG_GETARG_POINTER(1), NULL));
}
PG_FUNCTION_INFO_V1(distanceInIntervalPeriod_PeriodV1);

float8 distancePeriod_Period(const Period *periodA, const Period *periodB) {
	float8 distanceInSeconds;
	Interval *distanceInInterval = distanceInIntervalPeriod_Period(periodA, periodB, NULL );

	distanceInSeconds = interval2seconds(distanceInInterval);
	pfree(distanceInInterval);

	return distanceInSeconds;
}
