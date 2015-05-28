/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_TRAJECTORY_INTERPOLATION_H_
#define	HE_TRAJECTORY_INTERPOLATION_H_

#include "Definition.h"
#include "../BoxST/Definition.h"

float8 *speedAtTrajectory(Datum trajectory, int32 i, const float8 *s0, int32 precision, float8 *result);
float8 *accelerationAtTrajectory(Datum trajectory, int32 i, const float8 *s0, int32 precision, float8 *result);
PGDLLEXPORT Datum speedAtTrajectoryV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum speedAtTrajectoryV1_2(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum accelerationAtTrajectoryV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum accelerationAtTrajectoryV1_2(PG_FUNCTION_ARGS);

float8 *speedAtTTrajectory(Datum trajectory, Timestamp ts, int32 precision, float8 *result);
float8 *accelerationAtTTrajectory(Datum trajectory, Timestamp ts, int32 precision, float8 *result);
PGDLLEXPORT Datum speedAtTTrajectoryV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum accelerationAtTTrajectoryV1(PG_FUNCTION_ARGS);

PointST *atInstantTrajectory(Datum trajectory, Timestamp ts, int32 precision, PointST *result);
PGDLLEXPORT Datum atInstantTrajectoryV1(PG_FUNCTION_ARGS);

Trajectory *atPeriodTrajectory(Datum trajectory, const Period *period, int32 precision);
PGDLLEXPORT Datum atPeriodTrajectoryV1(PG_FUNCTION_ARGS);

PointST *atDistanceTrajectory(const Trajectory *trajectory, float8 distance, int32 precision, PointST *result);
PGDLLEXPORT Datum atDistanceTrajectoryV1(PG_FUNCTION_ARGS);

int32 atPointTrajectory(const Trajectory *trajectory, const PointSP *point, int32 precision, PointST ***result);
PGDLLEXPORT Datum atPointTrajectoryV1(PG_FUNCTION_ARGS);

int32 atBoxTrajectory(const Trajectory *trajectory, const BoxSP *box, int32 precision, Trajectory ***result);
PGDLLEXPORT Datum atBoxTrajectoryV1(PG_FUNCTION_ARGS);

int32 atBoxSTTrajectory(Datum trajectory, const BoxST *box, int32 precision, Trajectory ***result);
PGDLLEXPORT Datum atBoxSTTrajectoryV1(PG_FUNCTION_ARGS);

#endif
