/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/Temporal/Period/Casts.h"

RangeT *Period2RangeT(const Period *period, RangeT *result) {
	Interval *tmp = NULL, *r = NULL;
	RangeT *ret = result ? result : (RangeT *) palloc(sizeof(*ret));

	tmp = DatumGetIntervalP(DirectFunctionCall2(timestamp_mi, TimestampGetDatum(period->e), TimestampGetDatum(period->i)));
	r = DatumGetIntervalP(DirectFunctionCall2(interval_div, IntervalPGetDatum(tmp), Float8GetDatum(2.0)));
	pfree(tmp);
	ret->r = *r;
	pfree(r);

	ret->c = DatumGetTimestamp(DirectFunctionCall2(timestamp_pl_interval, TimestampGetDatum(period->i), IntervalPGetDatum(r)));

	return ret;
}

PGDLLEXPORT Datum Period2RangeTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(Period2RangeT((Period *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(Period2RangeTV1);
