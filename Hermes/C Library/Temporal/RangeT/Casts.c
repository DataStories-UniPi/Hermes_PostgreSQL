/**
 *	@file
 *	@author Marios Vodas (mvodas@gmail.com).
 *	@brief The implementantion of RangeT
 *
 *	@details These functions are documented in the @ref RangeT/Casts.h
 *
 */

#include "../../h/Temporal/RangeT/Casts.h"

Period *RangeT2Period(const RangeT *range, Period *result) {
	Period *ret = result ? result : (Period *) palloc(sizeof(*ret));

	ret->i = DatumGetTimestamp(DirectFunctionCall2(timestamp_mi_interval, TimestampGetDatum(range->c), IntervalPGetDatum(&range->r)));
	ret->e = DatumGetTimestamp(DirectFunctionCall2(timestamp_pl_interval, TimestampGetDatum(range->c), IntervalPGetDatum(&range->r)));

	return ret;
}

PGDLLEXPORT Datum RangeT2PeriodV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(RangeT2Period((RangeT *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(RangeT2PeriodV1);
