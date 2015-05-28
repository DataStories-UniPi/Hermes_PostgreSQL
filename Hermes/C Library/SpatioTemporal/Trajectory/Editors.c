/*
* Authors: Marios Vodas (mvodas@gmail.com).
*/

#include "../../h/SpatioTemporal/Trajectory/Editors.h"

Trajectory *expandOnPointTrajectory(Trajectory *trajectory, const PointST *newPoint) {
	if (trajectory->NrPoints > 0 && newPoint->t <= trajectory->points[trajectory->NrPoints - 1].t)
		return NULL;

	trajectory = reallocateTrajectory(trajectory, trajectory->NrPoints + 1);
	trajectory->points[trajectory->NrPoints - 1] = *newPoint;

	return trajectory;
}

PGDLLEXPORT Datum expandOnPointTrajectoryV1(PG_FUNCTION_ARGS) {
	Trajectory *ret = NULL, *copy = (Trajectory *) PG_GETARG_VARLENA_P(0);

	if ((Pointer) copy == PG_GETARG_POINTER(0))
		copy = (Trajectory *) PG_DETOAST_DATUM_COPY(PG_GETARG_DATUM(0));

	ret = expandOnPointTrajectory(copy, (PointST *) PG_GETARG_POINTER(1));

	if (ret == NULL) {
		pfree(copy);

		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("invalid time sequence for Trajectory")));
	}

	PG_RETURN_POINTER(updateMetadataTrajectory(ret));
}
PG_FUNCTION_INFO_V1(expandOnPointTrajectoryV1);

Trajectory *expandOnSegmentTrajectory(Trajectory *trajectory, const SegmentST *segment) {
	if (trajectory->NrPoints > 0) {
		if (segment->t.i != trajectory->points[trajectory->NrPoints - 1].t || segment->sp.i.x != trajectory->points[trajectory->NrPoints - 1].sp.x || segment->sp.i.y != trajectory->points[trajectory->NrPoints - 1].sp.y)
			return NULL;

		trajectory = reallocateTrajectory(trajectory, trajectory->NrPoints + 1);
	} else {
		pfree(trajectory);
		trajectory = allocateTrajectory(2);

		trajectory->points[0].t = segment->t.i;
		trajectory->points[0].sp = segment->sp.i;
	}

	trajectory->points[trajectory->NrPoints - 1].t = segment->t.e;
	trajectory->points[trajectory->NrPoints - 1].sp = segment->sp.e;

	return trajectory;
}

PGDLLEXPORT Datum expandOnSegmentTrajectoryV1(PG_FUNCTION_ARGS) {
	Trajectory *ret = NULL, *copy = (Trajectory *) PG_GETARG_VARLENA_P(0);

	if ((Pointer) copy == PG_GETARG_POINTER(0))
		copy = (Trajectory *) PG_DETOAST_DATUM_COPY(PG_GETARG_DATUM(0));

	ret = expandOnSegmentTrajectory(copy, (SegmentST *) PG_GETARG_POINTER(1));

	if (ret == NULL) {
		pfree(copy);

		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("invalid time sequence for Trajectory")));
	}

	PG_RETURN_POINTER(updateMetadataTrajectory(ret));
}
PG_FUNCTION_INFO_V1(expandOnSegmentTrajectoryV1);

Trajectory *expandOnTrajectoryTrajectory(Trajectory *trajectoryA, const Trajectory *trajectoryB, char **error) {
	if (error)
		*error = NULL;

	if (trajectoryA->NrPoints > 0) {
		if (trajectoryB->NrPoints > 0) {
			if (trajectoryB->points[0].t != trajectoryA->points[trajectoryA->NrPoints - 1].t || trajectoryB->points[0].sp.x != trajectoryA->points[trajectoryA->NrPoints - 1].sp.x || trajectoryB->points[0].sp.y != trajectoryA->points[trajectoryA->NrPoints - 1].sp.y) {
				if (error)
					*error = "invalid time sequence for Trajectory";
				else
					pfree(trajectoryA);

				return NULL;
			}

			trajectoryA = reallocateTrajectory(trajectoryA, trajectoryA->NrPoints + trajectoryB->NrPoints - 1);

			memcpy(&trajectoryA->points[trajectoryA->NrPoints - trajectoryB->NrPoints + 1], &trajectoryB->points[1], (trajectoryB->NrPoints - 1) * sizeof(PointST));
		}
	} else {
		pfree(trajectoryA);

		if (trajectoryB->NrPoints > 0)
			trajectoryA = cloneTrajectory(trajectoryB);
		else
			trajectoryA = NULL;
	}

	return trajectoryA;
}

PGDLLEXPORT Datum expandOnTrajectoryTrajectoryV1(PG_FUNCTION_ARGS) {
	char *error;
	Trajectory *ret = NULL, *copy = (Trajectory *) PG_GETARG_VARLENA_P(0);

	if ((Pointer) copy == PG_GETARG_POINTER(0))
		copy = (Trajectory *) PG_DETOAST_DATUM_COPY(PG_GETARG_DATUM(0));

	ret = expandOnTrajectoryTrajectory(copy, (Trajectory *) PG_GETARG_VARLENA_P(1), &error);

	if (error)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("%s", error)));

	if (ret == NULL)
		PG_RETURN_NULL();

	PG_RETURN_POINTER(updateMetadataTrajectory(ret));
}
PG_FUNCTION_INFO_V1(expandOnTrajectoryTrajectoryV1);
