/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_RANGET_DEFINITION_H_
#define	HE_RANGET_DEFINITION_H_

#include <postgres.h>
#include <fmgr.h>
#include <utils/timestamp.h>

typedef struct {
	Interval r;
	Timestamp c;
} RangeT;

RangeT *cloneRangeT(const RangeT *range, RangeT *result);
PGDLLEXPORT Datum cloneRangeTV1(PG_FUNCTION_ARGS);

RangeT *constructorRangeT(const Interval *r, Timestamp c, RangeT *result, char **error);
PGDLLEXPORT Datum constructorRangeTV1(PG_FUNCTION_ARGS);

RangeT *inRangeT(const char *str, RangeT *result, char **error);
PGDLLEXPORT Datum inRangeTV1(PG_FUNCTION_ARGS);

char *outRangeT(const RangeT *range);
PGDLLEXPORT Datum outRangeTV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum recvRangeT(PG_FUNCTION_ARGS);

bytea *RangeT2Bytea(const RangeT *range);
PGDLLEXPORT Datum sendRangeT(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum hashRangeT(PG_FUNCTION_ARGS);

bool equalsRangeT(const RangeT *rangeA, const RangeT *rangeB);
PGDLLEXPORT Datum equalsRangeTV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum getRRangeT(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getCRangeT(PG_FUNCTION_ARGS);

#endif
