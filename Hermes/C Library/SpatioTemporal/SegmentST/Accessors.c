/*
* Authors: Marios Vodas (mvodas@gmail.com).
*/

#include "../../h/SpatioTemporal/SegmentST/Accessors.h"
#include <funcapi.h>

PointST *iSegmentST(const SegmentST *segment, PointST *result) {
	PointST *ret = result ? result : (PointST *)palloc(sizeof(*ret));

	ret->t = segment->t.i;
	ret->sp = segment->sp.i;

	return ret;
}

PointST *eSegmentST(const SegmentST *segment, PointST *result) {
	PointST *ret = result ? result : (PointST *)palloc(sizeof(*ret));

	ret->t = segment->t.e;
	ret->sp = segment->sp.e;

	return ret;
}

PGDLLEXPORT Datum iSegmentSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(iSegmentST((SegmentST *)PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(iSegmentSTV1);

PGDLLEXPORT Datum eSegmentSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(eSegmentST((SegmentST *)PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(eSegmentSTV1);

PGDLLEXPORT Datum pointsSegmentST(PG_FUNCTION_ARGS) {
	FuncCallContext *funcctx;
	PointST *point = NULL;

	if (SRF_IS_FIRSTCALL()) {
		MemoryContext oldcontext;

		funcctx = SRF_FIRSTCALL_INIT();
		oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

		funcctx->user_fctx = PG_GETARG_POINTER(0);

		MemoryContextSwitchTo(oldcontext);
	}

	funcctx = SRF_PERCALL_SETUP();

	switch (funcctx->call_cntr) {
	case 0:
		point = iSegmentST((SegmentST *) funcctx->user_fctx, NULL);

		break;
	case 1:
		point = eSegmentST((SegmentST *) funcctx->user_fctx, NULL);

		break;
	default:
		SRF_RETURN_DONE(funcctx);
	}
	
	SRF_RETURN_NEXT(funcctx, PointerGetDatum(point));
}
PG_FUNCTION_INFO_V1(pointsSegmentST);
