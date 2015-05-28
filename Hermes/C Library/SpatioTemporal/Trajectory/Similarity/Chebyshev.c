/*
 * Authors: Giannis Kostis (jkostis@gmail.com).
 */

#include "../../../h/SpatioTemporal/Trajectory/Similarity.h"
#include "../../../h/SpatioTemporal/Trajectory/Interpolation.h"
#include "../../../h/SpatioTemporal/Trajectory/SessionParameters.h"
#include "../../../h/Spatial/PointSP/Distance.h"

float8 *Chebyshev(const Trajectory *trajectoryA, const Trajectory *trajectoryB, int32 precision, float8 *result) {
	int32 curr_pointA, curr_pointB;
	PointST point;
	float8 dist, *ret = NULL;

	if (trajectoryA->NrPoints > 0 && trajectoryB->NrPoints > 0 && trajectoryA->points[0].t == trajectoryB->points[0].t && trajectoryA->points[trajectoryA->NrPoints - 1].t == trajectoryB->points[trajectoryB->NrPoints - 1].t) {
		ret = result ? result : (float8 *) palloc(sizeof(*ret));
		*ret = 0.0;

		curr_pointA = 0;
		curr_pointB = 0;

		while (curr_pointA < trajectoryA->NrPoints && curr_pointB < trajectoryB->NrPoints) {
			if (trajectoryA->points[curr_pointA].t < trajectoryB->points[curr_pointB].t) {
				atInstantTrajectory(PointerGetDatum(trajectoryB), trajectoryA->points[curr_pointA].t, precision, &point);

				dist = distanceChebyshevPointSP_PointSP(&trajectoryA->points[curr_pointA].sp, &point.sp);
				if (*ret < dist)
					*ret = dist;

				curr_pointA++;
			} else if (trajectoryA->points[curr_pointA].t > trajectoryB->points[curr_pointB].t) {
				atInstantTrajectory(PointerGetDatum(trajectoryA), trajectoryB->points[curr_pointB].t, precision, &point);

				dist = distanceChebyshevPointSP_PointSP(&trajectoryB->points[curr_pointB].sp, &point.sp);
				if (*ret < dist)
					*ret = dist;

				curr_pointB++;
			} else {
				dist = distanceChebyshevPointSP_PointSP(&trajectoryA->points[curr_pointA].sp, &trajectoryB->points[curr_pointB].sp);
				if (*ret < dist)
					*ret = dist;

				curr_pointA++;
				curr_pointB++;
			}
		}
	}

	return ret;
}

PGDLLEXPORT Datum ChebyshevV1(PG_FUNCTION_ARGS) {
	float8 ret;

	if (Chebyshev((Trajectory *) PG_GETARG_VARLENA_P(0), (Trajectory *) PG_GETARG_VARLENA_P(1), validateInterpolationPrecision(PG_GETARG_INT32(2)), &ret) == NULL)
		PG_RETURN_NULL();

	PG_RETURN_FLOAT8(ret);
}
PG_FUNCTION_INFO_V1(ChebyshevV1);
