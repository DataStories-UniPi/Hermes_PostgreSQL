/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_SEGMENTSP_CASTS_H_
#define HE_SEGMENTSP_CASTS_H_

#include "Definition.h"
#include "../BoxSP/Definition.h"
#include "../LineSP/Definition.h"

BoxSP *SegmentSP2BoxSP(const SegmentSP *segment, BoxSP *result);
PGDLLEXPORT Datum SegmentSP2BoxSPV1(PG_FUNCTION_ARGS);

LineSP *SegmentSP2LineSP(const SegmentSP *segment, LineSP *result);
PGDLLEXPORT Datum SegmentSP2LineSPV1(PG_FUNCTION_ARGS);

#endif
