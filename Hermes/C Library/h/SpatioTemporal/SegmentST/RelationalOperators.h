/*
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the SegmentST data type.
 *
 * @see @ref SegmentST
 * @see @ref data_types_spatio_temp
 */

#ifndef HE_SEGMENTST_RELATIONAL_OPERATORS_H_
#define HE_SEGMENTST_RELATIONAL_OPERATORS_H_

#include "Definition.h"
#include "../PointST/Definition.h"
#include "../BoxST/Definition.h"
#include "../RangeST/Definition.h"

bool intersectsSegmentST_BoxST(const SegmentST *segment, const BoxST *box);
bool intersectsSegmentST_SegmentST(const SegmentST *segmentA, const SegmentST *segmentB);
PGDLLEXPORT Datum intersectsSegmentST_BoxSTV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum intersectsSegmentST_SegmentSTV1(PG_FUNCTION_ARGS);

bool containsSegmentST_PointST(const SegmentST *segment, const PointST *point);
PGDLLEXPORT Datum containsSegmentST_PointSTV1(PG_FUNCTION_ARGS);

bool containedSegmentST_BoxST(const SegmentST *segment, const BoxST *box);
bool containedProperlySegmentST_BoxST(const SegmentST *segment, const BoxST *box);
PGDLLEXPORT Datum containedSegmentST_BoxSTV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum containedProperlySegmentST_BoxSTV1(PG_FUNCTION_ARGS);

bool withinDistanceSegmentST_RangeST(const SegmentST *segment, const RangeST *range);
PGDLLEXPORT Datum withinDistanceSegmentST_RangeSTV1(PG_FUNCTION_ARGS);

#endif
