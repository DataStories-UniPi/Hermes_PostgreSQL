/*
 * Authors: Giannis Kostis (jkostis@gmail.com).
 */

#include "../../../h/SpatioTemporal/Trajectory/Similarity.h"
#include "../../../h/SpatioTemporal/Trajectory/Interpolation.h"
#include "../../../h/SpatioTemporal/Trajectory/SessionParameters.h"
#include "../../../h/Spatial/PointSP/Distance.h"
#include "../../../h/Utilities/Utilities.h"
#include <funcapi.h>
#include <access/htup_details.h>
#include <math.h>

float8 calc_integral(float8 a, float8 b, float8 c, float8 t) {
	float8 partA, partB, x, arcsinh;

	partA = (2 * a * t + b) / (4 * a);
	partA *= sqrt(a * t * t + b * t + c);

	partB = (b * b - 4 * a * c) / (8 * a * sqrt(a));

	if (partB != 0 && (4 * a * c - b * b) > 0) {
		x = (2 * a * t + b) / sqrt(4 * a * c - b * b);
		arcsinh = log(x + sqrt(x * x + 1));
		partB *= arcsinh;
	}

	return partA - partB;
}

float8 compute_segment_dissim(const PointST *a1, const PointST *a2, const PointST *b1, const PointST *b2, float8 *error) {
	float8 dissim;

	if (error) {
		float8 d1, d2;
		float8 a, b, c, d;
		float8 ab, cd;
		float8 afactor, bfactor;
		float8 ddt;
		float8 adcb;

		d1 = distancePointSP_PointSP(&a1->sp, &b1->sp);
		d2 = distancePointSP_PointSP(&a2->sp, &b2->sp);

		dissim = (d1 + d2) * differenceInSeconds(a2->t, a1->t) / 2;

		a = b2->sp.x - a2->sp.x;
		b = b1->sp.x - a1->sp.x;
		c = b2->sp.y - a2->sp.y;
		d = b1->sp.y - a1->sp.y;

		ab = a - b;
		cd = c - d;

		bfactor = 2 * (ab * b + cd * d);
		ddt = 0.0;

		if (bfactor >= 0) {
			ddt = d1;
		} else {
			afactor = ab * ab + cd * cd;

			if (afactor == 0) {
				// error = 0
			} else {
				if (bfactor <= -2 * afactor)
					ddt = d2;
				else
					ddt = sqrt(-bfactor / (4 * afactor) + (b * b + d * d));
			}

		}

		adcb = a * d - c * b;

		if (ddt != 0)
			*error += differenceInSeconds(a2->t, a1->t) / (12 * adcb * adcb) / (ddt * ddt * ddt);
	} else {
		float8 xtemp, ytemp;
		float8 xdiff, ydiff, tdiff;
		float8 a, b, c;
		float8 afactor, bfactor, cfactor;

		xtemp = b2->sp.x - b1->sp.x - a2->sp.x + a1->sp.x;
		ytemp = b2->sp.y - b1->sp.y - a2->sp.y + a1->sp.y;

		xdiff = b1->sp.x - a1->sp.x;
		ydiff = b1->sp.y - a1->sp.y;
		tdiff = differenceInSeconds(a2->t, a1->t);

		a = xtemp * xtemp + ytemp * ytemp;
		b = 2 * (xtemp * xdiff + ytemp * ydiff);
		c = xdiff * xdiff + ydiff * ydiff;

		afactor = a / (tdiff * tdiff);
		bfactor = b / tdiff - 2 * a * timestamp2seconds(a1->t) / (tdiff * tdiff);
		cfactor = a * timestamp2seconds(a1->t) * timestamp2seconds(a1->t) / (tdiff * tdiff) - b * timestamp2seconds(a1->t) / tdiff + c;

		if (afactor == 0) {
			dissim = sqrt(cfactor) / tdiff;
		} else {
			dissim = calc_integral(afactor, bfactor, cfactor, timestamp2seconds(a2->t));
			dissim -= calc_integral(afactor, bfactor, cfactor, timestamp2seconds(a1->t));
		}
	}

	return dissim;
}

float8 *DISSIM(const Trajectory *trajectoryA, const Trajectory *trajectoryB, int32 precision, float8 *result, float8 *error) {
	int32 curr_pointA, curr_pointB;
	PointST point, lastA, lastB;
	float8 *ret = NULL;

	if (trajectoryA->NrPoints > 0 && trajectoryB->NrPoints > 0 && trajectoryA->points[0].t == trajectoryB->points[0].t && trajectoryA->points[trajectoryA->NrPoints - 1].t == trajectoryB->points[trajectoryB->NrPoints - 1].t) {
		ret = result ? result : (float8 *) palloc(sizeof(*ret));
		*ret = 0.0;

		if (error)
			*error = 0.0;

		lastA = trajectoryA->points[0];
		lastB = trajectoryB->points[0];

		curr_pointA = 1;
		curr_pointB = 1;

		while (curr_pointA < trajectoryA->NrPoints && curr_pointB < trajectoryB->NrPoints) {
			if (trajectoryA->points[curr_pointA].t < trajectoryB->points[curr_pointB].t) {
				atInstantTrajectory(PointerGetDatum(trajectoryB), trajectoryA->points[curr_pointA].t, precision, &point);

				*ret += compute_segment_dissim(&lastA, &trajectoryA->points[curr_pointA], &lastB, &point, error);

				lastA = trajectoryA->points[curr_pointA];
				lastB = point;

				curr_pointA++;
			} else if (trajectoryA->points[curr_pointA].t > trajectoryB->points[curr_pointB].t) {
				atInstantTrajectory(PointerGetDatum(trajectoryA), trajectoryB->points[curr_pointB].t, precision, &point);

				*ret += compute_segment_dissim(&lastA, &point, &lastB, &trajectoryB->points[curr_pointB], error);

				lastA = point;
				lastB = trajectoryB->points[curr_pointB];

				curr_pointB++;
			} else {
				*ret += compute_segment_dissim(&lastA, &trajectoryA->points[curr_pointA], &lastB, &trajectoryB->points[curr_pointB], error);

				lastA = trajectoryA->points[curr_pointA];
				lastB = trajectoryB->points[curr_pointB];

				curr_pointA++;
				curr_pointB++;
			}
		}
	}

	return ret;
}

PGDLLEXPORT Datum DISSIMExact(PG_FUNCTION_ARGS) {
	float8 ret;

	if (DISSIM((Trajectory *) PG_GETARG_VARLENA_P(0), (Trajectory *) PG_GETARG_VARLENA_P(1), validateInterpolationPrecision(PG_GETARG_INT32(2)), &ret, NULL) == NULL)
		PG_RETURN_NULL();

	PG_RETURN_FLOAT8(ret);
}
PG_FUNCTION_INFO_V1(DISSIMExact);

PGDLLEXPORT Datum DISSIMApproximate(PG_FUNCTION_ARGS) {
	float8 dissim, error;

	bool nulls[2] = {false, false};

	TupleDesc resultTupleDesc;
	Datum resultValues[2];
	HeapTuple result;

	if (DISSIM((Trajectory *) PG_GETARG_VARLENA_P(0), (Trajectory *) PG_GETARG_VARLENA_P(1), validateInterpolationPrecision(PG_GETARG_INT32(2)), &dissim, &error) == NULL)
		PG_RETURN_NULL();

	resultValues[0] = Float8GetDatum(dissim);
	resultValues[1] = Float8GetDatum(error);

	if (get_call_result_type(fcinfo, NULL, &resultTupleDesc) != TYPEFUNC_COMPOSITE)
		ereport(ERROR, (errcode(ERRCODE_FEATURE_NOT_SUPPORTED), errmsg("function returning record called in context that cannot accept type record")));

	BlessTupleDesc(resultTupleDesc);
	result = heap_form_tuple(resultTupleDesc, resultValues, nulls);

	PG_RETURN_DATUM(HeapTupleGetDatum(result));
}
PG_FUNCTION_INFO_V1(DISSIMApproximate);
