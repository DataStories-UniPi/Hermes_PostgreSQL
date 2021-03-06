/*
* @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the SegmentSP data type.
 *
 * @see @ref SegmentSP
 * @see @ref data_types_spatial
 */
#ifndef HE_SEGMENTSP_DISTANCE_H_
#define HE_SEGMENTSP_DISTANCE_H_

#include "Definition.h"

float8 distanceSegmentSP_PointSP(const SegmentSP *segment, const PointSP *point);
PGDLLEXPORT Datum distanceSegmentSP_PointSPV1(PG_FUNCTION_ARGS);

float8 distanceSegmentSP_SegmentSP(const SegmentSP *segmentA, const SegmentSP *segmentB);
PGDLLEXPORT Datum distanceSegmentSP_SegmentSPV1(PG_FUNCTION_ARGS);

#endif
