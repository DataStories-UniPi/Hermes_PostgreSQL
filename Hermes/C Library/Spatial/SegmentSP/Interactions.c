/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/Spatial/SegmentSP/Interactions.h"
#include "../../h/Spatial/SegmentSP/Properties.h"
#include "../../h/Spatial/SegmentSP/Casts.h"
#include "../../h/Spatial/BoxSP/Properties.h"
#include "../../h/Spatial/BoxSP/Interactions.h"
#include "../../h/Spatial/PointSP/Casts.h"
#include "../../h/Spatial/CRS/PointXY/Casts.h"
#include "../../h/Math/Vector2D/ArithmeticOperators.h"
#include "../../h/Math/Vector2D/Casts.h"
#include "../../h/SessionParameters.h"
#include <funcapi.h>
#include <access/htup_details.h>
#include <math.h>

static bool clip(float8 denom, float8 numer, float8 *t0, float8 *t1) {
	if (denom > 0.0) {
		if (numer > denom * *t1) {
			return false ;
		}
		if (numer > denom * *t0) {
			*t0 = numer / denom;
		}
		return true ;
	} else if (denom < 0.0) {
		if (numer > denom * *t0) {
			return false ;
		}
		if (numer > denom * *t1) {
			*t1 = numer / denom;
		}
		return true ;
	} else {
		return numer <= 0.0;
	}
}

int32 intersectionSegmentSP_BoxSP(const SegmentSP *segment, const BoxSP *box, bool solid, SegmentSP *commonSegment, PointSP *commonPoint) {
	PointXY tmpPoint;
	Vector2D tmpVector;

	Vector2D segmentCenter, segmentDirection;
	float8 t0, t1;
	float8 saveT0, saveT1;

	Vector2D boxCenter;
	float8 boxExtent[2];

	bool notAllClipped;

	Vector2D diff;

	int32 ret = 0;

	centerSegmentSP(segment, &tmpPoint);
	PointXY2Vector2D(&tmpPoint, &segmentCenter);

	directionSegmentSP(segment, &segmentDirection, &t1);
	t1 /= 2.0;
	t0 = -t1;

	centerBoxSP(box, &tmpPoint);
	PointXY2Vector2D(&tmpPoint, &boxCenter);

	boxExtent[0] = (box->h.x - box->l.x) / 2.0;
	boxExtent[1] = (box->h.y - box->l.y) / 2.0;

	subtractionVector2D(&segmentCenter, &boxCenter, &diff);

	saveT0 = t0;
	saveT1 = t1;
	notAllClipped = clip(+segmentDirection.x, -diff.x - boxExtent[0], &t0, &t1) && clip(-segmentDirection.x, +diff.x - boxExtent[0], &t0, &t1) && clip(+segmentDirection.y, -diff.y - boxExtent[1], &t0, &t1) && clip(-segmentDirection.y, +diff.y - boxExtent[1], &t0, &t1);

	if (notAllClipped && (solid || t0 != saveT0 || t1 != saveT1)) {
		if (t1 > t0) {
			ret = 2;

			if (commonSegment) {
				additionVector2D(&segmentCenter, multiplicationScalarVector2D(&segmentDirection, t0, &tmpVector), &tmpVector);
				Vector2D2PointSP(&tmpVector, &commonSegment->i);

				additionVector2D(&segmentCenter, multiplicationScalarVector2D(&segmentDirection, t1, &segmentDirection), &segmentCenter);
				Vector2D2PointSP(&segmentCenter, &commonSegment->e);
			}
		} else {
			ret = 1;

			if (commonPoint) {
				additionVector2D(&segmentCenter, multiplicationScalarVector2D(&segmentDirection, t0, &segmentDirection), &segmentCenter);
				Vector2D2PointSP(&segmentCenter, commonPoint);
			}
		}
	}

	return ret;
}

PGDLLEXPORT Datum intersectionSegmentSP_BoxSPV1(PG_FUNCTION_ARGS) {
	int32 nr_intersection_points;
	SegmentSP *commonSegment = NULL;
	PointSP *commonPoint = NULL;

	bool nulls[3];

	TupleDesc resultTupleDesc;
	Datum resultValues[3];
	HeapTuple result;

	commonSegment = (SegmentSP *) palloc(sizeof(*commonSegment));
	commonPoint = (PointSP *) palloc(sizeof(*commonPoint));

	nr_intersection_points = intersectionSegmentSP_BoxSP((SegmentSP *) PG_GETARG_POINTER(0), (BoxSP *) PG_GETARG_POINTER(1), PG_GETARG_BOOL(2), commonSegment, commonPoint);

	resultValues[0] = Int32GetDatum(nr_intersection_points);
	nulls[0] = false;
	if (nr_intersection_points == 1) {
		pfree(commonSegment);

		resultValues[2] = PointerGetDatum(commonPoint);

		nulls[1] = true;
		nulls[2] = false;
	} else if (nr_intersection_points == 2) {
		pfree(commonPoint);

		resultValues[1] = PointerGetDatum(commonSegment);

		nulls[1] = false;
		nulls[2] = true;
	} else {
		pfree(commonSegment);
		pfree(commonPoint);

		nulls[1] = true;
		nulls[2] = true;
	}

	if (get_call_result_type(fcinfo, NULL, &resultTupleDesc) != TYPEFUNC_COMPOSITE)
		ereport(ERROR, (errcode(ERRCODE_FEATURE_NOT_SUPPORTED), errmsg("function returning record called in context that cannot accept type record")));

	BlessTupleDesc(resultTupleDesc);
	result = heap_form_tuple(resultTupleDesc, resultValues, nulls);

	PG_RETURN_DATUM(HeapTupleGetDatum(result));
}
PG_FUNCTION_INFO_V1(intersectionSegmentSP_BoxSPV1);

int32 intersectionSegmentSP_SegmentSP(const SegmentSP *segmentA, const SegmentSP *segmentB, SegmentSP *commonSegment, PointSP *commonPoint) {
	PointXY tmpPoint;

	Vector2D aCenter, aDirection;
	float8 aExtent;

	Vector2D bCenter, bDirection;
	float8 bExtent;

	Vector2D diff;

	float8 D0DotPerpD1, diffNDotPerpD1;
	float8 parameter[2];

	BoxSP aBox, bBox, intersectionBox;

	int32 ret = 0;

	centerSegmentSP(segmentA, &tmpPoint);
	PointXY2Vector2D(&tmpPoint, &aCenter);

	centerSegmentSP(segmentB, &tmpPoint);
	PointXY2Vector2D(&tmpPoint, &bCenter);

	directionSegmentSP(segmentA, &aDirection, &aExtent);
	aExtent /= 2.0;

	directionSegmentSP(segmentB, &bDirection, &bExtent);
	bExtent /= 2.0;

	subtractionVector2D(&bCenter, &aCenter, &diff);

	D0DotPerpD1 = perpDotProductVector2D(&aDirection, &bDirection);
	if (fabs(D0DotPerpD1) >= zeroTolerance) {
		parameter[0] = perpDotProductVector2D(&diff, &bDirection) / D0DotPerpD1;
		parameter[1] = perpDotProductVector2D(&diff, &aDirection) / D0DotPerpD1;

		if (fabs(parameter[0]) <= aExtent && fabs(parameter[1]) <= bExtent) {
			ret = 1;

			if (commonPoint) {
				additionVector2D(&aCenter, multiplicationScalarVector2D(&aDirection, parameter[0], &aDirection), &aCenter);
				Vector2D2PointSP(&aCenter, commonPoint);
			}
		}
	} else {
		normalizeVector2D(&diff, &diff, NULL );

		diffNDotPerpD1 = perpDotProductVector2D(&diff, &bDirection);
		if (fabs(diffNDotPerpD1) < zeroTolerance) {
			SegmentSP2BoxSP(segmentA, &aBox);
			SegmentSP2BoxSP(segmentB, &bBox);

			if (intersectionBoxSP_BoxSP(&aBox, &bBox, &intersectionBox)) {
				if (isPunctalBoxSP(&intersectionBox)) {
					ret = 1;

					if (commonPoint)
						clonePointSP(&intersectionBox.l, commonPoint);
				} else {
					ret = 2;

					if (commonSegment) {
						if (equalsPointSP(&segmentA->i, &aBox.l) || equalsPointSP(&segmentA->e, &aBox.l)) {
							l_hBoxSP(&intersectionBox, commonSegment);
						} else {
							hl_lhBoxSP(&intersectionBox, commonSegment);
						}
					}
				}
			}
		}
	}

	return ret;
}

PGDLLEXPORT Datum intersectionSegmentSP_SegmentSPV1(PG_FUNCTION_ARGS) {
	int32 nr_intersection_points;
	SegmentSP *commonSegment = NULL;
	PointSP *commonPoint = NULL;

	bool nulls[3];

	TupleDesc resultTupleDesc;
	Datum resultValues[3];
	HeapTuple result;

	commonSegment = (SegmentSP *) palloc(sizeof(*commonSegment));
	commonPoint = (PointSP *) palloc(sizeof(*commonPoint));

	nr_intersection_points = intersectionSegmentSP_SegmentSP((SegmentSP *) PG_GETARG_POINTER(0), (SegmentSP *) PG_GETARG_POINTER(1), commonSegment, commonPoint);

	resultValues[0] = Int32GetDatum(nr_intersection_points);
	nulls[0] = false;
	if (nr_intersection_points == 1) {
		pfree(commonSegment);

		resultValues[2] = PointerGetDatum(commonPoint);

		nulls[1] = true;
		nulls[2] = false;
	} else if (nr_intersection_points == 2) {
		pfree(commonPoint);

		resultValues[1] = PointerGetDatum(commonSegment);

		nulls[1] = false;
		nulls[2] = true;
	} else {
		pfree(commonSegment);
		pfree(commonPoint);

		nulls[1] = true;
		nulls[2] = true;
	}

	if (get_call_result_type(fcinfo, NULL, &resultTupleDesc) != TYPEFUNC_COMPOSITE)
		ereport(ERROR, (errcode(ERRCODE_FEATURE_NOT_SUPPORTED), errmsg("function returning record called in context that cannot accept type record")));

	BlessTupleDesc(resultTupleDesc);
	result = heap_form_tuple(resultTupleDesc, resultValues, nulls);

	PG_RETURN_DATUM(HeapTupleGetDatum(result));
}
PG_FUNCTION_INFO_V1(intersectionSegmentSP_SegmentSPV1);

PointSP *closestPointSegmentSP_PointSP(const SegmentSP *segment, const PointSP *point, PointSP *result) {
	PointXY tmpPoint;

	Vector2D segmentCenter, segmentDirection;
	float8 segmentExtent;

	Vector2D pointVector;

	Vector2D diff;
	float8 param;

	Vector2D closestPoint;
	PointSP *ret = result ? result : (PointSP *) palloc(sizeof(*ret));

	centerSegmentSP(segment, &tmpPoint);
	PointXY2Vector2D(&tmpPoint, &segmentCenter);

	directionSegmentSP(segment, &segmentDirection, &segmentExtent);
	segmentExtent /= 2.0;

	PointSP2Vector2D(point, &pointVector);

	subtractionVector2D(&pointVector, &segmentCenter, &diff);
	param = dotProductVector2D(&segmentDirection, &diff);

	if (-segmentExtent < param) {
		if (param < segmentExtent) {
			additionVector2D(&segmentCenter, multiplicationScalarVector2D(&segmentDirection, param, &segmentDirection), &closestPoint);

			Vector2D2PointSP(&closestPoint, ret);
		} else {
			*ret = segment->e;
		}
	} else {
		*ret = segment->i;
	}

	return ret;
}

PGDLLEXPORT Datum closestPointSegmentSP_PointSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(closestPointSegmentSP_PointSP((SegmentSP *) PG_GETARG_POINTER(0), (PointSP *) PG_GETARG_POINTER(1), NULL));
}
PG_FUNCTION_INFO_V1(closestPointSegmentSP_PointSPV1);

int32 closestPointsSegmentSP_SegmentSP(const SegmentSP *segmentA, const SegmentSP *segmentB, PointSP *cpA, PointSP *cpB) {
	PointXY tmpPoint;

	Vector2D aCenter, aDirection;
	float8 aExtent;

	Vector2D bCenter, bDirection;
	float8 bExtent;

	Vector2D diff;

	float8 a01, b0, b1, det;
	float8 s0, s1, extDet0, extDet1, tmpS0, tmpS1;
	float8 invDet;
	float8 e0pe1, sign, b0Avr, lambda;

	Vector2D closestPointA, closestPointB;

	if (!cpA || !cpB)
		return 0;

	centerSegmentSP(segmentA, &tmpPoint);
	PointXY2Vector2D(&tmpPoint, &aCenter);

	centerSegmentSP(segmentB, &tmpPoint);
	PointXY2Vector2D(&tmpPoint, &bCenter);

	directionSegmentSP(segmentA, &aDirection, &aExtent);
	aExtent /= 2.0;

	directionSegmentSP(segmentB, &bDirection, &bExtent);
	bExtent /= 2.0;

	subtractionVector2D(&aCenter, &bCenter, &diff);
	a01 = -dotProductVector2D(&aDirection, &bDirection);
	b0 = dotProductVector2D(&diff, &aDirection);
	b1 = -dotProductVector2D(&diff, &bDirection);
	det = fabs(1.0 - a01 * a01);

	if (det >= zeroTolerance) {
		s0 = a01 * b1 - b0;
		s1 = a01 * b0 - b1;
		extDet0 = aExtent * det;
		extDet1 = bExtent * det;

		if (s0 >= -extDet0) {
			if (s0 <= extDet0) {
				if (s1 >= -extDet1) {
					if (s1 <= extDet1) {
						invDet = 1.0 / det;
						s0 *= invDet;
						s1 *= invDet;
					} else {
						s1 = bExtent;
						tmpS0 = -(a01 * s1 + b0);
						if (tmpS0 < -aExtent) {
							s0 = -aExtent;
						} else if (tmpS0 <= aExtent) {
							s0 = tmpS0;
						} else {
							s0 = aExtent;
						}
					}
				} else {
					s1 = -bExtent;
					tmpS0 = -(a01 * s1 + b0);
					if (tmpS0 < -aExtent) {
						s0 = -aExtent;
					} else if (tmpS0 <= aExtent) {
						s0 = tmpS0;
					} else {
						s0 = aExtent;
					}
				}
			} else {
				if (s1 >= -extDet1) {
					if (s1 <= extDet1) {
						s0 = aExtent;
						tmpS1 = -(a01 * s0 + b1);
						if (tmpS1 < -bExtent) {
							s1 = -bExtent;
						} else if (tmpS1 <= bExtent) {
							s1 = tmpS1;
						} else {
							s1 = bExtent;
						}
					} else {
						s1 = bExtent;
						tmpS0 = -(a01 * s1 + b0);
						if (tmpS0 < -aExtent) {
							s0 = -aExtent;
						} else if (tmpS0 <= aExtent) {
							s0 = tmpS0;
						} else {
							s0 = aExtent;
							tmpS1 = -(a01 * s0 + b1);
							if (tmpS1 < -bExtent) {
								s1 = -bExtent;
							} else if (tmpS1 <= bExtent) {
								s1 = tmpS1;
							} else {
								s1 = bExtent;
							}
						}
					}
				} else {
					s1 = -bExtent;
					tmpS0 = -(a01 * s1 + b0);
					if (tmpS0 < -aExtent) {
						s0 = -aExtent;
					} else if (tmpS0 <= aExtent) {
						s0 = tmpS0;
					} else {
						s0 = aExtent;
						tmpS1 = -(a01 * s0 + b1);
						if (tmpS1 > bExtent) {
							s1 = bExtent;
						} else if (tmpS1 >= -bExtent) {
							s1 = tmpS1;
						} else {
							s1 = -bExtent;
						}
					}
				}
			}
		} else {
			if (s1 >= -extDet1) {
				if (s1 <= extDet1) {
					s0 = -aExtent;
					tmpS1 = -(a01 * s0 + b1);
					if (tmpS1 < -bExtent) {
						s1 = -bExtent;
					} else if (tmpS1 <= bExtent) {
						s1 = tmpS1;
					} else {
						s1 = bExtent;
					}
				} else {
					s1 = bExtent;
					tmpS0 = -(a01 * s1 + b0);
					if (tmpS0 > aExtent) {
						s0 = aExtent;
					} else if (tmpS0 >= -aExtent) {
						s0 = tmpS0;
					} else {
						s0 = -aExtent;
						tmpS1 = -(a01 * s0 + b1);
						if (tmpS1 < -bExtent) {
							s1 = -bExtent;
						} else if (tmpS1 <= bExtent) {
							s1 = tmpS1;
						} else {
							s1 = bExtent;
						}
					}
				}
			} else {
				s1 = -bExtent;
				tmpS0 = -(a01 * s1 + b0);
				if (tmpS0 > aExtent) {
					s0 = aExtent;
				} else if (tmpS0 >= -aExtent) {
					s0 = tmpS0;
				} else {
					s0 = -aExtent;
					tmpS1 = -(a01 * s0 + b1);
					if (tmpS1 < -bExtent) {
						s1 = -bExtent;
					} else if (tmpS1 <= bExtent) {
						s1 = tmpS1;
					} else {
						s1 = bExtent;
					}
				}
			}
		}
	} else {
		e0pe1 = aExtent + bExtent;
		sign = (a01 > 0.0) ? -1.0 : 1.0;
		b0Avr = 0.5 * (b0 - sign * b1);
		lambda = -b0Avr;
		if (lambda < -e0pe1) {
			lambda = -e0pe1;
		} else if (lambda > e0pe1) {
			lambda = e0pe1;
		}

		s1 = -sign * lambda * bExtent / e0pe1;
		s0 = lambda + sign * s1;
	}

	additionVector2D(&aCenter, multiplicationScalarVector2D(&aDirection, s0, &aDirection), &closestPointA);
	additionVector2D(&bCenter, multiplicationScalarVector2D(&bDirection, s1, &bDirection), &closestPointB);

	Vector2D2PointSP(&closestPointA, cpA);
	Vector2D2PointSP(&closestPointB, cpB);

	return equalsPointSP(cpA, cpB) ? 1 : 2;
}

PGDLLEXPORT Datum closestPointsSegmentSP_SegmentSPV1(PG_FUNCTION_ARGS) {
	int32 nr_closest_points;
	PointSP *cpA = NULL, *cpB = NULL;

	bool nulls[3] = { false, false, false };

	TupleDesc resultTupleDesc;
	Datum resultValues[3];
	HeapTuple result;

	cpA = (PointSP *) palloc(sizeof(*cpA));
	cpB = (PointSP *) palloc(sizeof(*cpB));

	nr_closest_points = closestPointsSegmentSP_SegmentSP((SegmentSP *) PG_GETARG_POINTER(0), (SegmentSP *) PG_GETARG_POINTER(1), cpA, cpB);

	if (nr_closest_points == 1) {
		resultValues[0] = BoolGetDatum(true);
		resultValues[1] = PointerGetDatum(cpA);
		resultValues[2] = PointerGetDatum(cpB);
	} else if (nr_closest_points == 2) {
		resultValues[0] = BoolGetDatum(false);
		resultValues[1] = PointerGetDatum(cpA);
		resultValues[2] = PointerGetDatum(cpB);
	} else {
		pfree(cpA);
		pfree(cpB);

		nulls[0] = true;
		nulls[1] = true;
		nulls[2] = true;
	}

	if (get_call_result_type(fcinfo, NULL, &resultTupleDesc) != TYPEFUNC_COMPOSITE)
		ereport(ERROR, (errcode(ERRCODE_FEATURE_NOT_SUPPORTED), errmsg("function returning record called in context that cannot accept type record")));

	BlessTupleDesc(resultTupleDesc);
	result = heap_form_tuple(resultTupleDesc, resultValues, nulls);

	PG_RETURN_DATUM(HeapTupleGetDatum(result));
}
PG_FUNCTION_INFO_V1(closestPointsSegmentSP_SegmentSPV1);
