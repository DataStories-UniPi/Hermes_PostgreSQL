/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/SpatioTemporal/SegmentST/Casts.h"
#include "../../h/Spatial/SegmentSP/Casts.h"

BoxST *SegmentST2BoxST(const SegmentST *segment, BoxST *result) {
	BoxST *ret = result ? result : (BoxST *) palloc(sizeof(*ret));

	ret->t = segment->t;
	SegmentSP2BoxSP(&segment->sp, &ret->sp);

	return ret;
}

PGDLLEXPORT Datum SegmentST2BoxSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(SegmentST2BoxST((SegmentST *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(SegmentST2BoxSTV1);
