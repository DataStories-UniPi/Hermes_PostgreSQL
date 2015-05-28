/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_TRAJECTORY_PROPERTIES_H_
#define	HE_TRAJECTORY_PROPERTIES_H_

#include "Definition.h"

Interval *durationInIntervalTrajectory(Datum trajectory, Interval *result);
PGDLLEXPORT Datum durationInIntervalTrajectoryV1(PG_FUNCTION_ARGS);

float8 *durationTrajectory(Datum trajectory, float8 *result);

float8 *lengthTrajectory(const Trajectory *trajectory, float8 *result);
PGDLLEXPORT Datum lengthTrajectoryV1(PG_FUNCTION_ARGS);

float8 *displacementTrajectory(Datum trajectory, float8 *result);
PGDLLEXPORT Datum displacementTrajectoryV1(PG_FUNCTION_ARGS);

PointST *centroidTrajectory(const Trajectory *trajectory, PointST *result);
PGDLLEXPORT Datum centroidTrajectoryV1(PG_FUNCTION_ARGS);

PointSP *massCenterTrajectory(const Trajectory *trajectory, PointSP *result);
PGDLLEXPORT Datum massCenterTrajectoryV1(PG_FUNCTION_ARGS);

float8 *gyradiusTrajectory(const Trajectory *trajectory, float8 *result);
PGDLLEXPORT Datum gyradiusTrajectoryV1(PG_FUNCTION_ARGS);

float8 *angleXXAvgTrajectory(const Trajectory *trajectory, float8 *result);
PGDLLEXPORT Datum angleXXAvgTrajectoryV1(PG_FUNCTION_ARGS);

float8 *angleXXTrajectory(Datum trajectory, float8 *result);
PGDLLEXPORT Datum angleXXTrajectoryV1(PG_FUNCTION_ARGS);

float8 *averageSpeedTrajectory(Datum trajectory, float8 *result);
PGDLLEXPORT Datum averageSpeedTrajectoryV1(PG_FUNCTION_ARGS);

float8 *normalizedSamplingRateTrajectory(Datum trajectory, float8 *result);
PGDLLEXPORT Datum normalizedSamplingRateTrajectoryV1(PG_FUNCTION_ARGS);

#endif
