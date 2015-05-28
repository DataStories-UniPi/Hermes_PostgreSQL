/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_BOXSP_PROPERTIES_H_
#define HE_BOXSP_PROPERTIES_H_

#include "Definition.h"
#include "../SegmentSP/Definition.h"
#include "../CRS/PointXY/Definition.h"

bool isPunctalBoxSP(const BoxSP *box);
PGDLLEXPORT Datum isPunctalBoxSPV1(PG_FUNCTION_ARGS);

int32 widthBoxSP(const BoxSP *box);
int32 heightBoxSP(const BoxSP *box);
int64 areaBoxSP(const BoxSP *box);
PGDLLEXPORT Datum widthBoxSPV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum heightBoxSPV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum areaBoxSPV1(PG_FUNCTION_ARGS);

PointXY *centerBoxSP(const BoxSP *box, PointXY *result);
float8 centerXBoxSP(const BoxSP *box);
float8 centerYBoxSP(const BoxSP *box);
PGDLLEXPORT Datum centerBoxSPV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum centerXBoxSPV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum centerYBoxSPV1(PG_FUNCTION_ARGS);

PointSP *lhBoxSP(const BoxSP *box, PointSP *result);
PointSP *hlBoxSP(const BoxSP *box, PointSP *result);
PGDLLEXPORT Datum lhBoxSPV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum hlBoxSPV1(PG_FUNCTION_ARGS);

SegmentSP *l_hBoxSP(const BoxSP *box, SegmentSP *result);
SegmentSP *h_lBoxSP(const BoxSP *box, SegmentSP *result);
PGDLLEXPORT Datum l_hBoxSPV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum h_lBoxSPV1(PG_FUNCTION_ARGS);

SegmentSP *lh_hlBoxSP(const BoxSP *box, SegmentSP *result);
SegmentSP *hl_lhBoxSP(const BoxSP *box, SegmentSP *result);
PGDLLEXPORT Datum lh_hlBoxSPV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum hl_lhBoxSPV1(PG_FUNCTION_ARGS);

SegmentSP *lh_hBoxSP(const BoxSP *box, SegmentSP *result);
SegmentSP *h_lhBoxSP(const BoxSP *box, SegmentSP *result);
PGDLLEXPORT Datum lh_hBoxSPV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum h_lhBoxSPV1(PG_FUNCTION_ARGS);

SegmentSP *h_hlBoxSP(const BoxSP *box, SegmentSP *result);
SegmentSP *hl_hBoxSP(const BoxSP *box, SegmentSP *result);
PGDLLEXPORT Datum h_hlBoxSPV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum hl_hBoxSPV1(PG_FUNCTION_ARGS);

SegmentSP *hl_lBoxSP(const BoxSP *box, SegmentSP *result);
SegmentSP *l_hlBoxSP(const BoxSP *box, SegmentSP *result);
PGDLLEXPORT Datum hl_lBoxSPV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum l_hlBoxSPV1(PG_FUNCTION_ARGS);

SegmentSP *l_lhBoxSP(const BoxSP *box, SegmentSP *result);
SegmentSP *lh_lBoxSP(const BoxSP *box, SegmentSP *result);
PGDLLEXPORT Datum l_lhBoxSPV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum lh_lBoxSPV1(PG_FUNCTION_ARGS);

#endif
