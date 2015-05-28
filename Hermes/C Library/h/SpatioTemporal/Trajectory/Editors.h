/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_TRAJECTORY_EDITORS_H_
#define	HE_TRAJECTORY_EDITORS_H_

#include "Definition.h"
#include "../SegmentST/Definition.h"

Trajectory *expandOnPointTrajectory(Trajectory *trajectory, const PointST *point);
PGDLLEXPORT Datum expandOnPointTrajectoryV1(PG_FUNCTION_ARGS);

Trajectory *expandOnSegmentTrajectory(Trajectory *trajectory, const SegmentST *segment);
PGDLLEXPORT Datum expandOnSegmentTrajectoryV1(PG_FUNCTION_ARGS);

Trajectory *expandOnTrajectoryTrajectory(Trajectory *trajectoryA, const Trajectory *trajectoryB, char **error);
PGDLLEXPORT Datum expandOnTrajectoryTrajectoryV1(PG_FUNCTION_ARGS);

#endif
