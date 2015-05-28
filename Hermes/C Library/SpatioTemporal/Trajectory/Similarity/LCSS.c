/*
 * Authors: Giannis Kostis (jkostis@gmail.com).
 */

#include "../../../h/SpatioTemporal/Trajectory/Similarity.h"
#include <math.h>

float8 stddevTrajectory(const Trajectory *trajectory) {
	int32 i;

	int64 sumX = 0, sumY = 0;
	float8 avgX, avgY;

	float8 f = 0.0;

	for (i = 0; i < trajectory->NrPoints; i++) {
		sumX += trajectory->points[i].sp.x;
		sumY += trajectory->points[i].sp.y;
	}
	
	avgX = (float8) sumX / trajectory->NrPoints;
	avgY = (float8) sumY / trajectory->NrPoints;

	for (i = 0; i < trajectory->NrPoints; i++)
		f += pow(trajectory->points[i].sp.x - avgX, 2.0) + pow(trajectory->points[i].sp.y - avgY, 2.0);

	return sqrt(f / trajectory->NrPoints);
}

bool match(int32 iA, int32 iB, const PointSP *a, const PointSP *b, int32 w, float8 eps) {
	int32 i_diff, x_diff, y_diff;

	i_diff = abs(iA - iB);
	x_diff = abs(a->x - b->x);
	y_diff = abs(a->y - b->y);

	if (i_diff <= w && x_diff < eps && y_diff < eps)
		return true;

	return false;
}

float8 *LCSS(const Trajectory *trajectoryA, const Trajectory *trajectoryB, int32 w, float8 w_percentage, float8 eps, float8 *result) {
	float8 stddevA, stddevB;
	int32 **df = NULL;

	int32 rows, cols;
	int32 i, j;

	float8 *ret = NULL;

	if (trajectoryA->NrPoints > 0 && trajectoryB->NrPoints > 0) {
		if (w < 0)
			w = (int32) (Max(trajectoryA->NrPoints, trajectoryB->NrPoints) * w_percentage);

		if (eps <= 0 || eps >= 1) {
			stddevA = stddevTrajectory(trajectoryA);
			stddevB = stddevTrajectory(trajectoryB);

			eps = Min(stddevA, stddevB);
		}

		rows = trajectoryA->NrPoints + 1;
		cols = trajectoryB->NrPoints + 1;

		df = (int32 **) palloc(rows * sizeof(*df));
		for (i = 0; i < rows; i++)
			df[i] = (int32 *) palloc0(cols * sizeof(**df));

		for (i = 1; i < rows; i++) {
			for (j = 1; j < cols; j++) {
				if (match(i, j, &trajectoryA->points[i - 1].sp, &trajectoryB->points[j - 1].sp, w, eps))
					df[i][j] = df[i - 1][j - 1] + 1;
				else if (df[i - 1][j] >= df[i][j - 1])
					df[i][j] = df[i - 1][j];
				else
					df[i][j] = df[i][j - 1];
			}
		}

		ret = result ? result : (float8 *) palloc(sizeof(*ret));
		*ret = 1.0 - ((float8) df[rows - 1][cols - 1]) / Min(rows - 1, cols - 1);

		for (i = 0; i < rows; i++)
			pfree(df[i]);
		pfree(df);
	}

	return ret;
}

PGDLLEXPORT Datum LCSSV1(PG_FUNCTION_ARGS) {
	float8 ret;

	if (LCSS((Trajectory *) PG_GETARG_VARLENA_P(0), (Trajectory *) PG_GETARG_VARLENA_P(1), PG_GETARG_INT32(2), PG_GETARG_FLOAT8(3), PG_GETARG_FLOAT8(4), &ret) == NULL)
		PG_RETURN_NULL();

	PG_RETURN_FLOAT8(ret);
}
PG_FUNCTION_INFO_V1(LCSSV1);
