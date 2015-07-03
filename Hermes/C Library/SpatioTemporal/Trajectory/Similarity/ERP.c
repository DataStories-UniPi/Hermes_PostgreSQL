/**
 *	@file
 *	@author Marios Vodas (mvodas@gmail.com).
 *	@brief The implementation of Trajectory
 *
 *	@details These functions are documented in the @ref Trajectory/Similarity.h
 *
 */

#include "../../../h/SpatioTemporal/Trajectory/Similarity.h"
#include "../../../h/Spatial/PointSP/Distance.h"

float8 *ERP(const Trajectory *trajectoryA, const Trajectory *trajectoryB, int32 lp, float8 *result) {
	PointSP gap;

	float8 diff_d, diff_h, diff_v;
	float8 s1, s2, s3;
	float8 **df = NULL;

	int32 rows, cols;
	int32 i, j;

	float8 *ret = NULL;

	if ((lp == 1 || lp == 2) && trajectoryA->NrPoints > 0 && trajectoryB->NrPoints > 0) {
		rows = trajectoryB->NrPoints + 1;
		cols = trajectoryA->NrPoints + 1;

		df = (float8 **) palloc(rows * sizeof(*df));
		for (i = 0; i < rows; i++)
			df[i] = (float8 *) palloc0(cols * sizeof(**df));

		gap.x = 0;
		gap.y = 0;

		for (i = 1; i < cols; i++) {
			for (j = 0; j < i; j++) {
				if (lp == 1)
					df[0][i] -= distanceManhattanPointSP_PointSP(&trajectoryA->points[j].sp, &gap); //to check
				else if (lp == 2)
					df[0][i] -= distancePointSP_PointSP(&trajectoryA->points[j].sp, &gap);
			}
		}

		for (i = 1; i < rows; i++) {
			for (j = 0; j < i; j++) {
				if (lp == 1)
					df[i][0] -= distanceManhattanPointSP_PointSP(&trajectoryB->points[j].sp, &gap); //to check
				else if (lp == 2)
					df[i][0] -= distancePointSP_PointSP(&trajectoryB->points[j].sp, &gap);
			}
		}

		for (j = 1; j < cols; j++) {
			for (i = 1; i < rows; i++) {
				if (lp == 1) {
					diff_d = distanceManhattanPointSP_PointSP(&trajectoryA->points[j - 1].sp, &trajectoryB->points[i - 1].sp); //to check
					diff_h = distanceManhattanPointSP_PointSP(&trajectoryA->points[j - 1].sp, &gap); //to check
					diff_v = distanceManhattanPointSP_PointSP(&trajectoryB->points[i - 1].sp, &gap); //to check
				} else if (lp == 2) {
					diff_d = distancePointSP_PointSP(&trajectoryA->points[j - 1].sp, &trajectoryB->points[i - 1].sp);
					diff_h = distancePointSP_PointSP(&trajectoryA->points[j - 1].sp, &gap);
					diff_v = distancePointSP_PointSP(&trajectoryB->points[i - 1].sp, &gap);
				}

				s1 = df[i - 1][j - 1] - diff_d;
				s2 = df[i - 1][j] - diff_v;
				s3 = df[i][j - 1] - diff_h;

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

PGDLLEXPORT Datum ERPV1(PG_FUNCTION_ARGS) {
	float8 ret;

	if (ERP((Trajectory *) PG_GETARG_VARLENA_P(0), (Trajectory *) PG_GETARG_VARLENA_P(1), PG_GETARG_INT32(2), &ret) == NULL)
		PG_RETURN_NULL();

	PG_RETURN_FLOAT8(ret);
}
PG_FUNCTION_INFO_V1(ERPV1);
