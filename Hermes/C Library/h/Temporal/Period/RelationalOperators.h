/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_PERIOD_RELATIONAL_OPERATORS_H_
#define HE_PERIOD_RELATIONAL_OPERATORS_H_

#include "Definition.h"

bool intersectsPeriod_Period(const Period *periodA, const Period *periodB);
PGDLLEXPORT Datum intersectsPeriod_PeriodV1(PG_FUNCTION_ARGS);

bool containsPeriod_Timestamp(const Period *period, Timestamp ts);
bool containsPeriod_Period(const Period *periodA, const Period *periodB);
PGDLLEXPORT Datum containsPeriod_TimestampV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum containsPeriod_PeriodV1(PG_FUNCTION_ARGS);

bool containsProperlyPeriod_Timestamp(const Period *period, Timestamp ts);
bool containsProperlyPeriod_Period(const Period *periodA, const Period *periodB);
PGDLLEXPORT Datum containsProperlyPeriod_TimestampV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum containsProperlyPeriod_PeriodV1(PG_FUNCTION_ARGS);

bool afterPeriod_Timestamp(const Period *period, Timestamp ts);
bool afterPeriod_Period(const Period *periodA, const Period *periodB);
PGDLLEXPORT Datum afterPeriod_TimestampV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum afterPeriod_PeriodV1(PG_FUNCTION_ARGS);

bool beforePeriod_Timestamp(const Period *period, Timestamp ts);
bool beforePeriod_Period(const Period *periodA, const Period *periodB);
PGDLLEXPORT Datum beforePeriod_TimestampV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum beforePeriod_PeriodV1(PG_FUNCTION_ARGS);

bool meetsPeriod_Timestamp(const Period *period, Timestamp ts);
bool meetsPeriod_Period(const Period *periodA, const Period *periodB);
PGDLLEXPORT Datum meetsPeriod_TimestampV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum meetsPeriod_PeriodV1(PG_FUNCTION_ARGS);

bool startsPeriod_Period(const Period *periodA, const Period *periodB);
bool finishesPeriod_Period(const Period *periodA, const Period *periodB);
PGDLLEXPORT Datum startsPeriod_PeriodV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum finishesPeriod_PeriodV1(PG_FUNCTION_ARGS);

#endif
