/**
 *	@file
 *	@author Marios Vodas (mvodas@gmail.com).
 *	@brief The implementation of SegmentST
 *
 *	@details These functions are documented in the @ref SegmentST/Properties.h
 *
 */

#include "../../h/SpatioTemporal/SegmentST/Properties.h"
#include "../../h/Spatial/SegmentSP/Properties.h"
#include "../../h/Utilities/Utilities.h"

bool isStoppedSegmentST(const SegmentST *segment, float8 tolerance) {
	return lengthSegmentSP(&segment->sp) <= tolerance;
}

PGDLLEXPORT Datum isStoppedSegmentSTV1(PG_FUNCTION_ARGS) {
	float8 tolerance = PG_GETARG_FLOAT8(1);

	if (tolerance < 0.0) {
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("tolerance cannot be negative")));
	}

	PG_RETURN_BOOL(isStoppedSegmentST((SegmentST *) PG_GETARG_POINTER(0), tolerance));
}
PG_FUNCTION_INFO_V1(isStoppedSegmentSTV1);

float8 averageSpeedSegmentST(const SegmentST *segment) {
	return lengthSegmentSP(&segment->sp) / differenceInSeconds(segment->t.e, segment->t.i);
}

PGDLLEXPORT Datum averageSpeedSegmentSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(averageSpeedSegmentST((SegmentST *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(averageSpeedSegmentSTV1);
