/**
 *	@file
 *	@author Marios Vodas (mvodas@gmail.com).
 *	@brief The implementation of Trajectory
 *
 *	@details These functions are documented in the @ref Trajectory/Accessors.h
 *
 */

#include "../../h/SpatioTemporal/Trajectory/Accessors.h"
#include "../../h/Utilities/Utilities.h"
#include <funcapi.h>
#include <access/tuptoaster.h>

PGDLLEXPORT Datum sizeTrajectory(PG_FUNCTION_ARGS) {
	PG_RETURN_INT64((int64) toast_raw_datum_size(PG_GETARG_DATUM(0)));
}
PG_FUNCTION_INFO_V1(sizeTrajectory);

int32 NrPointsTrajectory(Datum trajectory) {
	return (int32) (toast_raw_datum_size(trajectory) - offsetof(Trajectory, points)) / sizeof(PointST);
}

PGDLLEXPORT Datum NrPointsTrajectoryV1(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(NrPointsTrajectory(PG_GETARG_DATUM(0)));
}
PG_FUNCTION_INFO_V1(NrPointsTrajectoryV1);

PGDLLEXPORT Datum NrSegmentsTrajectory(PG_FUNCTION_ARGS) {
	int32 NrPoints = NrPointsTrajectory(PG_GETARG_DATUM(0));
	
	PG_RETURN_INT32(NrPoints2NrSegments(NrPoints));
}
PG_FUNCTION_INFO_V1(NrSegmentsTrajectory);

float8 samplingPeriodTrajectory(Datum trajectory) {
	struct varlena *slice = NULL;

	float8 ret;

	slice = PG_DETOAST_DATUM_SLICE(trajectory, offsetof(Trajectory, samplingPeriod) - VARHDRSZ, sizeof(ret));

	memcpy(&ret, VARDATA(slice), sizeof(ret));

	pfree(slice);

	return ret;
}

PGDLLEXPORT Datum samplingPeriodTrajectoryV1(PG_FUNCTION_ARGS) {
	PG_RETURN_INTERVAL_P(seconds2interval(samplingPeriodTrajectory(PG_GETARG_DATUM(0)), NULL));
}
PG_FUNCTION_INFO_V1(samplingPeriodTrajectoryV1);

PointST *pointAtTrajectory(Datum trajectory, int32 i, PointST *result) {
	int32 NrPoints;
	struct varlena *slice = NULL;

	PointST *ret = NULL;

	NrPoints = NrPointsTrajectory(trajectory);
	if (0 < i && i <= NrPoints) {
		slice = PG_DETOAST_DATUM_SLICE(trajectory, offsetof(Trajectory, points) - VARHDRSZ + (i - 1) * sizeof(PointST), sizeof(PointST));

		ret = result ? result : (PointST *) palloc(sizeof(*ret));
		memcpy(ret, VARDATA(slice), sizeof(*ret));

		pfree(slice);
	}

	return ret;
}

SegmentST *segmentAtTrajectory(Datum trajectory, int32 i, SegmentST *result) {
	int32 NrPoints;
	struct varlena *slice = NULL;
	PointST *point = NULL;

	SegmentST *ret = NULL;

	NrPoints = NrPointsTrajectory(trajectory);
	if (0 < i && i < NrPoints) {
		slice = PG_DETOAST_DATUM_SLICE(trajectory, offsetof(Trajectory, points) - VARHDRSZ + (i - 1) * sizeof(PointST), 2 * sizeof(PointST));

		point = (PointST *) VARDATA(slice);

		ret = result ? result : (SegmentST *) palloc(sizeof(*ret));
		ret->t.i = point[0].t;
		ret->sp.i = point[0].sp;
		ret->t.e = point[1].t;
		ret->sp.e = point[1].sp;

		pfree(slice);
	}

	return ret;
}

PGDLLEXPORT Datum pointAtTrajectoryV1(PG_FUNCTION_ARGS) {
	PointST *ret = pointAtTrajectory(PG_GETARG_DATUM(0), PG_GETARG_INT32(1), NULL);

	if (ret == NULL)
		PG_RETURN_NULL();

	PG_RETURN_POINTER(ret);
}
PG_FUNCTION_INFO_V1(pointAtTrajectoryV1);

PGDLLEXPORT Datum segmentAtTrajectoryV1(PG_FUNCTION_ARGS) {
	SegmentST *ret = segmentAtTrajectory(PG_GETARG_DATUM(0), PG_GETARG_INT32(1), NULL);

	if (ret == NULL)
		PG_RETURN_NULL();

	PG_RETURN_POINTER(ret);
}
PG_FUNCTION_INFO_V1(segmentAtTrajectoryV1);

PointST *firstPointTrajectory(Datum trajectory, PointST *result) {
	return pointAtTrajectory(trajectory, 1, result);
}

PointST *lastPointTrajectory(Datum trajectory, PointST *result) {
	return pointAtTrajectory(trajectory, NrPointsTrajectory(trajectory), result);
}

SegmentST *firstSegmentTrajectory(Datum trajectory, SegmentST *result) {
	return segmentAtTrajectory(trajectory, 1, result);
}

SegmentST *lastSegmentTrajectory(Datum trajectory, SegmentST *result) {
	return segmentAtTrajectory(trajectory, NrPointsTrajectory(trajectory) - 1, result);
}

PGDLLEXPORT Datum firstPointTrajectoryV1(PG_FUNCTION_ARGS) {
	PointST *ret = firstPointTrajectory(PG_GETARG_DATUM(0), NULL);

	if (ret == NULL)
		PG_RETURN_NULL();

	PG_RETURN_POINTER(ret);
}
PG_FUNCTION_INFO_V1(firstPointTrajectoryV1);

PGDLLEXPORT Datum lastPointTrajectoryV1(PG_FUNCTION_ARGS) {
	PointST *ret = lastPointTrajectory(PG_GETARG_DATUM(0), NULL);

	if (ret == NULL)
		PG_RETURN_NULL();

	PG_RETURN_POINTER(ret);
}
PG_FUNCTION_INFO_V1(lastPointTrajectoryV1);

PGDLLEXPORT Datum firstSegmentTrajectoryV1(PG_FUNCTION_ARGS) {
	SegmentST *ret = firstSegmentTrajectory(PG_GETARG_DATUM(0), NULL);

	if (ret == NULL)
		PG_RETURN_NULL();

	PG_RETURN_POINTER(ret);
}
PG_FUNCTION_INFO_V1(firstSegmentTrajectoryV1);

PGDLLEXPORT Datum lastSegmentTrajectoryV1(PG_FUNCTION_ARGS) {
	SegmentST *ret = lastSegmentTrajectory(PG_GETARG_DATUM(0), NULL);

	if (ret == NULL)
		PG_RETURN_NULL();

	PG_RETURN_POINTER(ret);
}
PG_FUNCTION_INFO_V1(lastSegmentTrajectoryV1);

Trajectory *subTrajectory(Datum trajectory, int32 s, int32 e) {
	int32 NrPoints;
	struct varlena *slice = NULL;

	Trajectory *ret = NULL;

	NrPoints = NrPointsTrajectory(trajectory);

	s = Max(s, 1);
	e = Min(e, NrPoints);

	if (0 < s && s <= e && e <= NrPoints) {
		NrPoints = e - s + 1;
		
		slice = PG_DETOAST_DATUM_SLICE(trajectory, offsetof(Trajectory, points) - VARHDRSZ + (s - 1) * sizeof(PointST), NrPoints * sizeof(PointST));

		ret = allocateTrajectory(NrPoints);
		memcpy(ret->points, VARDATA(slice), VARSIZE(slice) - VARHDRSZ);

		pfree(slice);
	}

	return ret;
}

PGDLLEXPORT Datum subTrajectoryV1(PG_FUNCTION_ARGS) {
	Trajectory *ret = subTrajectory(PG_GETARG_DATUM(0), PG_GETARG_INT32(1), PG_GETARG_INT32(2));

	if (ret == NULL)
		PG_RETURN_NULL();

	PG_RETURN_POINTER(updateMetadataTrajectory(ret));
}
PG_FUNCTION_INFO_V1(subTrajectoryV1);

PGDLLEXPORT Datum pointsTrajectory(PG_FUNCTION_ARGS) {
	FuncCallContext *funcctx;
	Trajectory *trajectory;

	if (SRF_IS_FIRSTCALL()) {
		MemoryContext oldcontext;

		funcctx = SRF_FIRSTCALL_INIT();
		oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

		trajectory = (Trajectory *) PG_GETARG_VARLENA_P(0);

		funcctx->user_fctx = trajectory;
		funcctx->max_calls = trajectory->NrPoints;

		MemoryContextSwitchTo(oldcontext);
	}

	funcctx = SRF_PERCALL_SETUP();

	if (funcctx->call_cntr < funcctx->max_calls) {
		uint32 i = funcctx->call_cntr;
		trajectory = (Trajectory *) funcctx->user_fctx;
		
		SRF_RETURN_NEXT(funcctx, PointerGetDatum(&trajectory->points[i]));
	} else
		SRF_RETURN_DONE(funcctx);
}
PG_FUNCTION_INFO_V1(pointsTrajectory);

PGDLLEXPORT Datum segmentsTrajectory(PG_FUNCTION_ARGS) {
	FuncCallContext *funcctx;
	Trajectory *trajectory;
	SegmentST *segment;

	if (SRF_IS_FIRSTCALL()) {
		MemoryContext oldcontext;

		funcctx = SRF_FIRSTCALL_INIT();
		oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

		trajectory = (Trajectory *) PG_GETARG_VARLENA_P(0);

		funcctx->user_fctx = trajectory;
		funcctx->max_calls = trajectory->NrPoints - 1;

		MemoryContextSwitchTo(oldcontext);
	}

	funcctx = SRF_PERCALL_SETUP();

	if (funcctx->call_cntr < funcctx->max_calls) {
		trajectory = (Trajectory *) funcctx->user_fctx;

		segment = (SegmentST *) palloc(sizeof(*segment));
		segment->t.i = trajectory->points[funcctx->call_cntr].t;
		segment->sp.i = trajectory->points[funcctx->call_cntr].sp;
		segment->t.e = trajectory->points[funcctx->call_cntr + 1].t;
		segment->sp.e = trajectory->points[funcctx->call_cntr + 1].sp;

		SRF_RETURN_NEXT(funcctx, PointerGetDatum(segment));
	} else
		SRF_RETURN_DONE(funcctx);
}
PG_FUNCTION_INFO_V1(segmentsTrajectory);
