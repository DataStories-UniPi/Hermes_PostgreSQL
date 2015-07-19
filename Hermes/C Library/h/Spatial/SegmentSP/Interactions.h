/*
* @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the SegmentSP data type.
 *
 * @see @ref SegmentSP
 * @see @ref data_types_spatial
 */

#ifndef HE_SEGMENTSP_INTERACTIONS_H_
#define HE_SEGMENTSP_INTERACTIONS_H_

#include "Definition.h"
#include "../BoxSP/Definition.h"

int32 intersectionSegmentSP_BoxSP(const SegmentSP *segment, const BoxSP *box, bool solid, SegmentSP *commonSegment, PointSP *commonPoint);
PGDLLEXPORT Datum intersectionSegmentSP_BoxSPV1(PG_FUNCTION_ARGS);

int32 intersectionSegmentSP_SegmentSP(const SegmentSP *segmentA, const SegmentSP *segmentB, SegmentSP *commonSegment, PointSP *commonPoint);
PGDLLEXPORT Datum intersectionSegmentSP_SegmentSPV1(PG_FUNCTION_ARGS);

PointSP *closestPointSegmentSP_PointSP(const SegmentSP *segment, const PointSP *point, PointSP *result);
PGDLLEXPORT Datum closestPointSegmentSP_PointSPV1(PG_FUNCTION_ARGS);

int32 closestPointsSegmentSP_SegmentSP(const SegmentSP *segmentA, const SegmentSP *segmentB, PointSP *cpA, PointSP *cpB);
PGDLLEXPORT Datum closestPointsSegmentSP_SegmentSPV1(PG_FUNCTION_ARGS);

#endif
