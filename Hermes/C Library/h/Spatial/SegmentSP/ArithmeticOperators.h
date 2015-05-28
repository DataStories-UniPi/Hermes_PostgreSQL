/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_SEGMENTSP_ARITHMETIC_OPERATORS_H_
#define HE_SEGMENTSP_ARITHMETIC_OPERATORS_H_

#include "Definition.h"

SegmentSP *resizeSegmentSP(const SegmentSP *segment, float8 nlength, SegmentSP *result);
PGDLLEXPORT Datum resizeSegmentSPV1(PG_FUNCTION_ARGS);

#endif
