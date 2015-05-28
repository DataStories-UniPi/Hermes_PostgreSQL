/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_SEGMENTST_CASTS_H_
#define HE_SEGMENTST_CASTS_H_

#include "Definition.h"
#include "../BoxST/Definition.h"

BoxST *SegmentST2BoxST(const SegmentST *segment, BoxST *result);
PGDLLEXPORT Datum SegmentST2BoxSTV1(PG_FUNCTION_ARGS);

#endif
