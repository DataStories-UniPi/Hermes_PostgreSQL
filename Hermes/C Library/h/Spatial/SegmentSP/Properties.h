/*
* @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the SegmentSP data type.
 *
 * @see @ref SegmentSP
 * @see @ref data_types_spatial
 */

#ifndef HE_SEGMENTSP_PROPERTIES_H_
#define HE_SEGMENTSP_PROPERTIES_H_

#include "Definition.h"
#include "../CRS/PointXY/Definition.h"
#include "../../Math/Vector2D/Definition.h"

float8 lengthSegmentSP(const SegmentSP *segment);
Vector2D *directionSegmentSP(const SegmentSP *segment, Vector2D *result, float8 *norm);
PGDLLEXPORT Datum lengthSegmentSPV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum directionSegmentSPV1(PG_FUNCTION_ARGS);

PointXY *centerSegmentSP(const SegmentSP *segment, PointXY *result);
float8 centerXSegmentSP(const SegmentSP *segment);
float8 centerYSegmentSP(const SegmentSP *segment);
PGDLLEXPORT Datum centerSegmentSPV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum centerXSegmentSPV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum centerYSegmentSPV1(PG_FUNCTION_ARGS);

#endif
