/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/SpatioTemporal/BoxST/Casts.h"
#include "../../h/Temporal/Period/Casts.h"
#include "../../h/Spatial/BoxSP/Casts.h"

RangeST *BoxST2RangeST(const BoxST *box, RangeST *result) {
	RangeST *ret = result ? result : (RangeST *) palloc(sizeof(*ret));

	Period2RangeT(&box->t, &ret->t);
	BoxSP2RangeSP(&box->sp, &ret->sp);

	return ret;
}

PGDLLEXPORT Datum BoxST2RangeSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(BoxST2RangeST((BoxST *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(BoxST2RangeSTV1);
