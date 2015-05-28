/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_TRAJECTORY_H_
#define	HE_TRAJECTORY_H_

#include "../PointST/Definition.h"

/*
 ""
 "'yyyy-MM-dd hh:mm:ss.zzzzzz' x y"
 "'yyyy-MM-dd hh:mm:ss.zzzzzz' x y,'yyyy-MM-dd hh:mm:ss.zzzzzz' x y"
 "'yyyy-MM-dd hh:mm:ss.zzzzzz' x y,'yyyy-MM-dd hh:mm:ss.zzzzzz' x y,'yyyy-MM-dd hh:mm:ss.zzzzzz' x y"
 ...
 */

//#pragma pack(push)
//#pragma pack(1)

typedef struct {
	char vl_len_[4];

	int32 NrPoints;

	float8 samplingPeriod;

	PointST points[0];
} Trajectory;

//#pragma pack(pop)

Trajectory *allocateTrajectory(int32 NrPoints);
Trajectory *reallocateTrajectory(Trajectory *trajectory, int32 NrPoints);
Trajectory *updateMetadataTrajectory(Trajectory *trajectory);

Trajectory *cloneTrajectory(const Trajectory *trajectory);
PGDLLEXPORT Datum cloneTrajectoryV1(PG_FUNCTION_ARGS);

Trajectory *inTrajectory(const char *str, char **error);
PGDLLEXPORT Datum inTrajectoryV1(PG_FUNCTION_ARGS);

char *outTrajectory(const Trajectory *trajectory);
PGDLLEXPORT Datum outTrajectoryV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum recvTrajectory(PG_FUNCTION_ARGS);

bytea *Trajectory2Bytea(const Trajectory *trajectory);
PGDLLEXPORT Datum sendTrajectory(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum hashTrajectory(PG_FUNCTION_ARGS);

bool equalsTrajectory(const Trajectory *trajectoryA, const Trajectory *trajectoryB);
PGDLLEXPORT Datum equalsTrajectoryV1(PG_FUNCTION_ARGS);

#define NrPoints2NrSegments(NrPoints) ((NrPoints) == 0 ? 0 : (NrPoints) - 1)

#endif
