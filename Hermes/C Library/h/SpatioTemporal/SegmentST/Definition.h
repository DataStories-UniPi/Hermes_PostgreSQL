/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_SEGMENTST_DEFINITION_H_
#define	HE_SEGMENTST_DEFINITION_H_

#include "../../Temporal/Period/Definition.h"
#include "../../Spatial/SegmentSP/Definition.h"
#include "../PointST/Definition.h"

typedef struct {
	Period t;
	SegmentSP sp;
} SegmentST;

SegmentST *cloneSegmentST(const SegmentST *segment, SegmentST *result);
PGDLLEXPORT Datum cloneSegmentSTV1(PG_FUNCTION_ARGS);

SegmentST *constructorSegmentST(Timestamp ti, int32 ix, int32 iy, Timestamp te, int32 ex, int32 ey, SegmentST *result, char **error);
PGDLLEXPORT Datum constructorSegmentSTV1(PG_FUNCTION_ARGS);

SegmentST *constructorHighSegmentST(const PointST *i, const PointST *e, SegmentST *result, char **error);
PGDLLEXPORT Datum constructorHighSegmentSTV1(PG_FUNCTION_ARGS);

SegmentST *inSegmentST(const char *str, SegmentST *result, char **error);
PGDLLEXPORT Datum inSegmentSTV1(PG_FUNCTION_ARGS);

char *outSegmentST(const SegmentST *segment);
PGDLLEXPORT Datum outSegmentSTV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum recvSegmentST(PG_FUNCTION_ARGS);

bytea *SegmentST2Bytea(const SegmentST *segment);
PGDLLEXPORT Datum sendSegmentST(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum hashSegmentST(PG_FUNCTION_ARGS);

bool equalsSegmentST(const SegmentST *segmentA, const SegmentST *segmentB);
PGDLLEXPORT Datum equalsSegmentSTV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum getTSegmentST(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getTiSegmentST(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getTeSegmentST(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getSpSegmentST(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getISegmentST(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getIxSegmentST(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getIySegmentST(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getESegmentST(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getExSegmentST(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getEySegmentST(PG_FUNCTION_ARGS);

#endif
