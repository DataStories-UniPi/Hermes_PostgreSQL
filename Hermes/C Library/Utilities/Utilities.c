/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing utilities functions used by the Hermes Mod.
 *
 */

#include "../h/Utilities/Utilities.h"
#include "../h/Utilities/MSVCWorkarounds.h"
#include <utils/builtins.h>
#include <utils/int8.h>
#include <math.h>
#include <time.h>

#ifdef WIN32
#include <processthreadsapi.h>
#endif


int asprintfHermes(char **ptr, const char *frm, ...) {
	va_list ap;
	int count;
	char *buffer;

	va_start(ap, frm);

#ifdef WIN32
	count = _vscprintf(frm, ap);
#else
	count = vsnprintf(NULL, 0, frm, ap);
#endif

	va_end(ap);

	if (count >= 0) {
		buffer = (char *) palloc(count + 1);
		if (buffer == NULL )
			return -1;

		va_start(ap, frm);
		count = vsnprintf(buffer, count + 1, frm, ap);
		va_end(ap);

		if (count < 0) {
			pfree(buffer);
			return count;
		}
		*ptr = buffer;
	}

	return count;
}

Timestamp epoch() {
	return DatumGetTimestamp(DirectFunctionCall3(timestamp_in, CStringGetDatum("epoch"), PointerGetDatum(0), Int32GetDatum(-1))) ;
}

/** @brief Function converting the interval to seconds
 *
 *	@param[in] interval constant pointer to interval
 *	@returns seconds
 *
 */
float8 interval2seconds(const Interval *interval) {
	float8 result;
	text *epochText = cstring_to_text("epoch");

	result = DatumGetFloat8(DirectFunctionCall2(interval_part, PointerGetDatum(epochText), IntervalPGetDatum(interval)));
	pfree(epochText);

	return result;
}

/** @brief Function converting the interval to seconds in its place
 *
 *	@param[in] seconds Seconds
 *	@param[in] result pointer to result
 *
 *  @return seconds
 */
Interval *seconds2interval(float8 seconds, Interval *result) {
	Interval *ret = NULL;
	Interval *oneSecond = DatumGetIntervalP(DirectFunctionCall3(interval_in, CStringGetDatum("00:00:01"), PointerGetDatum(0), Int32GetDatum(-1)));

	ret = DatumGetIntervalP(DirectFunctionCall2(interval_mul, PointerGetDatum(oneSecond), Float8GetDatum(seconds)));
	pfree(oneSecond);

	if (result) {
		*result = *ret;
		pfree(ret);
		ret = result;
	}

	return ret;
}

/** @brief Function converting timestamp to seconds
 *
 * 	@param ts timestamp
 *  @return seconds
 */
float8 timestamp2seconds(Timestamp ts) {
	float8 result;
	text *epochText = cstring_to_text("epoch");

	result = DatumGetFloat8(DirectFunctionCall2(timestamp_part, PointerGetDatum(epochText), TimestampGetDatum(ts)));
	pfree(epochText);

	return result;
}

/** @brief Function converting seconds to timestamp
 *
 *	@param seconds
 *  @return timestamp
 */
Timestamp seconds2timestamp(float8 seconds) {
	Timestamp result;
	Interval *secondsInterval = seconds2interval(seconds, NULL );

	result = DatumGetTimestamp(DirectFunctionCall2(timestamp_pl_interval, TimestampGetDatum(epoch()), IntervalPGetDatum(secondsInterval)));
	pfree(secondsInterval);

	return result;
}

Timestamp timestampPlusSeconds(Timestamp ts, float8 seconds) {
	Timestamp result;
	Interval *secondsInterval = seconds2interval(seconds, NULL );

	result = DatumGetTimestamp(DirectFunctionCall2(timestamp_pl_interval, TimestampGetDatum(ts), IntervalPGetDatum(secondsInterval)));
	pfree(secondsInterval);

	return result;
}

Timestamp timestampMinusSeconds(Timestamp ts, float8 seconds) {
	Timestamp result;
	Interval *secondsInterval = seconds2interval(seconds, NULL );

	result = DatumGetTimestamp(DirectFunctionCall2(timestamp_mi_interval, TimestampGetDatum(ts), IntervalPGetDatum(secondsInterval)));
	pfree(secondsInterval);

	return result;
}

Interval *differenceInInterval(Timestamp tsA, Timestamp tsB, Interval *result) {
	Interval *ret = DatumGetIntervalP(DirectFunctionCall2(timestamp_mi, TimestampGetDatum(tsA), TimestampGetDatum(tsB)));

	if (result) {
		*result = *ret;
		pfree(ret);
		ret = result;
	}

	return ret;
}

float8 differenceInSeconds(Timestamp tsA, Timestamp tsB) {
	float8 result;
	Interval *differenceInterval = DatumGetIntervalP(DirectFunctionCall2(timestamp_mi, TimestampGetDatum(tsA), TimestampGetDatum(tsB)));

	result = interval2seconds(differenceInterval);
	pfree(differenceInterval);

	return result;
}

Timestamp roundTimestamp(Timestamp ts) {
	Timestamp tmpTs;
	float8 second;
	Interval *secondInterval = NULL;
	text *secondText = cstring_to_text("second");

	second = DatumGetFloat8(DirectFunctionCall2(timestamp_part, PointerGetDatum(secondText), TimestampGetDatum(ts)));
	second = round(second - floor(second));
	secondInterval = seconds2interval(second, NULL );

	tmpTs = DatumGetTimestamp(DirectFunctionCall2(timestamp_trunc, PointerGetDatum(secondText), TimestampGetDatum(ts)));
	pfree(secondText);

	tmpTs = DatumGetTimestamp(DirectFunctionCall2(timestamp_pl_interval, TimestampGetDatum(tmpTs), IntervalPGetDatum(secondInterval)));
	pfree(secondInterval);

	return tmpTs;
}

PGDLLEXPORT Datum roundTimestampV1(PG_FUNCTION_ARGS) {
	PG_RETURN_TIMESTAMP(roundTimestamp(PG_GETARG_TIMESTAMP(0)));
}
PG_FUNCTION_INFO_V1(roundTimestampV1);

float8 Pi() {
	return acos(-1.0);
}

float8 dPi() {
	return DatumGetFloat8(DirectFunctionCall1(dpi, (Datum) 0));
}

float8 radians2degrees(float8 rad) {
	return DatumGetFloat8(DirectFunctionCall1(degrees, Float8GetDatum(rad)));
}

float8 degrees2radians(float8 deg) {
	return DatumGetFloat8(DirectFunctionCall1(radians, Float8GetDatum(deg)));
}

float8 degrees360(float8 degrees180) {
	return (degrees180 < 0.0) ? 360.0 + degrees180 : degrees180;
}

PGDLLEXPORT Datum degrees360V1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(degrees360(PG_GETARG_FLOAT8(0)));
}
PG_FUNCTION_INFO_V1(degrees360V1);

float8 toPrecision(float8 n, float8 precision) {
	return round(n / precision) * precision;
}

float4 minFloat4(float4 a, float4 b) {
	return DatumGetFloat4(DirectFunctionCall2(float4smaller, Float4GetDatum(a), Float4GetDatum(b)));
}

float4 maxFloat4(float4 a, float4 b) {
	return DatumGetFloat4(DirectFunctionCall2(float4larger, Float4GetDatum(a), Float4GetDatum(b)));
}

float8 minFloat8(float8 a, float8 b) {
	return DatumGetFloat8(DirectFunctionCall2(float8smaller, Float8GetDatum(a), Float8GetDatum(b)));
}

float8 maxFloat8(float8 a, float8 b) {
	return DatumGetFloat8(DirectFunctionCall2(float8larger, Float8GetDatum(a), Float8GetDatum(b)));
}

int16 minInt16(int16 a, int16 b) {
	return DatumGetInt16(DirectFunctionCall2(int2smaller, Int16GetDatum(a), Int16GetDatum(b))) ;
}

int16 maxInt16(int16 a, int16 b) {
	return DatumGetInt16(DirectFunctionCall2(int2larger, Int16GetDatum(a), Int16GetDatum(b))) ;
}

int32 minInt32(int32 a, int32 b) {
	return DatumGetInt32(DirectFunctionCall2(int4smaller, Int32GetDatum(a), Int32GetDatum(b))) ;
}

int32 maxInt32(int32 a, int32 b) {
	return DatumGetInt32(DirectFunctionCall2(int4larger, Int32GetDatum(a), Int32GetDatum(b))) ;
}

int64 minInt64(int64 a, int64 b) {
	return DatumGetInt64(DirectFunctionCall2(int8smaller, Int64GetDatum(a), Int64GetDatum(b))) ;
}

int64 maxInt64(int64 a, int64 b) {
	return DatumGetInt64(DirectFunctionCall2(int8larger, Int64GetDatum(a), Int64GetDatum(b))) ;
}

Timestamp minTimestamp(Timestamp a, Timestamp b) {
	return DatumGetTimestamp(DirectFunctionCall2(timestamp_smaller, TimestampGetDatum(a), TimestampGetDatum(b))) ;
}

Timestamp maxTimestamp(Timestamp a, Timestamp b) {
	return DatumGetTimestamp(DirectFunctionCall2(timestamp_larger, TimestampGetDatum(a), TimestampGetDatum(b))) ;
}

Interval *minInterval(const Interval *a, const Interval *b) {
	return DatumGetIntervalP(DirectFunctionCall2(interval_smaller, IntervalPGetDatum(a), IntervalPGetDatum(b))) ;
}

Interval *maxInterval(const Interval *a, const Interval *b) {
	return DatumGetIntervalP(DirectFunctionCall2(interval_larger, IntervalPGetDatum(a), IntervalPGetDatum(b))) ;
}

PGDLLEXPORT Datum minFloat4V1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT4(minFloat4(PG_GETARG_FLOAT4(0), PG_GETARG_FLOAT4(1)));
}
PG_FUNCTION_INFO_V1(minFloat4V1);

PGDLLEXPORT Datum maxFloat4V1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT4(maxFloat4(PG_GETARG_FLOAT4(0), PG_GETARG_FLOAT4(1)));
}
PG_FUNCTION_INFO_V1(maxFloat4V1);

PGDLLEXPORT Datum minFloat8V1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(minFloat8(PG_GETARG_FLOAT8(0), PG_GETARG_FLOAT8(1)));
}
PG_FUNCTION_INFO_V1(minFloat8V1);

PGDLLEXPORT Datum maxFloat8V1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(maxFloat8(PG_GETARG_FLOAT8(0), PG_GETARG_FLOAT8(1)));
}
PG_FUNCTION_INFO_V1(maxFloat8V1);

PGDLLEXPORT Datum minInt16V1(PG_FUNCTION_ARGS) {
	PG_RETURN_INT16(minInt16(PG_GETARG_INT16(0), PG_GETARG_INT16(1)));
}
PG_FUNCTION_INFO_V1(minInt16V1);

PGDLLEXPORT Datum maxInt16V1(PG_FUNCTION_ARGS) {
	PG_RETURN_INT16(maxInt16(PG_GETARG_INT16(0), PG_GETARG_INT16(1)));
}
PG_FUNCTION_INFO_V1(maxInt16V1);

PGDLLEXPORT Datum minInt32V1(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(minInt32(PG_GETARG_INT32(0), PG_GETARG_INT32(1)));
}
PG_FUNCTION_INFO_V1(minInt32V1);

PGDLLEXPORT Datum maxInt32V1(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(maxInt32(PG_GETARG_INT32(0), PG_GETARG_INT32(1)));
}
PG_FUNCTION_INFO_V1(maxInt32V1);

PGDLLEXPORT Datum minInt64V1(PG_FUNCTION_ARGS) {
	PG_RETURN_INT64(minInt64(PG_GETARG_INT64(0), PG_GETARG_INT64(1)));
}
PG_FUNCTION_INFO_V1(minInt64V1);

PGDLLEXPORT Datum maxInt64V1(PG_FUNCTION_ARGS) {
	PG_RETURN_INT64(maxInt64(PG_GETARG_INT64(0), PG_GETARG_INT64(1)));
}
PG_FUNCTION_INFO_V1(maxInt64V1);

PGDLLEXPORT Datum minTimestampV1(PG_FUNCTION_ARGS) {
	PG_RETURN_TIMESTAMP(minTimestamp(PG_GETARG_TIMESTAMP(0), PG_GETARG_TIMESTAMP(1)));
}
PG_FUNCTION_INFO_V1(minTimestampV1);

PGDLLEXPORT Datum maxTimestampV1(PG_FUNCTION_ARGS) {
	PG_RETURN_TIMESTAMP(maxTimestamp(PG_GETARG_TIMESTAMP(0), PG_GETARG_TIMESTAMP(1)));
}
PG_FUNCTION_INFO_V1(maxTimestampV1);

PGDLLEXPORT Datum minIntervalV1(PG_FUNCTION_ARGS) {
	PG_RETURN_INTERVAL_P(minInterval(PG_GETARG_INTERVAL_P(0), PG_GETARG_INTERVAL_P(1)));
}
PG_FUNCTION_INFO_V1(minIntervalV1);

PGDLLEXPORT Datum maxIntervalV1(PG_FUNCTION_ARGS) {
	PG_RETURN_INTERVAL_P(maxInterval(PG_GETARG_INTERVAL_P(0), PG_GETARG_INTERVAL_P(1)));
}
PG_FUNCTION_INFO_V1(maxIntervalV1);

PGDLLEXPORT Datum getFalse(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(false);
}
PG_FUNCTION_INFO_V1(getFalse);
