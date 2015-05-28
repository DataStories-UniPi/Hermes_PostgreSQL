/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/Temporal/Period/RelationalOperators.h"

bool intersectsPeriod_Period(const Period *periodA, const Period *periodB) {
	return (periodA->i <= periodB->e && periodB->e <= periodA->e) || (periodB->i <= periodA->e && periodA->e <= periodB->e);
}

PGDLLEXPORT Datum intersectsPeriod_PeriodV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(intersectsPeriod_Period((Period *) PG_GETARG_POINTER(0), (Period *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(intersectsPeriod_PeriodV1);

bool containsPeriod_Timestamp(const Period *period, Timestamp ts) {
	return period->i <= ts && ts <= period->e;
}

bool containsPeriod_Period(const Period *periodA, const Period *periodB) {
	return periodA->i <= periodB->i && periodB->e <= periodA->e;
}

PGDLLEXPORT Datum containsPeriod_TimestampV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(containsPeriod_Timestamp((Period *) PG_GETARG_POINTER(0), PG_GETARG_TIMESTAMP(1)));
}
PG_FUNCTION_INFO_V1(containsPeriod_TimestampV1);

PGDLLEXPORT Datum containsPeriod_PeriodV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(containsPeriod_Period((Period *) PG_GETARG_POINTER(0), (Period *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(containsPeriod_PeriodV1);

bool containsProperlyPeriod_Timestamp(const Period *period, Timestamp ts) {
	return period->i < ts && ts < period->e;
}

bool containsProperlyPeriod_Period(const Period *periodA, const Period *periodB) {
	return periodA->i < periodB->i && periodB->e < periodA->e;
}

PGDLLEXPORT Datum containsProperlyPeriod_TimestampV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(containsProperlyPeriod_Timestamp((Period *) PG_GETARG_POINTER(0), PG_GETARG_TIMESTAMP(1)));
}
PG_FUNCTION_INFO_V1(containsProperlyPeriod_TimestampV1);

PGDLLEXPORT Datum containsProperlyPeriod_PeriodV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(containsProperlyPeriod_Period((Period *) PG_GETARG_POINTER(0), (Period *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(containsProperlyPeriod_PeriodV1);

bool afterPeriod_Timestamp(const Period *period, Timestamp ts) {
	return period->i > ts;
}

bool afterPeriod_Period(const Period *periodA, const Period *periodB) {
	return periodA->i > periodB->e;
}

PGDLLEXPORT Datum afterPeriod_TimestampV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(afterPeriod_Timestamp((Period *) PG_GETARG_POINTER(0), PG_GETARG_TIMESTAMP(1)));
}
PG_FUNCTION_INFO_V1(afterPeriod_TimestampV1);

PGDLLEXPORT Datum afterPeriod_PeriodV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(afterPeriod_Period((Period *) PG_GETARG_POINTER(0), (Period *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(afterPeriod_PeriodV1);

bool beforePeriod_Timestamp(const Period *period, Timestamp ts) {
	return period->e < ts;
}

bool beforePeriod_Period(const Period *periodA, const Period *periodB) {
	return periodA->e < periodB->i;
}

PGDLLEXPORT Datum beforePeriod_TimestampV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(beforePeriod_Timestamp((Period *) PG_GETARG_POINTER(0), PG_GETARG_TIMESTAMP(1)));
}
PG_FUNCTION_INFO_V1(beforePeriod_TimestampV1);

PGDLLEXPORT Datum beforePeriod_PeriodV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(beforePeriod_Period((Period *) PG_GETARG_POINTER(0), (Period *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(beforePeriod_PeriodV1);

bool meetsPeriod_Timestamp(const Period *period, Timestamp ts) {
	return period->e == ts;
}

bool meetsPeriod_Period(const Period *periodA, const Period *periodB) {
	return periodA->e == periodB->i;
}

PGDLLEXPORT Datum meetsPeriod_TimestampV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(meetsPeriod_Timestamp((Period *) PG_GETARG_POINTER(0), PG_GETARG_TIMESTAMP(1)));
}
PG_FUNCTION_INFO_V1(meetsPeriod_TimestampV1);

PGDLLEXPORT Datum meetsPeriod_PeriodV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(meetsPeriod_Period((Period *) PG_GETARG_POINTER(0), (Period *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(meetsPeriod_PeriodV1);

bool startsPeriod_Period(const Period *periodA, const Period *periodB) {
	return periodA->i == periodB->i;
}

bool finishesPeriod_Period(const Period *periodA, const Period *periodB) {
	return periodA->e == periodB->e;
}

PGDLLEXPORT Datum startsPeriod_PeriodV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(startsPeriod_Period((Period *) PG_GETARG_POINTER(0), (Period *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(startsPeriod_PeriodV1);

PGDLLEXPORT Datum finishesPeriod_PeriodV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(finishesPeriod_Period((Period *) PG_GETARG_POINTER(0), (Period *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(finishesPeriod_PeriodV1);
