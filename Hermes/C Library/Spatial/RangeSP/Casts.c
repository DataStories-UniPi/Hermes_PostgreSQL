/**
 *	@file
 *	@author Marios Vodas (mvodas@gmail.com).
 *	@brief The implementantion of RangeSP
 *
 *	@details These functions are documented in the @ref RangeSP/Casts.h
 *
 */

#include "../../h/Spatial/RangeSP/Casts.h"

BoxSP *RangeSP2BoxSP(const RangeSP *range, BoxSP *result) {
	BoxSP *ret = result ? result : (BoxSP *) palloc(sizeof(*ret));

	ret->l.x = range->c.x - range->r;
	ret->l.y = range->c.y - range->r;
	ret->h.x = range->c.x + range->r;
	ret->h.y = range->c.y + range->r;

	return ret;
}

PGDLLEXPORT Datum RangeSP2BoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(RangeSP2BoxSP((RangeSP *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(RangeSP2BoxSPV1);
