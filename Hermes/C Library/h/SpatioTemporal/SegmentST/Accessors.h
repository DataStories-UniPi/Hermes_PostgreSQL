/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_SEGMENTST_ACCESSORS_H_
#define	HE_SEGMENTST_ACCESSORS_H_

#include "Definition.h"
#include "../PointST/Definition.h"

PointST *iSegmentST(const SegmentST *segment, PointST *result);
PointST *eSegmentST(const SegmentST *segment, PointST *result);
PGDLLEXPORT Datum iSegmentSTV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum eSegmentSTV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum pointsSegmentST(PG_FUNCTION_ARGS);

#endif
