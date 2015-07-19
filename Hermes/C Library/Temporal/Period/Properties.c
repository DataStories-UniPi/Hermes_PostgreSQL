/**
 *	@file
 *	@author Marios Vodas (mvodas@gmail.com).
 *	@brief The implementantion of Period
 *
 *	@details These functions are documented in the @ref Period/Properties.h
 *
 */

#include "../../h/Temporal/Period/Properties.h"
#include "../../h/Utilities/Utilities.h"

bool isPunctalPeriod(const Period *period) {
	return period->e == period->i;
}

PGDLLEXPORT Datum isPunctalPeriodV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(isPunctalPeriod((Period *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(isPunctalPeriodV1);

Interval *durationInIntervalPeriod(const Period *period, Interval *result) {
	return differenceInInterval(period->e, period->i, result);
}

PGDLLEXPORT Datum durationInIntervalPeriodV1(PG_FUNCTION_ARGS) {
	PG_RETURN_INTERVAL_P(durationInIntervalPeriod((Period *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(durationInIntervalPeriodV1);

float8 durationPeriod(const Period *period) {
	float8 durationInSeconds;
	Interval *durationInInterval = durationInIntervalPeriod(period, NULL );

	durationInSeconds = interval2seconds(durationInInterval);
	pfree(durationInInterval);

	return durationInSeconds;
}
