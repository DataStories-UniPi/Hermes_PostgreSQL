/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_SEGMENTSP_RELATIONAL_OPERATORS_H_
#define HE_SEGMENTSP_RELATIONAL_OPERATORS_H_

#include "Definition.h"
#include "../BoxSP/Definition.h"
#include "../RangeSP/Definition.h"

bool intersectsSegmentSP_BoxSP(const SegmentSP *segment, const BoxSP *box);
bool intersectsSegmentSP_SegmentSP(const SegmentSP *segmentA, const SegmentSP *segmentB);
PGDLLEXPORT Datum intersectsSegmentSP_BoxSPV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum intersectsSegmentSP_SegmentSPV1(PG_FUNCTION_ARGS);

bool containsSegmentSP_PointSP(const SegmentSP *segment, const PointSP *point);
PGDLLEXPORT Datum containsSegmentSP_PointSPV1(PG_FUNCTION_ARGS);

bool containedSegmentSP_BoxSP(const SegmentSP *segment, const BoxSP *box);
bool containedProperlySegmentSP_BoxSP(const SegmentSP *segment, const BoxSP *box);
PGDLLEXPORT Datum containedSegmentSP_BoxSPV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum containedProperlySegmentSP_BoxSPV1(PG_FUNCTION_ARGS);

bool withinDistanceSegmentSP_RangeSP(const SegmentSP *segment, const RangeSP *range);
PGDLLEXPORT Datum withinDistanceSegmentSP_RangeSPV1(PG_FUNCTION_ARGS);

#endif
