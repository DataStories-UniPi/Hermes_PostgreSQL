/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_SEGMENTST_PROPERTIES_H_
#define HE_SEGMENTST_PROPERTIES_H_

#include "Definition.h"

bool isStoppedSegmentST(const SegmentST *segment, float8 tolerance);
PGDLLEXPORT Datum isStoppedSegmentSTV1(PG_FUNCTION_ARGS);

float8 averageSpeedSegmentST(const SegmentST *segment);
PGDLLEXPORT Datum averageSpeedSegmentSTV1(PG_FUNCTION_ARGS);

#endif
