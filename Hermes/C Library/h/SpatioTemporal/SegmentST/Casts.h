/*
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the SegmentST data type.
 *
 * @see @ref SegmentST
 * @see @ref data_types_spatio_temp
 */

#ifndef HE_SEGMENTST_CASTS_H_
#define HE_SEGMENTST_CASTS_H_

#include "Definition.h"
#include "../BoxST/Definition.h"

BoxST *SegmentST2BoxST(const SegmentST *segment, BoxST *result);
PGDLLEXPORT Datum SegmentST2BoxSTV1(PG_FUNCTION_ARGS);

#endif
