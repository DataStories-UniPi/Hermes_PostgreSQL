/**
 *	@file
 *	@author Marios Vodas (mvodas@gmail.com).
 *	@brief The implementation of Trajectory
 *
 *	@details These functions are documented in the @ref Trajectory/Definition.h
 *
 */
#include "../../h/SpatioTemporal/Trajectory/Definition.h"
#include "../../h/Utilities/Utilities.h"
#include <libpq/pqformat.h>
#include <access/hash.h>

Trajectory *allocateTrajectory(int32 NrPoints) {
	size_t size;
	Trajectory *result = NULL;

	size = offsetof(Trajectory, points) + NrPoints * sizeof(PointST);
	result = (Trajectory *) palloc(size);
	SET_VARSIZE(result, size);

	result->NrPoints = NrPoints;

	result->samplingPeriod = 0.0;

	return result;
}

Trajectory *reallocateTrajectory(Trajectory *trajectory, int32 NrPoints) {
	size_t size;

	size = offsetof(Trajectory, points) + NrPoints * sizeof(PointST);
	trajectory = (Trajectory *) repalloc(trajectory, size);
	SET_VARSIZE(trajectory, size);

	trajectory->NrPoints = NrPoints;

	trajectory->samplingPeriod = 0.0;

	return trajectory;
}

Trajectory *updateMetadataTrajectory(Trajectory *trajectory) {
	int32 i;

	if (trajectory->NrPoints > 1) {
		trajectory->samplingPeriod = differenceInSeconds(trajectory->points[1].t, trajectory->points[0].t);

		for (i = 2; i < trajectory->NrPoints; i++) {
			if (trajectory->samplingPeriod != differenceInSeconds(trajectory->points[i].t, trajectory->points[i - 1].t)) {
				trajectory->samplingPeriod = 0.0;

				break;
			}
		}
	} else
		trajectory->samplingPeriod = 0.0;

	return trajectory;
}

Trajectory *cloneTrajectory(const Trajectory *trajectory) {
	Trajectory *result = (Trajectory *) palloc(VARSIZE(trajectory));

	memcpy(result, trajectory, VARSIZE(trajectory));

	return result;
}

PGDLLEXPORT Datum cloneTrajectoryV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(cloneTrajectory((Trajectory *) PG_GETARG_VARLENA_P(0)));
}
PG_FUNCTION_INFO_V1(cloneTrajectoryV1);

Trajectory *inTrajectory(const char *str, char **error) {
	char *str_dup;
	char *str_temp;

	int32 NrPoints;
	char ts[27];

	PointST point_curr, point_prev, *sequence;

	Trajectory *result = NULL;

	if (error)
		*error = NULL;

	str_dup = pstrdup(str);

	NrPoints = 0;
	str_temp = strtok(str_dup, ",");
	if (str_temp != NULL ) {
		if (sscanf(str_temp, " '%26[^']' %d %d ", ts, &point_curr.sp.x, &point_curr.sp.y) != 3) {
			if (error)
				asprintfHermes(error, "invalid input point syntax for Trajectory: \"%s\"", str);

			pfree(str_dup);
			return NULL;
		}

		point_curr.t = DatumGetTimestamp(DirectFunctionCall3(timestamp_in, CStringGetDatum(ts), PointerGetDatum(0), Int32GetDatum(-1)));

		sequence = (PointST *) palloc(sizeof(point_curr));
		memcpy(sequence, &point_curr, sizeof(point_curr));

		NrPoints = 1;
		point_prev = point_curr;

		str_temp = strtok(NULL, ",");
		if (str_temp != NULL ) {
			if (sscanf(str_temp, " '%26[^']' %d %d ", ts, &point_curr.sp.x, &point_curr.sp.y) != 3) {
				if (error)
					asprintfHermes(error, "invalid input point syntax for Trajectory: \"%s\"", str);

				pfree(str_dup);
				pfree(sequence);
				return NULL;
			}

			point_curr.t = DatumGetTimestamp(DirectFunctionCall3(timestamp_in, CStringGetDatum(ts), PointerGetDatum(0), Int32GetDatum(-1)));

			if (point_curr.t <= point_prev.t) {
				if (error)
					asprintfHermes(error, "invalid time sequence for Trajectory: \"%s\"", str);

				pfree(str_dup);
				pfree(sequence);
				return NULL;
			}

			sequence = (PointST *) repalloc(sequence, 2 * sizeof(point_curr));
			memcpy(sequence + 1, &point_curr, sizeof(point_curr));

			NrPoints = 2;
			point_prev = point_curr;

			str_temp = strtok(NULL, ",");
			while (str_temp != NULL ) {
				if (sscanf(str_temp, " '%26[^']' %d %d ", ts, &point_curr.sp.x, &point_curr.sp.y) != 3) {
					if (error)
						asprintfHermes(error, "invalid input point syntax for Trajectory: \"%s\"", str);

					pfree(str_dup);
					pfree(sequence);
					return NULL;
				}

				point_curr.t = DatumGetTimestamp(DirectFunctionCall3(timestamp_in, CStringGetDatum(ts), PointerGetDatum(0), Int32GetDatum(-1)));

				if (point_curr.t <= point_prev.t) {
					if (error)
						asprintfHermes(error, "invalid time sequence for Trajectory: \"%s\"", str);

					pfree(str_dup);
					pfree(sequence);
					return NULL;
				}

				sequence = (PointST *) repalloc(sequence, (NrPoints + 1) * sizeof(point_curr));
				memcpy(sequence + NrPoints, &point_curr, sizeof(point_curr));

				NrPoints++;
				point_prev = point_curr;
				str_temp = strtok(NULL, ",");
			}
		}
	}

	pfree(str_dup);

	result = allocateTrajectory(NrPoints);

	if (NrPoints > 0) {
		memcpy(result->points, sequence, NrPoints * sizeof(*sequence));
		pfree(sequence);
	}

	return result;
}

PGDLLEXPORT Datum inTrajectoryV1(PG_FUNCTION_ARGS) {
	char *error;
	Trajectory *result = inTrajectory(PG_GETARG_CSTRING(0), &error);

	if (error)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("%s", error)));

	PG_RETURN_POINTER(updateMetadataTrajectory(result));
}
PG_FUNCTION_INFO_V1(inTrajectoryV1);

char *outTrajectory(const Trajectory *trajectory) {
	int32 i;

	int result_size_temp;
	int result_size_curr;

	char *result_temp;
	char *result;

	char *format_curr;

	result = (char *) palloc(1);
	memset(result, '\0', 1);
	result_size_curr = 0;

	for (i = 0; i < trajectory->NrPoints; i++) {
		if (i == 0) {
			format_curr = "'%s' %d %d";
		} else {
			format_curr = ",'%s' %d %d";
		}

		result_size_temp = asprintfHermes(&result_temp, format_curr, DatumGetCString(DirectFunctionCall1(timestamp_out, TimestampGetDatum(trajectory->points[i].t))), trajectory->points[i].sp.x, trajectory->points[i].sp.y);

		result = (char *) repalloc(result, (result_size_curr + result_size_temp + 1) * sizeof(char));
		memcpy(result + result_size_curr, result_temp, (result_size_temp + 1) * sizeof(char));
		pfree(result_temp);

		result_size_curr = result_size_curr + result_size_temp;
	}

	return result;
}

PGDLLEXPORT Datum outTrajectoryV1(PG_FUNCTION_ARGS) {
	PG_RETURN_CSTRING(outTrajectory((Trajectory *) PG_GETARG_VARLENA_P(0)));
}
PG_FUNCTION_INFO_V1(outTrajectoryV1);

PGDLLEXPORT Datum recvTrajectory(PG_FUNCTION_ARGS) {
	StringInfo msg = (StringInfo) PG_GETARG_POINTER(0);

	int32 NrPoints, i;

	Trajectory *result = NULL;

	NrPoints = pq_getmsgint(msg, sizeof(NrPoints));
	if (NrPoints < 0)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("invalid NrPoints for Trajectory")));

	result = allocateTrajectory(NrPoints);

	result->samplingPeriod = pq_getmsgfloat8(msg);

	if (result->NrPoints > 0) {
		result->points[0].t = getmsgTimestampHermes(msg);
		result->points[0].sp.x = pq_getmsgint(msg, sizeof(result->points[0].sp.x));
		result->points[0].sp.y = pq_getmsgint(msg, sizeof(result->points[0].sp.y));

		for (i = 1; i < result->NrPoints; i++) {
			result->points[i].t = getmsgTimestampHermes(msg);
			result->points[i].sp.x = pq_getmsgint(msg, sizeof(result->points[i].sp.x));
			result->points[i].sp.y = pq_getmsgint(msg, sizeof(result->points[i].sp.y));

			if (result->points[i].t <= result->points[i - 1].t) {
				pfree(result);
				ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("invalid time sequence for Trajectory")));
			}
		}
	}

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(recvTrajectory);

bytea *Trajectory2Bytea(const Trajectory *trajectory) {
	int32 i;

	StringInfoData buf;

	pq_begintypsend(&buf);

	pq_sendint(&buf, trajectory->NrPoints, sizeof(trajectory->NrPoints));

	pq_sendfloat8(&buf, ELIMINATE_MINUS_ZERO(trajectory->samplingPeriod));

	for (i = 0; i < trajectory->NrPoints; i++) {
		sendTimestampHermes(&buf, trajectory->points[i].t);
		pq_sendint(&buf, trajectory->points[i].sp.x, sizeof(trajectory->points[i].sp.x));
		pq_sendint(&buf, trajectory->points[i].sp.y, sizeof(trajectory->points[i].sp.y));
	}

	return pq_endtypsend(&buf);
}

PGDLLEXPORT Datum sendTrajectory(PG_FUNCTION_ARGS) {
	PG_RETURN_BYTEA_P(Trajectory2Bytea((Trajectory *) PG_GETARG_VARLENA_P(0)));
}
PG_FUNCTION_INFO_V1(sendTrajectory);

PGDLLEXPORT Datum hashTrajectory(PG_FUNCTION_ARGS) {
	PG_RETURN_DATUM(DirectFunctionCall1(hashvarlena, PointerGetDatum(Trajectory2Bytea((Trajectory *) PG_GETARG_VARLENA_P(0)))));
}
PG_FUNCTION_INFO_V1(hashTrajectory);

bool equalsTrajectory(const Trajectory *trajectoryA, const Trajectory *trajectoryB) {
	int32 i;

	if (trajectoryA->NrPoints != trajectoryB->NrPoints)
		return false;

	for (i = 0; i < trajectoryA->NrPoints; i++)
		if (equalsPointST(&trajectoryA->points[i], &trajectoryB->points[i]) == false)
			return false;

	return true;
}

PGDLLEXPORT Datum equalsTrajectoryV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(equalsTrajectory((Trajectory *) PG_GETARG_VARLENA_P(0), (Trajectory *) PG_GETARG_VARLENA_P(1)));
}
PG_FUNCTION_INFO_V1(equalsTrajectoryV1);
