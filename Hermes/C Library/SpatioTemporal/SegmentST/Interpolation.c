/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/SpatioTemporal/SegmentST/Interpolation.h"
#include "../../h/SpatioTemporal/SegmentST/Properties.h"
#include "../../h/Temporal/SessionParameters.h"
#include "../../h/Temporal/Period/RelationalOperators.h"
#include "../../h/Temporal/Period/Properties.h"
#include "../../h/Temporal/Period/Interactions.h"
#include "../../h/Spatial/SegmentSP/RelationalOperators.h"
#include "../../h/Spatial/SegmentSP/Properties.h"
#include "../../h/Spatial/SegmentSP/Interactions.h"
#include "../../h/Spatial/BoxSP/RelationalOperators.h"
#include "../../h/Spatial/PointSP/Distance.h"
#include "../../h/SessionParameters.h"
#include "../../h/Utilities/Utilities.h"
#include "../../h/Utilities/MSVCWorkarounds.h"
#include <funcapi.h>
#include <access/htup_details.h>

PointSP *atInstantSPSegmentST(const SegmentST *segment, Timestamp ts, bool checkContainment, PointSP *result) {
	float8 l, s;

	PointSP *ret = NULL;

	if (!checkContainment || containsPeriod_Timestamp(&segment->t, ts)) {
		ret = result ? result : (PointSP *) palloc(sizeof(*ret));

		l = lengthSegmentSP(&segment->sp);

		if (l >= zeroTolerance) {
			s = (l / differenceInSeconds(segment->t.e, segment->t.i)) * differenceInSeconds(ts, segment->t.i);

			ret->x = (int32) round(segment->sp.i.x + s * (segment->sp.e.x - segment->sp.i.x) / l);
			ret->y = (int32) round(segment->sp.i.y + s * (segment->sp.e.y - segment->sp.i.y) / l);
		} else {
			*ret = segment->sp.i;
		}
	}

	return ret;
}

PointST *atInstantSegmentST(const SegmentST *segment, Timestamp ts, bool checkContainment, PointST *result) {
	PointSP point;

	PointST *ret = NULL;

	if (atInstantSPSegmentST(segment, ts, checkContainment, &point)) {
		ret = result ? result : (PointST *) palloc(sizeof(*ret));

		ret->t = ts;
		ret->sp = point;
	}

	return ret;
}

PGDLLEXPORT Datum atInstantSPSegmentSTV1(PG_FUNCTION_ARGS) {
	PointSP *ret = atInstantSPSegmentST((SegmentST *) PG_GETARG_POINTER(0), PG_GETARG_TIMESTAMP(1), PG_GETARG_BOOL(2), NULL );

	if (ret == NULL)
		PG_RETURN_NULL();

	PG_RETURN_POINTER(ret);
}
PG_FUNCTION_INFO_V1(atInstantSPSegmentSTV1);

PGDLLEXPORT Datum atInstantSegmentSTV1(PG_FUNCTION_ARGS) {
	PointST *ret = atInstantSegmentST((SegmentST *) PG_GETARG_POINTER(0), PG_GETARG_TIMESTAMP(1), PG_GETARG_BOOL(2), NULL );

	if (ret == NULL)
		PG_RETURN_NULL();

	PG_RETURN_POINTER(ret);
}
PG_FUNCTION_INFO_V1(atInstantSegmentSTV1);

int32 atPeriodSPSegmentST(const SegmentST *segment, const Period *period, SegmentSP *segmentAt, PointSP *pointAt) {
	Period commonPeriod;

	int32 ret = 0;

	if (intersectionPeriod_Period(&segment->t, period, &commonPeriod)) {
		if (isStoppedSegmentST(segment, 0.0)) {
			if (pointAt)
				*pointAt = segment->sp.i;

			ret = 1;
		} else {
			if (isPunctalPeriod(&commonPeriod)) {
				if (pointAt)
					atInstantSPSegmentST(segment, commonPeriod.i, false, pointAt);

				ret = 1;
			} else {
				if (segmentAt) {
					if (containsPeriod_Period(period, &segment->t)) {
						*segmentAt = segment->sp;
					} else {
						atInstantSPSegmentST(segment, commonPeriod.i, false, &segmentAt->i);
						atInstantSPSegmentST(segment, commonPeriod.e, false, &segmentAt->e);
					}
				}

				ret = 2;
			}
		}
	}

	return ret;
}

int32 atPeriodSegmentST(const SegmentST *segment, const Period *period, SegmentST *segmentAt, PointST *pointAt) {
	Period commonPeriod;

	int32 ret = 0;

	if (intersectionPeriod_Period(&segment->t, period, &commonPeriod)) {
		if (isStoppedSegmentST(segment, 0.0)) {
			if (isPunctalPeriod(&commonPeriod)) {
				if (pointAt) {
					pointAt->t = commonPeriod.i;
					pointAt->sp = segment->sp.i;
				}

				ret = 1;
			} else {
				if (segmentAt) {
					segmentAt->t = commonPeriod;
					segmentAt->sp = segment->sp;
				}

				ret = 2;
			}
		} else {
			if (isPunctalPeriod(&commonPeriod)) {
				if (pointAt)
					atInstantSegmentST(segment, commonPeriod.i, false, pointAt);

				ret = 1;
			} else {
				if (segmentAt) {
					if (containsPeriod_Period(period, &segment->t)) {
						*segmentAt = *segment;
					} else {
						segmentAt->t = commonPeriod;
						atInstantSPSegmentST(segment, commonPeriod.i, false, &segmentAt->sp.i);
						atInstantSPSegmentST(segment, commonPeriod.e, false, &segmentAt->sp.e);
					}
				}

				ret = 2;
			}
		}
	}

	return ret;
}

PGDLLEXPORT Datum atPeriodSPSegmentSTV1(PG_FUNCTION_ARGS) {
	int32 nr_intersection_points;
	SegmentSP *segmentAt = NULL;
	PointSP *pointAt = NULL;

	bool nulls[3];

	TupleDesc resultTupleDesc;
	Datum resultValues[3];
	HeapTuple result;

	segmentAt = (SegmentSP *) palloc(sizeof(*segmentAt));
	pointAt = (PointSP *) palloc(sizeof(*pointAt));

	nr_intersection_points = atPeriodSPSegmentST((SegmentST *) PG_GETARG_POINTER(0), (Period *) PG_GETARG_POINTER(1), segmentAt, pointAt);

	resultValues[0] = Int32GetDatum(nr_intersection_points);
	nulls[0] = false;
	if (nr_intersection_points == 1) {
		pfree(segmentAt);

		resultValues[2] = PointerGetDatum(pointAt);

		nulls[1] = true;
		nulls[2] = false;
	} else if (nr_intersection_points == 2) {
		pfree(pointAt);

		resultValues[1] = PointerGetDatum(segmentAt);

		nulls[1] = false;
		nulls[2] = true;
	} else {
		pfree(segmentAt);
		pfree(pointAt);

		nulls[1] = true;
		nulls[2] = true;
	}

	if (get_call_result_type(fcinfo, NULL, &resultTupleDesc) != TYPEFUNC_COMPOSITE)
		ereport(ERROR, (errcode(ERRCODE_FEATURE_NOT_SUPPORTED), errmsg("function returning record called in context that cannot accept type record")));

	BlessTupleDesc(resultTupleDesc);
	result = heap_form_tuple(resultTupleDesc, resultValues, nulls);

	PG_RETURN_DATUM(HeapTupleGetDatum(result));
}
PG_FUNCTION_INFO_V1(atPeriodSPSegmentSTV1);

PGDLLEXPORT Datum atPeriodSegmentSTV1(PG_FUNCTION_ARGS) {
	int32 nr_intersection_points;
	SegmentST *segmentAt = NULL;
	PointST *pointAt = NULL;

	bool nulls[3];

	TupleDesc resultTupleDesc;
	Datum resultValues[3];
	HeapTuple result;

	segmentAt = (SegmentST *) palloc(sizeof(*segmentAt));
	pointAt = (PointST *) palloc(sizeof(*pointAt));

	nr_intersection_points = atPeriodSegmentST((SegmentST *) PG_GETARG_POINTER(0), (Period *) PG_GETARG_POINTER(1), segmentAt, pointAt);

	resultValues[0] = Int32GetDatum(nr_intersection_points);
	nulls[0] = false;
	if (nr_intersection_points == 1) {
		pfree(segmentAt);

		resultValues[2] = PointerGetDatum(pointAt);

		nulls[1] = true;
		nulls[2] = false;
	} else if (nr_intersection_points == 2) {
		pfree(pointAt);

		resultValues[1] = PointerGetDatum(segmentAt);

		nulls[1] = false;
		nulls[2] = true;
	} else {
		pfree(segmentAt);
		pfree(pointAt);

		nulls[1] = true;
		nulls[2] = true;
	}

	if (get_call_result_type(fcinfo, NULL, &resultTupleDesc) != TYPEFUNC_COMPOSITE)
		ereport(ERROR, (errcode(ERRCODE_FEATURE_NOT_SUPPORTED), errmsg("function returning record called in context that cannot accept type record")));

	BlessTupleDesc(resultTupleDesc);
	result = heap_form_tuple(resultTupleDesc, resultValues, nulls);

	PG_RETURN_DATUM(HeapTupleGetDatum(result));
}
PG_FUNCTION_INFO_V1(atPeriodSegmentSTV1);

Timestamp *atPointTSegmentST(const SegmentST *segment, const PointSP *point, bool checkContainment, Timestamp *result) {
	float8 v, s;

	Timestamp *ret = NULL;

	if (!checkContainment || containsSegmentSP_PointSP(&segment->sp, point)) {
		ret = result ? result : (Timestamp *) palloc(sizeof(*ret));

		v = averageSpeedSegmentST(segment);

		if (v >= zeroTolerance) {
			s = distancePointSP_PointSP(&segment->sp.i, point);
			*ret = roundTimestamp(timestampPlusSeconds(segment->t.i, s / v));
		} else {
			*ret = segment->t.i;
		}
	}

	return ret;
}

PointST *atPointSegmentST(const SegmentST *segment, const PointSP *point, bool checkContainment, PointST *result) {
	Timestamp ts;

	PointST *ret = NULL;

	if (atPointTSegmentST(segment, point, checkContainment, &ts)) {
		ret = result ? result : (PointST *) palloc(sizeof(*ret));

		ret->t = ts;
		ret->sp = *point;
	}

	return ret;
}

PGDLLEXPORT Datum atPointTSegmentSTV1(PG_FUNCTION_ARGS) {
	Timestamp ret;

	if (atPointTSegmentST((SegmentST *) PG_GETARG_POINTER(0), (PointSP *) PG_GETARG_POINTER(1), PG_GETARG_BOOL(2), &ret) == NULL)
		PG_RETURN_NULL();

	PG_RETURN_TIMESTAMP(ret);
}
PG_FUNCTION_INFO_V1(atPointTSegmentSTV1);

PGDLLEXPORT Datum atPointSegmentSTV1(PG_FUNCTION_ARGS) {
	PointST *ret = atPointSegmentST((SegmentST *) PG_GETARG_POINTER(0), (PointSP *) PG_GETARG_POINTER(1), PG_GETARG_BOOL(2), NULL );

	if (ret == NULL)
		PG_RETURN_NULL();

	PG_RETURN_POINTER(ret);
}
PG_FUNCTION_INFO_V1(atPointSegmentSTV1);

int32 atBoxTSegmentST(const SegmentST *segment, const BoxSP *box, Period *periodAt, Timestamp *tsAt) {
	int32 nr_intersection_points;
	SegmentSP commonSegment;
	PointSP commonPoint;

	int32 ret = 0;

	if ((nr_intersection_points = intersectionSegmentSP_BoxSP(&segment->sp, box, true, &commonSegment, &commonPoint))) {
		if (isStoppedSegmentST(segment, 0.0)) {
			if (periodAt)
				*periodAt = segment->t;

			ret = 2;
		} else {
			if (nr_intersection_points == 1) {
				if (tsAt)
					atPointTSegmentST(segment, &commonPoint, false, tsAt);

				ret = 1;
			} else if (nr_intersection_points == 2) {
				if (periodAt) {
					if (containedSegmentSP_BoxSP(&segment->sp, box)) {
						*periodAt = segment->t;
					} else {
						atPointTSegmentST(segment, &commonSegment.i, false, &periodAt->i);
						atPointTSegmentST(segment, &commonSegment.e, false, &periodAt->e);
					}
				}

				ret = 2;
			}
		}
	}

	return ret;
}

int32 atBoxSegmentST(const SegmentST *segment, const BoxSP *box, SegmentST *segmentAt, PointST *pointAt) {
	int32 nr_intersection_points;
	SegmentSP commonSegment;
	PointSP commonPoint;

	int32 ret = 0;

	if ((nr_intersection_points = intersectionSegmentSP_BoxSP(&segment->sp, box, true, &commonSegment, &commonPoint))) {
		if (isStoppedSegmentST(segment, 0.0)) {
			if (segmentAt)
				*segmentAt = *segment;

			ret = 2;
		} else {
			if (nr_intersection_points == 1) {
				if (pointAt)
					atPointSegmentST(segment, &commonPoint, false, pointAt);

				ret = 1;
			} else if (nr_intersection_points == 2) {
				if (segmentAt) {
					if (containedSegmentSP_BoxSP(&segment->sp, box)) {
						*segmentAt = *segment;
					} else {
						atPointTSegmentST(segment, &commonSegment.i, false, &segmentAt->t.i);
						atPointTSegmentST(segment, &commonSegment.e, false, &segmentAt->t.e);
						segmentAt->sp = commonSegment;
					}
				}

				ret = 2;
			}
		}
	}

	return ret;
}

PGDLLEXPORT Datum atBoxTSegmentSTV1(PG_FUNCTION_ARGS) {
	int32 nr_intersection_points;
	Period *periodAt = NULL;
	Timestamp tsAt;

	bool nulls[3];

	TupleDesc resultTupleDesc;
	Datum resultValues[3];
	HeapTuple result;

	periodAt = (Period *) palloc(sizeof(*periodAt));

	nr_intersection_points = atBoxTSegmentST((SegmentST *) PG_GETARG_POINTER(0), (BoxSP *) PG_GETARG_POINTER(1), periodAt, &tsAt);

	resultValues[0] = Int32GetDatum(nr_intersection_points);
	nulls[0] = false;
	if (nr_intersection_points == 1) {
		pfree(periodAt);

		resultValues[2] = TimestampGetDatum(tsAt);

		nulls[1] = true;
		nulls[2] = false;
	} else if (nr_intersection_points == 2) {
		resultValues[1] = PointerGetDatum(periodAt);

		nulls[1] = false;
		nulls[2] = true;
	} else {
		pfree(periodAt);

		nulls[1] = true;
		nulls[2] = true;
	}

	if (get_call_result_type(fcinfo, NULL, &resultTupleDesc) != TYPEFUNC_COMPOSITE)
		ereport(ERROR, (errcode(ERRCODE_FEATURE_NOT_SUPPORTED), errmsg("function returning record called in context that cannot accept type record")));

	BlessTupleDesc(resultTupleDesc);
	result = heap_form_tuple(resultTupleDesc, resultValues, nulls);

	PG_RETURN_DATUM(HeapTupleGetDatum(result));
}
PG_FUNCTION_INFO_V1(atBoxTSegmentSTV1);

PGDLLEXPORT Datum atBoxSegmentSTV1(PG_FUNCTION_ARGS) {
	int32 nr_intersection_points;
	SegmentST *segmentAt = NULL;
	PointST *pointAt = NULL;

	bool nulls[3];

	TupleDesc resultTupleDesc;
	Datum resultValues[3];
	HeapTuple result;

	segmentAt = (SegmentST *) palloc(sizeof(*segmentAt));
	pointAt = (PointST *) palloc(sizeof(*pointAt));

	nr_intersection_points = atBoxSegmentST((SegmentST *) PG_GETARG_POINTER(0), (BoxSP *) PG_GETARG_POINTER(1), segmentAt, pointAt);

	resultValues[0] = Int32GetDatum(nr_intersection_points);
	nulls[0] = false;
	if (nr_intersection_points == 1) {
		pfree(segmentAt);

		resultValues[2] = PointerGetDatum(pointAt);

		nulls[1] = true;
		nulls[2] = false;
	} else if (nr_intersection_points == 2) {
		pfree(pointAt);

		resultValues[1] = PointerGetDatum(segmentAt);

		nulls[1] = false;
		nulls[2] = true;
	} else {
		pfree(segmentAt);
		pfree(pointAt);

		nulls[1] = true;
		nulls[2] = true;
	}

	if (get_call_result_type(fcinfo, NULL, &resultTupleDesc) != TYPEFUNC_COMPOSITE)
		ereport(ERROR, (errcode(ERRCODE_FEATURE_NOT_SUPPORTED), errmsg("function returning record called in context that cannot accept type record")));

	BlessTupleDesc(resultTupleDesc);
	result = heap_form_tuple(resultTupleDesc, resultValues, nulls);

	PG_RETURN_DATUM(HeapTupleGetDatum(result));
}
PG_FUNCTION_INFO_V1(atBoxSegmentSTV1);

int32 atBoxSTSegmentST(const SegmentST *segment, const BoxST *box, SegmentST *segmentAt, PointST *pointAt) {
	int32 nr_intersection_points;

	SegmentST segmentAtBox;
	PointST pointAtBox;

	//SegmentST segmentAtPeriod;
	//PointST pointAtPeriod;

	int32 ret = 0;

	nr_intersection_points = atBoxSegmentST(segment, &box->sp, &segmentAtBox, &pointAtBox);

	if (nr_intersection_points == 1) {
		if (containsPeriod_Timestamp(&box->t, pointAtBox.t)) {
			if (pointAt)
				*pointAt = pointAtBox;

			ret = 1;
		}
	} else if (nr_intersection_points == 2) {
		ret = atPeriodSegmentST(&segmentAtBox, &box->t, segmentAt, pointAt);
	}

	/*
	 nr_intersection_points = atPeriodSegmentST(segment, &box->t, &segmentAtPeriod, &pointAtPeriod);

	 if (nr_intersection_points == 1) {
	 if (containsBoxSP_PointSP(&box->sp, &pointAtPeriod.sp)) {
	 if (pointAt)
	 *pointAt = pointAtPeriod;

	 ret = 1;
	 }
	 } else if (nr_intersection_points == 2) {
	 ret = atBoxSegmentST(&segmentAtPeriod, &box->sp, segmentAt, pointAt);
	 }
	 */

	return ret;
}

PGDLLEXPORT Datum atBoxSTSegmentSTV1(PG_FUNCTION_ARGS) {
	int32 nr_intersection_points;
	SegmentST *segmentAt = NULL;
	PointST *pointAt = NULL;

	bool nulls[3];

	TupleDesc resultTupleDesc;
	Datum resultValues[3];
	HeapTuple result;

	segmentAt = (SegmentST *) palloc(sizeof(*segmentAt));
	pointAt = (PointST *) palloc(sizeof(*pointAt));

	nr_intersection_points = atBoxSTSegmentST((SegmentST *) PG_GETARG_POINTER(0), (BoxST *) PG_GETARG_POINTER(1), segmentAt, pointAt);

	resultValues[0] = Int32GetDatum(nr_intersection_points);
	nulls[0] = false;
	if (nr_intersection_points == 1) {
		pfree(segmentAt);

		resultValues[2] = PointerGetDatum(pointAt);

		nulls[1] = true;
		nulls[2] = false;
	} else if (nr_intersection_points == 2) {
		pfree(pointAt);

		resultValues[1] = PointerGetDatum(segmentAt);

		nulls[1] = false;
		nulls[2] = true;
	} else {
		pfree(segmentAt);
		pfree(pointAt);

		nulls[1] = true;
		nulls[2] = true;
	}

	if (get_call_result_type(fcinfo, NULL, &resultTupleDesc) != TYPEFUNC_COMPOSITE)
		ereport(ERROR, (errcode(ERRCODE_FEATURE_NOT_SUPPORTED), errmsg("function returning record called in context that cannot accept type record")));

	BlessTupleDesc(resultTupleDesc);
	result = heap_form_tuple(resultTupleDesc, resultValues, nulls);

	PG_RETURN_DATUM(HeapTupleGetDatum(result));
}
PG_FUNCTION_INFO_V1(atBoxSTSegmentSTV1);

PointST *intersectionPointSegmentST_SegmentST(const SegmentST *segmentA, const SegmentST *segmentB, PointST *result) {
	int32 nr_intersection_points;
	SegmentSP commonSegment;
	PointSP commonPoint;

	Timestamp tsAtPointA1;
	Timestamp tsAtPointB1;

	//Timestamp tsAtPointA2;
	//Timestamp tsAtPointB2;

	PointST *ret = NULL;

	nr_intersection_points = intersectionSegmentSP_SegmentSP(&segmentA->sp, &segmentB->sp, &commonSegment, &commonPoint);

	if (nr_intersection_points == 1) {
		atPointTSegmentST(segmentA, &commonPoint, false, &tsAtPointA1);
		atPointTSegmentST(segmentB, &commonPoint, false, &tsAtPointB1);

		if (differenceInSeconds(tsAtPointA1, tsAtPointB1) < temporalTolerance) {
			ret = result ? result : (PointST *) palloc(sizeof(*ret));

			ret->t = tsAtPointA1;
			ret->sp = commonPoint;
		}
	} else if (nr_intersection_points == 2) { //This is when their segments are collinear. I don't know how to handle it so I return NULL.
		/*
		 atPointLightSegmentST(segmentA, &commonSegment.i, false, &tsAtPointA1);
		 atPointLightSegmentST(segmentB, &commonSegment.i, false, &tsAtPointB1);
		 atPointLightSegmentST(segmentA, &commonSegment.e, false, &tsAtPointA2);
		 atPointLightSegmentST(segmentB, &commonSegment.e, false, &tsAtPointB2);
		 */
	}

	return ret;
}

PGDLLEXPORT Datum intersectionPointSegmentST_SegmentSTV1(PG_FUNCTION_ARGS) {
	PointST *ret = intersectionPointSegmentST_SegmentST((SegmentST *) PG_GETARG_POINTER(0), (SegmentST *) PG_GETARG_POINTER(1), NULL );

	if (ret == NULL)
		PG_RETURN_NULL();

	PG_RETURN_POINTER(ret);
}
PG_FUNCTION_INFO_V1(intersectionPointSegmentST_SegmentSTV1);
