/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_UTILITIES_H_
#define	HE_UTILITIES_H_

#include <postgres.h>
#include <fmgr.h>
#include <utils/timestamp.h>

/* MACROS */

#ifdef HAVE_LONG_INT_64
#define	HE_FS_INT64		"%ld"
#define	HE_FS_UINT64	"%lu"
#else
#define	HE_FS_INT64		"%lld"
#define	HE_FS_UINT64	"%llu"
#endif

#define ELIMINATE_MINUS_ZERO(n)		((n) == (0.0) ? (0.0) : (n))

#ifdef HAVE_INT64_TIMESTAMP

#define getmsgTimestampHermes(msg)		(pq_getmsgint64((msg)))
#define sendTimestampHermes(buf, t)		(pq_sendint64((buf), (t)))

#define getmsgTimeOffsetHermes(msg)		(pq_getmsgint64((msg)))
#define sendTimeOffsetHermes(buf, t)	(pq_sendint64((buf), (t)))

#else

#define getmsgTimestampHermes(msg)		(pq_getmsgfloat8((msg)))
#define sendTimestampHermes(buf, t)		(pq_sendfloat8((buf), (ELIMINATE_MINUS_ZERO((t)))))

#define getmsgTimeOffsetHermes(msg)		(pq_getmsgfloat8((msg)))
#define sendTimeOffsetHermes(buf, t)	(pq_sendfloat8((buf), (ELIMINATE_MINUS_ZERO((t)))))

#endif

/* String Creation Without Buffer */

int asprintfHermes(char **ptr, const char *frm, ...);

/* Date Functions */

Timestamp epoch();

float8 interval2seconds(const Interval *interval);
Interval *seconds2interval(float8 seconds, Interval *result);

float8 timestamp2seconds(Timestamp ts);
Timestamp seconds2timestamp(float8 seconds);

Timestamp timestampPlusSeconds(Timestamp ts, float8 seconds);
Timestamp timestampMinusSeconds(Timestamp ts, float8 seconds);

Interval *differenceInInterval(Timestamp tsA, Timestamp tsB, Interval *result);
float8 differenceInSeconds(Timestamp tsA, Timestamp tsB);

Timestamp roundTimestamp(Timestamp ts);
PGDLLEXPORT Datum roundTimestampV1(PG_FUNCTION_ARGS);

/* Degrees-Radians Functions */

float8 Pi();
float8 dPi();

float8 radians2degrees(float8 rad);
float8 degrees2radians(float8 deg);

float8 degrees360(float8 degrees180);
PGDLLEXPORT Datum degrees360V1(PG_FUNCTION_ARGS);

/* Other Functions */

float8 toPrecision(float8 n, float8 precision);

/* Min-Max Functions */

float4 minFloat4(float4 a, float4 b);
float4 maxFloat4(float4 a, float4 b);
float8 minFloat8(float8 a, float8 b);
float8 maxFloat8(float8 a, float8 b);
int16 minInt16(int16 a, int16 b);
int16 maxInt16(int16 a, int16 b);
int32 minInt32(int32 a, int32 b);
int32 maxInt32(int32 a, int32 b);
int64 minInt64(int64 a, int64 b);
int64 maxInt64(int64 a, int64 b);
Timestamp minTimestamp(Timestamp a, Timestamp b);
Timestamp maxTimestamp(Timestamp a, Timestamp b);
Interval *minInterval(const Interval *a, const Interval *b);
Interval *maxInterval(const Interval *a, const Interval *b);

PGDLLEXPORT Datum minFloat4V1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum maxFloat4V1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum minFloat8V1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum maxFloat8V1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum minInt16V1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum maxInt16V1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum minInt32V1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum maxInt32V1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum minInt64V1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum maxInt64V1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum minTimestampV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum maxTimestampV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum minIntervalV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum maxIntervalV1(PG_FUNCTION_ARGS);

/* Dummy Functions */

PGDLLEXPORT Datum getFalse(PG_FUNCTION_ARGS);

#endif
