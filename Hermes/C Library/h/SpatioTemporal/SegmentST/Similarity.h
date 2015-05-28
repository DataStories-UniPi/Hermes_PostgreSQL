/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */
 
#ifndef HE_SEGMENTST_SIMILARITY_H_
#define	HE_SEGMENTST_SIMILARITY_H_

#include "Definition.h"

float8 *TrapezoidalSegmentST(const SegmentST *segmentA, const SegmentST *segmentB, float8 *result);
PGDLLEXPORT Datum TrapezoidalSegmentSTV1(PG_FUNCTION_ARGS);

#endif
