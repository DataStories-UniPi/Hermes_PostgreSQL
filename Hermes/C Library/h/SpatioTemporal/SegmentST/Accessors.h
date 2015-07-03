/*
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the SegmentST data type.
 *
 * @see @ref SegmentST
 * @see @ref data_types_spatio_temp
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
