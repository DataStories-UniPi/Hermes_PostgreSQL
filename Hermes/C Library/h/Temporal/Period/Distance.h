/*
* @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the Period data type.
 *
 * @see @ref Period
 * @see @ref data_types_temporal
 */
#ifndef HE_PERIOD_DISTANCE_H_
#define HE_PERIOD_DISTANCE_H_

#include "Definition.h"

Interval *distanceInIntervalPeriod_Timestamp(const Period *period, Timestamp ts, Interval *result);
PGDLLEXPORT Datum distanceInIntervalPeriod_TimestampV1(PG_FUNCTION_ARGS);

float8 distancePeriod_Timestamp(const Period *period, Timestamp ts);

Interval *distanceInIntervalPeriod_Period(const Period *periodA, const Period *periodB, Interval *result);
PGDLLEXPORT Datum distanceInIntervalPeriod_PeriodV1(PG_FUNCTION_ARGS);

float8 distancePeriod_Period(const Period *periodA, const Period *periodB);

#endif
