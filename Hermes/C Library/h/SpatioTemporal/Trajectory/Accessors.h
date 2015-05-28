/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_TRAJECTORY_ACCESSORS_H_
#define	HE_TRAJECTORY_ACCESSORS_H_

#include "Definition.h"
#include "../SegmentST/Definition.h"

PGDLLEXPORT Datum sizeTrajectory(PG_FUNCTION_ARGS);

int32 NrPointsTrajectory(Datum trajectory);
PGDLLEXPORT Datum NrPointsTrajectoryV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum NrSegmentsTrajectory(PG_FUNCTION_ARGS);

float8 samplingPeriodTrajectory(Datum trajectory);
PGDLLEXPORT Datum samplingPeriodTrajectoryV1(PG_FUNCTION_ARGS);

PointST *pointAtTrajectory(Datum trajectory, int32 i, PointST *result);
SegmentST *segmentAtTrajectory(Datum trajectory, int32 i, SegmentST *result);
PGDLLEXPORT Datum pointAtTrajectoryV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum segmentAtTrajectoryV1(PG_FUNCTION_ARGS);

PointST *firstPointTrajectory(Datum trajectory, PointST *result);
PointST *lastPointTrajectory(Datum trajectory, PointST *result);
SegmentST *firstSegmentTrajectory(Datum trajectory, SegmentST *result);
SegmentST *lastSegmentTrajectory(Datum trajectory, SegmentST *result);
PGDLLEXPORT Datum firstPointTrajectoryV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum lastPointTrajectoryV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum firstSegmentTrajectoryV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum lastSegmentTrajectoryV1(PG_FUNCTION_ARGS);

Trajectory *subTrajectory(Datum trajectory, int32 s, int32 e);
PGDLLEXPORT Datum subTrajectoryV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum pointsTrajectory(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum segmentsTrajectory(PG_FUNCTION_ARGS);

#endif
