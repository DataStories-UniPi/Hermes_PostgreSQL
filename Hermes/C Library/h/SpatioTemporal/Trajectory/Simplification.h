/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_TRAJECTORY_SIMPLIFICATION_H_
#define	HE_TRAJECTORY_SIMPLIFICATION_H_

#include "Definition.h"

Trajectory *simplifyTrajectory(const Trajectory *trajectory, float8 samplingPeriod, bool strict, int32 precision);
Trajectory *simplifyEpochTrajectory(const Trajectory *trajectory, float8 samplingPeriod, bool strict, int32 precision);

PGDLLEXPORT Datum simplifyTrajectoryV1(PG_FUNCTION_ARGS);

Trajectory *simplifyDPTrajectory(const Trajectory *trajectory, float8 eps, bool plus);
PGDLLEXPORT Datum simplifyDPTrajectoryV1(PG_FUNCTION_ARGS);

#endif
