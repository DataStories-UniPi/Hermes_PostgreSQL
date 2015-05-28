/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../../h/SpatioTemporal/Trajectory/Similarity.h"
#include "../../../h/Spatial/PointSP/Distance.h"

float8 *EuclideanEnd(const Trajectory *trajectoryA, const Trajectory *trajectoryB, float8 *result) {
	float8 *ret = NULL;

	if (trajectoryA->NrPoints > 0 && trajectoryB->NrPoints > 0) {
		ret = result ? result : (float8 *) palloc(sizeof(*ret));
		
		*ret = distancePointSP_PointSP(&trajectoryA->points[trajectoryA->NrPoints - 1].sp, &trajectoryB->points[trajectoryB->NrPoints - 1].sp);
	}

	return ret;
}

PGDLLEXPORT Datum EuclideanEndV1(PG_FUNCTION_ARGS) {
	float8 ret;

	if (EuclideanEnd((Trajectory *) PG_GETARG_VARLENA_P(0), (Trajectory *) PG_GETARG_VARLENA_P(1), &ret) == NULL)
		PG_RETURN_NULL();

	PG_RETURN_FLOAT8(ret);
}
PG_FUNCTION_INFO_V1(EuclideanEndV1);
