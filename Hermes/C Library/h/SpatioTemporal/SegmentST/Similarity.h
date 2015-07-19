/*
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the SegmentST data type.
 *
 * @see @ref SegmentST
 * @see @ref data_types_spatio_temp
 */
 
#ifndef HE_SEGMENTST_SIMILARITY_H_
#define	HE_SEGMENTST_SIMILARITY_H_

#include "Definition.h"

float8 *TrapezoidalSegmentST(const SegmentST *segmentA, const SegmentST *segmentB, float8 *result);
PGDLLEXPORT Datum TrapezoidalSegmentSTV1(PG_FUNCTION_ARGS);

#endif
