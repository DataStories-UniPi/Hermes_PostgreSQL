/**
 *	@file
 *	@author Marios Vodas (mvodas@gmail.com).
 *	@brief The implementation of RangeST
 *
 *	@details These functions are documented in the @ref RangeST/Casts.h
 *
 */

#include "../../h/SpatioTemporal/RangeST/Casts.h"
#include "../../h/Temporal/RangeT/Casts.h"
#include "../../h/Spatial/RangeSP/Casts.h"

BoxST *RangeST2BoxST(const RangeST *range, BoxST *result) {
	BoxST *ret = result ? result : (BoxST *) palloc(sizeof(*ret));

	RangeT2Period(&range->t, &ret->t);
	RangeSP2BoxSP(&range->sp, &ret->sp);

	return ret;
}

PGDLLEXPORT Datum RangeST2BoxSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(RangeST2BoxST((RangeST *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(RangeST2BoxSTV1);
