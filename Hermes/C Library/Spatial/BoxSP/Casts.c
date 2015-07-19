/**
 *	@file
 *	@author Marios Vodas (mvodas@gmail.com).
 *	@brief The implementation of BoxSP
 *
 *	@details These functions are documented in the @ref BoxSP/Casts.h
 *
 */

#include "../../h/Spatial/BoxSP/Casts.h"
#include "../../h/Spatial/PointSP/Distance.h"
#include "../../h/Utilities/MSVCWorkarounds.h"
#include <math.h>

RangeSP *BoxSP2RangeSP(const BoxSP *box, RangeSP *result) {
	RangeSP *ret = result ? result : (RangeSP *) palloc(sizeof(*ret));

	ret->r = (int32) round(distancePointSP_PointSP(&box->l, &box->h) / 2.0);
	ret->c.x = (int32) round((box->l.x + box->h.x) / 2.0);
	ret->c.y = (int32) round((box->l.y + box->h.y) / 2.0);

	return ret;
}

PGDLLEXPORT Datum BoxSP2RangeSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(BoxSP2RangeSP((BoxSP *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(BoxSP2RangeSPV1);
