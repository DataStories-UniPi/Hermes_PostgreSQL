/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/Spatial/SegmentSP/ArithmeticOperators.h"
#include "../../h/Spatial/SegmentSP/Properties.h"
#include "../../h/Utilities/MSVCWorkarounds.h"
#include <math.h>

SegmentSP *resizeSegmentSP(const SegmentSP *segment, float8 nlength, SegmentSP *result) {
	float8 length;
	SegmentSP *ret = NULL;

	if (nlength <= 0.0)
		return NULL ;

	ret = result ? result : (SegmentSP *) palloc(sizeof(*ret));

	ret->i = segment->i;

	length = lengthSegmentSP(segment);

	if (length == 0.0) {
		ret->e = segment->e;
	} else {
		ret->e.x = (int32) round(segment->i.x + nlength * (segment->e.x - segment->i.x) / length);
		ret->e.y = (int32) round(segment->i.y + nlength * (segment->e.y - segment->i.y) / length);
	}

	return ret;
}

PGDLLEXPORT Datum resizeSegmentSPV1(PG_FUNCTION_ARGS) {
	SegmentSP *result = resizeSegmentSP((SegmentSP *) PG_GETARG_POINTER(0), PG_GETARG_FLOAT8(1), NULL );

	if (result == NULL )
		PG_RETURN_NULL()
		;

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(resizeSegmentSPV1);
