/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */
 
#include "../../../h/SpatioTemporal/SegmentST/Similarity.h"
#include "../../../h/SpatioTemporal/SegmentST/Interpolation.h"
#include "../../../h/Temporal/Period/Interactions.h"
#include "../../../h/Spatial/PointSP/Distance.h"
#include "../../../h/Utilities/Utilities.h"

float8 *TrapezoidalSegmentST(const SegmentST *segmentA, const SegmentST *segmentB, float8 *result) {
	Period period;
	PointSP pointA, pointB;
	float8 *ret = NULL;

	if (intersectionPeriod_Period(&segmentA->t, &segmentB->t, &period)) {
		ret = result ? result : (float8 *) palloc(sizeof(*ret));

		atInstantSPSegmentST(segmentA, period.i, false, &pointA);
		atInstantSPSegmentST(segmentB, period.i, false, &pointB);

		*ret = distancePointSP_PointSP(&pointA, &pointB);

		atInstantSPSegmentST(segmentA, period.e, false, &pointA);
		atInstantSPSegmentST(segmentB, period.e, false, &pointB);

		*ret += distancePointSP_PointSP(&pointA, &pointB);

		*ret = *ret * differenceInSeconds(period.e, period.i) / 2.0;
	}

	return ret;
}

PGDLLEXPORT Datum TrapezoidalSegmentSTV1(PG_FUNCTION_ARGS) {
	float8 ret;
	
	if (TrapezoidalSegmentST((SegmentST *) PG_GETARG_POINTER(0), (SegmentST *) PG_GETARG_POINTER(1), &ret) == NULL)
		PG_RETURN_NULL();

	PG_RETURN_FLOAT8(ret);
}
PG_FUNCTION_INFO_V1(TrapezoidalSegmentSTV1);
