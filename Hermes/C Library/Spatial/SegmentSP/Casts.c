/**
 *	@file
 *	@author Marios Vodas (mvodas@gmail.com).
 *	@brief The implementantion of SegmentSP
 *
 *	@details These functions are documented in the @ref SegmentSP/Casts.h
 *
 */

#include "../../h/Spatial/SegmentSP/Casts.h"

BoxSP *SegmentSP2BoxSP(const SegmentSP *segment, BoxSP *result) {
	BoxSP *ret = result ? result : (BoxSP *) palloc(sizeof(*ret));

	ret->l.x = Min(segment->i.x, segment->e.x);
	ret->l.y = Min(segment->i.y, segment->e.y);
	ret->h.x = Max(segment->i.x, segment->e.x);
	ret->h.y = Max(segment->i.y, segment->e.y);

	return ret;
}

PGDLLEXPORT Datum SegmentSP2BoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(SegmentSP2BoxSP((SegmentSP *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(SegmentSP2BoxSPV1);

LineSP *SegmentSP2LineSP(const SegmentSP *segment, LineSP *result) {
	LineSP *ret = result ? result : (LineSP *) palloc(sizeof(*ret));

	ret->A = segment->i.y - segment->e.y;
	ret->B = segment->e.x - segment->i.x;
	ret->C = (float8) segment->e.y * segment->i.x - (float8) segment->i.y * segment->e.x;

	return ret;
}

PGDLLEXPORT Datum SegmentSP2LineSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(SegmentSP2LineSP((SegmentSP *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(SegmentSP2LineSPV1);
