/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_PERIOD_PROPERTIES_H_
#define HE_PERIOD_PROPERTIES_H_

#include "Definition.h"

bool isPunctalPeriod(const Period *period);
PGDLLEXPORT Datum isPunctalPeriodV1(PG_FUNCTION_ARGS);

Interval *durationInIntervalPeriod(const Period *period, Interval *result);
PGDLLEXPORT Datum durationInIntervalPeriodV1(PG_FUNCTION_ARGS);

float8 durationPeriod(const Period *period);

#endif
