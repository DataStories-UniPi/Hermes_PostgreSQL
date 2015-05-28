/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/Spatial/SegmentSP/Distance.h"
#include "../../h/Spatial/SegmentSP/Properties.h"
#include "../../h/Spatial/PointSP/Casts.h"
#include "../../h/Spatial/CRS/PointXY/Casts.h"
#include "../../h/Math/Vector2D/ArithmeticOperators.h"
#include "../../h/SessionParameters.h"
#include <math.h>

float8 distanceSegmentSP_PointSP(const SegmentSP *segment, const PointSP *point) {
	PointXY tmpPoint;

	Vector2D segmentCenter, segmentDirection;
	float8 segmentExtent;

	Vector2D pointVector;

	Vector2D diff;
	float8 param;

	Vector2D closestPoint;

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
		} else {
			PointSP2Vector2D(&segment->e, &closestPoint);
		}
	} else {
		PointSP2Vector2D(&segment->i, &closestPoint);
	}

	subtractionVector2D(&closestPoint, &pointVector, &diff);

	return normVector2D(&diff);
}

PGDLLEXPORT Datum distanceSegmentSP_PointSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(distanceSegmentSP_PointSP((SegmentSP *) PG_GETARG_POINTER(0), (PointSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(distanceSegmentSP_PointSPV1);

float8 distanceSegmentSP_SegmentSP(const SegmentSP *segmentA, const SegmentSP *segmentB) {
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
	float8 ret;

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

	subtractionVector2D(&closestPointA, &closestPointB, &diff);
	ret = normVector2D(&diff);

	return ret >= zeroTolerance ? ret : 0.0;
}

PGDLLEXPORT Datum distanceSegmentSP_SegmentSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(distanceSegmentSP_SegmentSP((SegmentSP *) PG_GETARG_POINTER(0), (SegmentSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(distanceSegmentSP_SegmentSPV1);
