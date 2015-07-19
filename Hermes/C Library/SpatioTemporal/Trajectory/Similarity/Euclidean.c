/**
 *	@file
 *	@author Marios Vodas (mvodas@gmail.com).
 *	@brief The implementation of Trajectory
 *
 *	@details These functions are documented in the @ref Trajectory/Similarity.h
 *
 */

#include "../../../h/SpatioTemporal/Trajectory/Similarity.h"
#include "../../../h/SpatioTemporal/Trajectory/Interpolation.h"
#include "../../../h/SpatioTemporal/Trajectory/SessionParameters.h"
#include "../../../h/Spatial/PointSP/Distance.h"
#include <math.h>

float8 *Euclidean(const Trajectory *trajectoryA, const Trajectory *trajectoryB, int32 precision, float8 *result) {
	int32 curr_pointA, curr_pointB;
	PointST point;
	float8 *ret = NULL;

	if (trajectoryA->NrPoints > 0 && trajectoryB->NrPoints > 0 && trajectoryA->points[0].t == trajectoryB->points[0].t && trajectoryA->points[trajectoryA->NrPoints - 1].t == trajectoryB->points[trajectoryB->NrPoints - 1].t) {
		ret = result ? result : (float8 *) palloc(sizeof(*ret));
		*ret = 0.0;

		curr_pointA = 0;
		curr_pointB = 0;

		while (curr_pointA < trajectoryA->NrPoints && curr_pointB < trajectoryB->NrPoints) {
			if (trajectoryA->points[curr_pointA].t < trajectoryB->points[curr_pointB].t) {
				atInstantTrajectory(PointerGetDatum(trajectoryB), trajectoryA->points[curr_pointA].t, precision, &point);

				*ret += sqDistancePointSP_PointSP(&trajectoryA->points[curr_pointA].sp, &point.sp);

				curr_pointA++;
			} else if (trajectoryA->points[curr_pointA].t > trajectoryB->points[curr_pointB].t) {
				atInstantTrajectory(PointerGetDatum(trajectoryA), trajectoryB->points[curr_pointB].t, precision, &point);

				*ret += sqDistancePointSP_PointSP(&trajectoryB->points[curr_pointB].sp, &point.sp);

				curr_pointB++;
			} else {
				*ret += sqDistancePointSP_PointSP(&trajectoryA->points[curr_pointA].sp, &trajectoryB->points[curr_pointB].sp);

				curr_pointA++;
				curr_pointB++;
			}
		}

		*ret = sqrt(*ret);
	}

	return ret;
}

PGDLLEXPORT Datum EuclideanV1(PG_FUNCTION_ARGS) {
	float8 ret;

	if (Euclidean((Trajectory *) PG_GETARG_VARLENA_P(0), (Trajectory *) PG_GETARG_VARLENA_P(1), validateInterpolationPrecision(PG_GETARG_INT32(2)), &ret) == NULL)
		PG_RETURN_NULL();

	PG_RETURN_FLOAT8(ret);
}
PG_FUNCTION_INFO_V1(EuclideanV1);
