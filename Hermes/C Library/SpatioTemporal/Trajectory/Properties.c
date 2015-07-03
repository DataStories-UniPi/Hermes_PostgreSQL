/**
 *	@file
 *	@author Marios Vodas (mvodas@gmail.com).
 *	@brief The implementation of Trajectory
 *
 *	@details These functions are documented in the @ref Trajectory/Properties.h
 *
 */

#include "../../h/SpatioTemporal/Trajectory/Properties.h"
#include "../../h/SpatioTemporal/Trajectory/Accessors.h"
#include "../../h/SpatioTemporal/SegmentST/Properties.h"
#include "../../h/Spatial/PointSP/Distance.h"
#include "../../h/Spatial/SegmentSP/Casts.h"
#include "../../h/Spatial/LineSP/Properties.h"
#include "../../h/Utilities/Utilities.h"
#include "../../h/Utilities/MSVCWorkarounds.h"

Interval *durationInIntervalTrajectory(Datum trajectory, Interval *result) {
	int32 NrPoints;
	PointST point;
	Timestamp i, e;

	NrPoints = NrPointsTrajectory(trajectory);
	if (NrPoints > 0) {
		pointAtTrajectory(trajectory, 1, &point);
		i = point.t;

		pointAtTrajectory(trajectory, NrPoints, &point);
		e = point.t;

		return differenceInInterval(e, i, result);
	}

	return NULL;
}

PGDLLEXPORT Datum durationInIntervalTrajectoryV1(PG_FUNCTION_ARGS) {
	Interval *ret = durationInIntervalTrajectory(PG_GETARG_DATUM(0), NULL);

	if (ret == NULL)
		PG_RETURN_NULL();

	PG_RETURN_INTERVAL_P(ret);
}
PG_FUNCTION_INFO_V1(durationInIntervalTrajectoryV1);

float8 *durationTrajectory(Datum trajectory, float8 *result) {
	Interval durationInInterval;
	float8 *ret = NULL;

	if (durationInIntervalTrajectory(trajectory, &durationInInterval)) {
		ret = result ? result : (float8 *) palloc(sizeof(*ret));
		*ret = interval2seconds(&durationInInterval);
	}

	return ret;
}

float8 *lengthTrajectory(const Trajectory *trajectory, float8 *result) {
	int32 i;
	float8 *ret = NULL;

	if (trajectory->NrPoints > 0) {
		ret = result ? result : (float8 *) palloc(sizeof(*ret));

		*ret = 0.0;

		for (i = 1; i < trajectory->NrPoints; i++)
			*ret += distancePointSP_PointSP(&trajectory->points[i].sp, &trajectory->points[i - 1].sp);
	}

	return ret;
}

PGDLLEXPORT Datum lengthTrajectoryV1(PG_FUNCTION_ARGS) {
	float8 ret;

	if (lengthTrajectory((Trajectory *) PG_GETARG_VARLENA_P(0), &ret) == NULL)
		PG_RETURN_NULL();

	PG_RETURN_FLOAT8(ret);
}
PG_FUNCTION_INFO_V1(lengthTrajectoryV1);

float8 *displacementTrajectory(Datum trajectory, float8 *result) {
	int32 NrPoints;
	PointST pointA, pointB;
	float8 *ret = NULL;

	NrPoints = NrPointsTrajectory(trajectory);
	if (NrPoints > 0) {
		ret = result ? result : (float8 *) palloc(sizeof(*ret));

		pointAtTrajectory(trajectory, 1, &pointA);
		pointAtTrajectory(trajectory, NrPoints, &pointB);

		*ret = distancePointSP_PointSP(&pointB.sp, &pointA.sp);
	}

	return ret;
}

PGDLLEXPORT Datum displacementTrajectoryV1(PG_FUNCTION_ARGS) {
	float8 ret;

	if (displacementTrajectory(PG_GETARG_DATUM(0), &ret) == NULL)
		PG_RETURN_NULL();

	PG_RETURN_FLOAT8(ret);
}
PG_FUNCTION_INFO_V1(displacementTrajectoryV1);

PointST *centroidTrajectory(const Trajectory *trajectory, PointST *result) {
	int32 i;

	float8 sum_t;
	int64 sum_x, sum_y;
	PointST *ret = NULL;

	if (trajectory->NrPoints > 0) {
		ret = result ? result : (PointST *) palloc(sizeof(*ret));

		sum_t = timestamp2seconds(trajectory->points[0].t);
		sum_x = trajectory->points[0].sp.x;
		sum_y = trajectory->points[0].sp.y;

		for (i = 1; i < trajectory->NrPoints; i++) {
			sum_t += timestamp2seconds(trajectory->points[i].t);
			sum_x += trajectory->points[i].sp.x;
			sum_y += trajectory->points[i].sp.y;
		}

		ret->t = seconds2timestamp(sum_t / trajectory->NrPoints);
		ret->sp.x = (int32) round((float8) sum_x / trajectory->NrPoints);
		ret->sp.y = (int32) round((float8) sum_y / trajectory->NrPoints);
	}

	return ret;
}

PGDLLEXPORT Datum centroidTrajectoryV1(PG_FUNCTION_ARGS) {
	PointST *ret = centroidTrajectory((Trajectory *) PG_GETARG_VARLENA_P(0), NULL);

	if (ret == NULL)
		PG_RETURN_NULL();

	PG_RETURN_POINTER(ret);
}
PG_FUNCTION_INFO_V1(centroidTrajectoryV1);

PointSP *massCenterTrajectory(const Trajectory *trajectory, PointSP *result) {
	int32 i;
	float8 x, y;
	PointSP *ret = NULL;

	if (trajectory->NrPoints > 0) {
		ret = result ? result : (PointSP *) palloc(sizeof(*ret));

		if (trajectory->NrPoints > 1) {
			x = (trajectory->points[1].sp.x + trajectory->points[0].sp.x) / 2.0;
			y = (trajectory->points[1].sp.y + trajectory->points[0].sp.y) / 2.0;

			for (i = 2; i < trajectory->NrPoints; i++) {
				x = (x + (trajectory->points[i].sp.x + trajectory->points[i - 1].sp.x) / 2.0) / 2.0;
				y = (y + (trajectory->points[i].sp.y + trajectory->points[i - 1].sp.y) / 2.0) / 2.0;
			}

			ret->x = (int32) round(x);
			ret->y = (int32) round(y);
		} else
			*ret = trajectory->points[0].sp;
	}

	return ret;
}

PGDLLEXPORT Datum massCenterTrajectoryV1(PG_FUNCTION_ARGS) {
	PointSP *ret = massCenterTrajectory((Trajectory *) PG_GETARG_VARLENA_P(0), NULL);

	if (ret == NULL)
		PG_RETURN_NULL();

	PG_RETURN_POINTER(ret);
}
PG_FUNCTION_INFO_V1(massCenterTrajectoryV1);

float8 *gyradiusTrajectory(const Trajectory *trajectory, float8 *result) {
	int32 i;
	PointST c;
	float8 *ret = NULL;

	if (trajectory->NrPoints > 0) {
		ret = result ? result : (float8 *) palloc(sizeof(*ret));

		centroidTrajectory(trajectory, &c);

		*ret = pow(distancePointSP_PointSP(&trajectory->points[0].sp, &c.sp), 2.0);

		for (i = 1; i < trajectory->NrPoints; i++)
			*ret += pow(distancePointSP_PointSP(&trajectory->points[i].sp, &c.sp), 2.0);

		*ret = sqrt(*ret / trajectory->NrPoints);
	}

	return ret;
}

PGDLLEXPORT Datum gyradiusTrajectoryV1(PG_FUNCTION_ARGS) {
	float8 ret;

	if (gyradiusTrajectory((Trajectory *) PG_GETARG_VARLENA_P(0), &ret) == NULL)
		PG_RETURN_NULL();

	PG_RETURN_FLOAT8(ret);
}
PG_FUNCTION_INFO_V1(gyradiusTrajectoryV1);

float8 *angleXXAvgTrajectory(const Trajectory *trajectory, float8 *result) {
	int32 i;
	SegmentSP segment;
	LineSP line;
	float8 *ret = NULL;

	if (trajectory->NrPoints > 1) {
		ret = result ? result : (float8 *) palloc(sizeof(*ret));

		segment.i = trajectory->points[0].sp;
		segment.e = trajectory->points[1].sp;

		*ret = angleXXLineSP(SegmentSP2LineSP(&segment, &line));

		for (i = 2; i < trajectory->NrPoints; i++) {
			segment.i = trajectory->points[i - 1].sp;
			segment.e = trajectory->points[i].sp;

			*ret += angleXXLineSP(SegmentSP2LineSP(&segment, &line));
		}

		*ret /= trajectory->NrPoints - 1;
	}

	return ret;
}

PGDLLEXPORT Datum angleXXAvgTrajectoryV1(PG_FUNCTION_ARGS) {
	float8 ret;

	if (angleXXAvgTrajectory((Trajectory *) PG_GETARG_VARLENA_P(0), &ret) == NULL)
		PG_RETURN_NULL();

	PG_RETURN_FLOAT8(ret);
}
PG_FUNCTION_INFO_V1(angleXXAvgTrajectoryV1);

float8 *angleXXTrajectory(Datum trajectory, float8 *result) {
	int32 NrPoints;
	PointST point;
	SegmentSP segment;
	LineSP line;
	float8 *ret = NULL;

	NrPoints = NrPointsTrajectory(trajectory);
	if (NrPoints > 1) {
		ret = result ? result : (float8 *) palloc(sizeof(*ret));

		pointAtTrajectory(trajectory, 1, &point);
		segment.i = point.sp;

		pointAtTrajectory(trajectory, NrPoints, &point);
		segment.e = point.sp;

		*ret = angleXXLineSP(SegmentSP2LineSP(&segment, &line));
	}

	return ret;
}

PGDLLEXPORT Datum angleXXTrajectoryV1(PG_FUNCTION_ARGS) {
	float8 ret;

	if (angleXXTrajectory(PG_GETARG_DATUM(0), &ret) == NULL)
		PG_RETURN_NULL();

	PG_RETURN_FLOAT8(ret);
}
PG_FUNCTION_INFO_V1(angleXXTrajectoryV1);

float8 *averageSpeedTrajectory(Datum trajectory, float8 *result) {
	int32 NrPoints;
	PointST point;
	SegmentST segment;
	float8 *ret = NULL;

	NrPoints = NrPointsTrajectory(trajectory);
	if (NrPoints > 1) {
		ret = result ? result : (float8 *) palloc(sizeof(*ret));

		pointAtTrajectory(trajectory, 1, &point);
		segment.t.i = point.t;
		segment.sp.i = point.sp;

		pointAtTrajectory(trajectory, NrPoints, &point);
		segment.t.e = point.t;
		segment.sp.e = point.sp;

		*ret = averageSpeedSegmentST(&segment);
	}

	return ret;
}

PGDLLEXPORT Datum averageSpeedTrajectoryV1(PG_FUNCTION_ARGS) {
	float8 ret;

	if (averageSpeedTrajectory(PG_GETARG_DATUM(0), &ret) == NULL)
		PG_RETURN_NULL();

	PG_RETURN_FLOAT8(ret);
}
PG_FUNCTION_INFO_V1(averageSpeedTrajectoryV1);

float8 *normalizedSamplingRateTrajectory(Datum trajectory, float8 *result) {
	int32 NrPoints;
	PointST point;
	Timestamp i, e;
	float8 *ret = NULL;

	NrPoints = NrPointsTrajectory(trajectory);
	if (NrPoints > 1) {
		ret = result ? result : (float8 *) palloc(sizeof(*ret));

		pointAtTrajectory(trajectory, 1, &point);
		i = point.t;

		pointAtTrajectory(trajectory, NrPoints, &point);
		e = point.t;

		*ret = NrPoints / differenceInSeconds(e, i);
	}

	return ret;
}

PGDLLEXPORT Datum normalizedSamplingRateTrajectoryV1(PG_FUNCTION_ARGS) {
	float8 ret;

	if (normalizedSamplingRateTrajectory(PG_GETARG_DATUM(0), &ret) == NULL)
		PG_RETURN_NULL();

	PG_RETURN_FLOAT8(ret);
}
PG_FUNCTION_INFO_V1(normalizedSamplingRateTrajectoryV1);
