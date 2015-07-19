/*
* @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the Period data type.
 *
 * @see @ref Period
 * @see @ref data_types_temporal
 */

#ifndef HE_PERIOD_INTERACTIONS_H_
#define HE_PERIOD_INTERACTIONS_H_

#include "Definition.h"

Period *intersectionPeriod_Period(const Period *periodA, const Period *periodB, Period *result);
PGDLLEXPORT Datum intersectionPeriod_PeriodV1(PG_FUNCTION_ARGS);

Period *unionPeriod_Timestamp(const Period *period, Timestamp ts, Period *result);
PGDLLEXPORT Datum unionPeriod_TimestampV1(PG_FUNCTION_ARGS);

Period *unionPeriod_Period(const Period *periodA, const Period *periodB, Period *result);
PGDLLEXPORT Datum unionPeriod_PeriodV1(PG_FUNCTION_ARGS);

#endif
