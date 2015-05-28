/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_TIMESTAMP_RELATIONAL_OPERATORS_H_
#define HE_TIMESTAMP_RELATIONAL_OPERATORS_H_

#include "../Period/Definition.h"

bool meetsTimestamp_Period(Timestamp ts, const Period *period);
PGDLLEXPORT Datum meetsTimestamp_PeriodV1(PG_FUNCTION_ARGS);

#endif
