/*
 * Authors: Giannis Kostis (jkostis@gmail.com).
 */

#include "../../../h/SpatioTemporal/Trajectory/Similarity.h"
#include "../../../h/Spatial/PointSP/Distance.h"
#include <math.h>
#include <float.h>

float8 *DTW(const Trajectory *trajectoryA, const Trajectory *trajectoryB, int32 w, float8 w_percentage, int32 lp, float8 *result) {
	float8 min;
	float8 **df = NULL;

	int32 rows, cols;
	int32 i, j;
	
	float8 *ret = NULL;

	if ((lp == 1 || lp == 2) && trajectoryA->NrPoints > 0 && trajectoryB->NrPoints > 0) {
		if (w < 0) //if window is not given, then ignore it.
			w = (int32) (Max(trajectoryA->NrPoints, trajectoryB->NrPoints) * w_percentage);

		rows = trajectoryA->NrPoints;
		cols = trajectoryB->NrPoints;

		df = (float8 **) palloc(rows * sizeof(*df));
		for (i = 0; i < rows; i++)
			df[i] = (float8 *) palloc0(cols * sizeof(**df));

		if (lp == 1)
			df[0][0] = distanceManhattanPointSP_PointSP(&trajectoryA->points[0].sp, &trajectoryB->points[0].sp);
		else if (lp == 2)
			df[0][0] = sqDistancePointSP_PointSP(&trajectoryA->points[0].sp, &trajectoryB->points[0].sp);

		for (i = 1; i < rows; i++) {
			if (lp == 1)
				df[i][0] = df[i - 1][0] + distanceManhattanPointSP_PointSP(&trajectoryA->points[i].sp, &trajectoryB->points[0].sp);
			else if (lp == 2)
				df[i][0] = df[i - 1][0] + sqDistancePointSP_PointSP(&trajectoryA->points[i].sp, &trajectoryB->points[0].sp);
		}

		for (i = 1; i < cols; i++) {
			if (lp == 1)
				df[0][i] = df[0][i - 1] + distanceManhattanPointSP_PointSP(&trajectoryA->points[0].sp, &trajectoryB->points[i].sp);
			else if (lp == 2)
				df[0][i] = df[0][i - 1] + sqDistancePointSP_PointSP(&trajectoryA->points[0].sp, &trajectoryB->points[i].sp);
		}

		for (i = 1; i < rows; i++) {
			for (j = 1; j < cols; j++) {
				if (abs(i - j) <= w) {
					min = Min(df[i - 1][j], df[i][j - 1]);
					min = Min(df[i - 1][j - 1], min);

					if (lp == 1)
						df[i][j] = min + distanceManhattanPointSP_PointSP(&trajectoryA->points[i].sp, &trajectoryB->points[j].sp);
					else if (lp == 2)
						df[i][j] = min + sqDistancePointSP_PointSP(&trajectoryA->points[i].sp, &trajectoryB->points[j].sp);
				} else
					df[i][j] = DBL_MAX;
			}
		}

		ret = result ? result : (float8 *) palloc(sizeof(*ret));
		if (lp == 1)
			*ret = df[rows - 1][cols - 1];
		else if (lp == 2)
			*ret = sqrt(df[rows - 1][cols - 1]);

		for (i = 0; i < rows; i++)
			pfree(df[i]);
		pfree(df);
	}

	return ret;
}

PGDLLEXPORT Datum DTWV1(PG_FUNCTION_ARGS) {
	float8 ret;

	if (DTW((Trajectory *) PG_GETARG_VARLENA_P(0), (Trajectory *) PG_GETARG_VARLENA_P(1), PG_GETARG_INT32(2), PG_GETARG_FLOAT8(3), PG_GETARG_INT32(4), &ret) == NULL)
		PG_RETURN_NULL();
	
	PG_RETURN_FLOAT8(ret);
}
PG_FUNCTION_INFO_V1(DTWV1);
