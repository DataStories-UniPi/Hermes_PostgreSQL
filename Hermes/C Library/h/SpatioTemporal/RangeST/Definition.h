/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_RANGEST_DEFINITION_H_
#define	HE_RANGEST_DEFINITION_H_

#include "../../Temporal/RangeT/Definition.h"
#include "../../Spatial/RangeSP/Definition.h"

typedef struct {
	RangeT t;
	RangeSP sp;
} RangeST;

RangeST *cloneRangeST(const RangeST *range, RangeST *result);
PGDLLEXPORT Datum cloneRangeSTV1(PG_FUNCTION_ARGS);

RangeST *constructorRangeST(const Interval *tr, Timestamp tc, int32 spr, int32 cx, int32 cy, RangeST *result, char **error);
PGDLLEXPORT Datum constructorRangeSTV1(PG_FUNCTION_ARGS);

RangeST *constructorHighRangeST(const RangeT *t, const RangeSP *sp, RangeST *result, char **error);
PGDLLEXPORT Datum constructorHighRangeSTV1(PG_FUNCTION_ARGS);

RangeST *inRangeST(const char *str, RangeST *result, char **error);
PGDLLEXPORT Datum inRangeSTV1(PG_FUNCTION_ARGS);

char *outRangeST(const RangeST *range);
PGDLLEXPORT Datum outRangeSTV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum recvRangeST(PG_FUNCTION_ARGS);

bytea *RangeST2Bytea(const RangeST *range);
PGDLLEXPORT Datum sendRangeST(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum hashRangeST(PG_FUNCTION_ARGS);

bool equalsRangeST(const RangeST *rangeA, const RangeST *rangeB);
PGDLLEXPORT Datum equalsRangeSTV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum getTRangeST(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getTrRangeST(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getTcRangeST(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getSpRangeST(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getSprRangeST(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getSpcRangeST(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getCxRangeST(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getCyRangeST(PG_FUNCTION_ARGS);

#endif
