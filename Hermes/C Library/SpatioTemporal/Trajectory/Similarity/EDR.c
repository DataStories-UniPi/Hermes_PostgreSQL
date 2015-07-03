/**
 *	@file
 *	@author Marios Vodas (mvodas@gmail.com).
 *	@brief The implementation of Trajectory
 *
 *	@details These functions are documented in the @ref Trajectory/Similarity.h
 *
 */

#include "../../../h/SpatioTemporal/Trajectory/Similarity.h"

#define HIT 0
#define MISS -1
#define GAP -1

float8 *EDR(const Trajectory *trajectoryA, const Trajectory *trajectoryB, float8 eps, float8 *result) {
	int32 s1, s2, s3;
	int32 **df = NULL;

	int32 rows, cols;
	int32 i, j;

	float8 *ret = NULL;

	if (trajectoryA->NrPoints > 0 && trajectoryB->NrPoints > 0) {
		rows = trajectoryB->NrPoints + 1;
		cols = trajectoryA->NrPoints + 1;

		df = (int32 **) palloc(rows * sizeof(*df));
		for (i = 0; i < rows; i++)
			df[i] = (int32 *) palloc0(cols * sizeof(**df));

		for (i = 0; i < cols; i++)
			df[0][i] = -i;

		for (i = 0; i < rows; i++)
			df[i][0] = -i;

		for (j = 1; j < cols; j++) {
			for (i = 1; i < rows; i++) {
				if (abs(trajectoryA->points[j - 1].sp.x - trajectoryB->points[i - 1].sp.x) <= eps && abs(trajectoryA->points[j - 1].sp.y - trajectoryB->points[i - 1].sp.y) <= eps)
					s1 = HIT;
				else
					s1 = MISS;

				s1 = df[i - 1][j - 1] + s1;
				s2 = df[i - 1][j] + GAP;
				s3 = df[i][j - 1] + GAP;

				df[i][j] = Max(s1, s2);
				df[i][j] = Max(df[i][j], s3);
			}
		}

		ret = result ? result : (float8 *) palloc(sizeof(*ret));
		*ret = 0 - df[rows - 1][cols - 1];

		for (i = 0; i < rows; i++)
			pfree(df[i]);
		pfree(df);
	}

	return ret;
}

PGDLLEXPORT Datum EDRV1(PG_FUNCTION_ARGS) {
	float8 ret;

	if (EDR((Trajectory *) PG_GETARG_VARLENA_P(0), (Trajectory *) PG_GETARG_VARLENA_P(1), PG_GETARG_FLOAT8(2), &ret) == NULL)
		PG_RETURN_NULL();

	PG_RETURN_FLOAT8(ret);
}
PG_FUNCTION_INFO_V1(EDRV1);
