/*
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the Trajectory data type.
 *
 * @see @ref Trajectory
 * @see @ref data_type_trajectory
 */

#ifndef HE_TRAJECTORY_CONSTRUCTORS_H_
#define	HE_TRAJECTORY_CONSTRUCTORS_H_

#include "Definition.h"

PGDLLEXPORT Datum constructorEmptyTrajectory(PG_FUNCTION_ARGS);

Trajectory *constructorPointTrajectory(const PointST *point);
PGDLLEXPORT Datum constructorPointTrajectoryV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum constructorPointArrayTrajectory(PG_FUNCTION_ARGS);

extern MemoryContext TrajCache_MemoryContext;

extern int32 TrajCache_NrCaches;

extern PointST **TrajCache;
extern int32 *TrajCache_Length;
extern int32 *TrajCache_Index;

PGDLLEXPORT Datum TrajCache_Allocate(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum TrajCache_Free(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum TrajCache_Clean(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum TrajCache_ResetIndex(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum TrajCache_Rewind(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum TrajCache_Append(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum TrajCache_getIndex(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum TrajCache_Point(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum TrajCache2Trajectory(PG_FUNCTION_ARGS);

#endif
