/**
 *	@file
 *	@author Marios Vodas (mvodas@gmail.com).
 *	@brief The implementantion of BoxST
 *
 *	@details These functions are documented in the @ref BoxST/Editors.h
 *
 */

#include "../../h/SpatioTemporal/BoxST/Editors.h"
#include "../../h/Spatial/BoxSP/Properties.h"
#include "../../h/Spatial/SegmentSP/Casts.h"
#include "../../h/Spatial/LineSP/Properties.h"
#include "../../h/Utilities/Utilities.h"
#include "../../h/Utilities/MSVCWorkarounds.h"

BoxST *bufferBoxST(const BoxST *box, float8 tSize, int32 xSize, int32 ySize, BoxST *result) {
	BoxST *ret = result ? result : (BoxST *) palloc(sizeof(*ret));

	ret->t.i = timestampMinusSeconds(box->t.i, tSize);
	ret->t.e = timestampPlusSeconds(box->t.e, tSize);
	ret->sp.l.x = box->sp.l.x - xSize;
	ret->sp.l.y = box->sp.l.y - ySize;
	ret->sp.h.x = box->sp.h.x + xSize;
	ret->sp.h.y = box->sp.h.y + ySize;

	return ret;
}

PGDLLEXPORT Datum bufferBoxSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(bufferBoxST((BoxST *) PG_GETARG_POINTER(0), interval2seconds(PG_GETARG_INTERVAL_P(1)), PG_GETARG_INT32(2), PG_GETARG_INT32(3), NULL));
}
PG_FUNCTION_INFO_V1(bufferBoxSTV1);

BoxST *enlargeBoxST(const BoxST *box, float8 tSize, int32 spSize, BoxST *result) {
	float8 w, xSize, ySize;
	SegmentSP diagSegment;
	LineSP diagLine;

	BoxST *ret = result ? result : (BoxST *) palloc(sizeof(*ret));

	ret->t.i = timestampMinusSeconds(box->t.i, tSize);
	ret->t.e = timestampPlusSeconds(box->t.e, tSize);

	w = angleXXLineSP(SegmentSP2LineSP(l_hBoxSP(&box->sp, &diagSegment), &diagLine));

	xSize = spSize * cos(w);
	ySize = spSize * sin(w);

	ret->sp.l.x = (int32) round(box->sp.l.x - xSize);
	ret->sp.l.y = (int32) round(box->sp.l.y - ySize);
	ret->sp.h.x = (int32) round(box->sp.h.x + xSize);
	ret->sp.h.y = (int32) round(box->sp.h.y + ySize);

	return ret;
}

PGDLLEXPORT Datum enlargeBoxSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(enlargeBoxST((BoxST *) PG_GETARG_POINTER(0), interval2seconds(PG_GETARG_INTERVAL_P(1)), PG_GETARG_INT32(2), NULL));
}
PG_FUNCTION_INFO_V1(enlargeBoxSTV1);
