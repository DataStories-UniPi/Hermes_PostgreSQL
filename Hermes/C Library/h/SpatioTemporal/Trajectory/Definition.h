/*
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the Trajectory data type.
 *
 * @see @ref Trajectory
 * @see @ref data_type_trajectory
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

/**
 * Trajectory is a spatio-temporal data type and is
 * compromised of a sequence of PointST objects
 * ordered by time
 *
 * @see @ref Trajectory/Accessors.sql
 * @see @ref Trajectory/Casts.sql
 * @see @ref Trajectory/Constructors.sql
 * @see @ref Trajectory/Definition.sql
 * @see @ref Trajectory/Editors.sql
 * @see @ref Trajectory/Interpolation.sql
 * @see @ref Trajectory/Properties.sql
 * @see @ref Trajectory/RelationalOperators.sql
 * @see @ref Trajectory/SessionParameters.sql
 * @see @ref Trajectory/Similarity.sql
 * @see @ref Trajectory/Simplification.sql
 *
 */
typedef struct {
	char vl_len_[4];        /**< i have no idea */
	int32 NrPoints;         /**< the number of PointST */
	float8 samplingPeriod;  /**< the time interval between sampling */
	PointST points[0];		/**< the actual points */
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
