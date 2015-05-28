/*
* Authors: Marios Vodas (mvodas@gmail.com).
*/

#include "../../h/SpatioTemporal/Trajectory/Interpolation.h"
#include "../../h/SpatioTemporal/Trajectory/Accessors.h"
#include "../../h/SpatioTemporal/Trajectory/Editors.h"
#include "../../h/SpatioTemporal/Trajectory/Constructors.h"
#include "../../h/SpatioTemporal/Trajectory/SessionParameters.h"
#include "../../h/SpatioTemporal/SegmentST/Accessors.h"
#include "../../h/Temporal/Period/Interactions.h"
#include "../../h/Spatial/PointSP/Distance.h"
#include "../../h/Spatial/SegmentSP/Properties.h"
#include "../../h/Spatial/SegmentSP/ArithmeticOperators.h"
#include "../../h/Spatial/SegmentSP/RelationalOperators.h"
#include "../../h/Spatial/SegmentSP/Interactions.h"
#include "../../h/Spatial/SegmentSP/Distance.h"
#include "../../h/Spatial/BoxSP/RelationalOperators.h"
#include "../../h/Utilities/Utilities.h"
#include "../../h/Utilities/MSVCWorkarounds.h"
#include "../../h/SessionParameters.h"
#include <funcapi.h>
#include <math.h>

float8 *speedAtTrajectory(Datum trajectory, int32 i, const float8 *s0, int32 precision, float8 *result) {
	int32 NrPoints, j;
	Trajectory *subtraj = NULL;
	float8 Dsp, Dt;

	float8 *ret = NULL;

	NrPoints = NrPointsTrajectory(trajectory);
	if (NrPoints > 1 && 0 < i && i <= NrPoints) {
		ret = result ? result : (float8 *) palloc(sizeof(*ret));

		if (s0) {
			if (i == 1)
				*ret = *s0 < 0.0 ? 0.0 : *s0;
			else {
				subtraj = subTrajectory(trajectory, i - 1, i);

				Dsp = distancePointSP_PointSP(&subtraj->points[1].sp, &subtraj->points[0].sp);
				Dt = differenceInSeconds(subtraj->points[1].t, subtraj->points[0].t);
				
				pfree(subtraj);

				*ret = 2.0 * (Dsp / Dt) - (*s0 < 0.0 ? 0.0 : *s0);

				if (*ret < 0.0)
					*ret = 0.0;
			}
		} else {
			if (precision == -1) {
				if (i + 1 <= NrPoints)
					subtraj = subTrajectory(trajectory, i, i + 1);
				else
					subtraj = subTrajectory(trajectory, i - 1, i);
			} else if (precision < 0 || i == 1)
				subtraj = subTrajectory(trajectory, 1, i == 1 ? 2 : i);
			else
				subtraj = subTrajectory(trajectory, i - Min(i - 2, precision) - 1, i);

			Dsp = distancePointSP_PointSP(&subtraj->points[1].sp, &subtraj->points[0].sp);
			Dt = differenceInSeconds(subtraj->points[1].t, subtraj->points[0].t);

			*ret = Dsp / Dt;

			for (j = 2; j < subtraj->NrPoints; j++) {
				Dsp = distancePointSP_PointSP(&subtraj->points[j].sp, &subtraj->points[j - 1].sp);
				Dt = differenceInSeconds(subtraj->points[j].t, subtraj->points[j - 1].t);

				*ret = 2.0 * (Dsp / Dt) - (*ret);

				if (*ret < 0.0)
					*ret = 0.0;
			}

			pfree(subtraj);
		}
	}

	return ret;
}

float8 *accelerationAtTrajectory(Datum trajectory, int32 i, const float8 *s0, int32 precision, float8 *result) {
	int32 NrPoints, starting_point;
	Trajectory *subtraj = NULL;
	float8 speed_prev, speed_next, Dt;
	
	float8 *ret = NULL;

	NrPoints = NrPointsTrajectory(trajectory);
	if (NrPoints > 1 && 0 < i && i < NrPoints) {
		ret = result ? result : (float8 *) palloc(sizeof(*ret));

		if (s0) {
			subtraj = subTrajectory(trajectory, i, i + 1);

			speed_prev = *s0 < 0.0 ? 0.0 : *s0;
		} else {
			if (precision == -1)
				starting_point = i;
			if (precision < 0)
				starting_point = 1;
			else
				starting_point = i - Min((i + 1) - 2, precision);

			subtraj = subTrajectory(trajectory, starting_point, i + 1);

			speedAtTrajectory(PointerGetDatum(subtraj), subtraj->NrPoints - 1, NULL, -2, &speed_prev);
		}

		speedAtTrajectory(PointerGetDatum(subtraj), subtraj->NrPoints, &speed_prev, 0, &speed_next);
		Dt = differenceInSeconds(subtraj->points[subtraj->NrPoints - 1].t, subtraj->points[subtraj->NrPoints - 2].t);

		pfree(subtraj);

		*ret = (speed_next - speed_prev) / Dt;
	}

	return ret;
}

PGDLLEXPORT Datum speedAtTrajectoryV1(PG_FUNCTION_ARGS) {
	float8 s0, ret;

	s0 = PG_GETARG_FLOAT8(2);
	if (s0 < 0.0)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("s0 < 0.0: %f", s0)));

	if (speedAtTrajectory(PG_GETARG_DATUM(0), PG_GETARG_INT32(1), &s0, 0, &ret) == NULL)
		PG_RETURN_NULL();

	PG_RETURN_FLOAT8(ret);
}
PG_FUNCTION_INFO_V1(speedAtTrajectoryV1);

PGDLLEXPORT Datum speedAtTrajectoryV1_2(PG_FUNCTION_ARGS) {
	float8 ret;

	if (speedAtTrajectory(PG_GETARG_DATUM(0), PG_GETARG_INT32(1), NULL, PG_GETARG_INT32(2), &ret) == NULL)
		PG_RETURN_NULL();

	PG_RETURN_FLOAT8(ret);
}
PG_FUNCTION_INFO_V1(speedAtTrajectoryV1_2);

PGDLLEXPORT Datum accelerationAtTrajectoryV1(PG_FUNCTION_ARGS) {
	float8 s0, ret;

	s0 = PG_GETARG_FLOAT8(2);
	if (s0 < 0.0)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("s0 < 0.0: %f", s0)));

	if (accelerationAtTrajectory(PG_GETARG_DATUM(0), PG_GETARG_INT32(1), &s0, 0, &ret) == NULL)
		PG_RETURN_NULL();

	PG_RETURN_FLOAT8(ret);
}
PG_FUNCTION_INFO_V1(accelerationAtTrajectoryV1);

PGDLLEXPORT Datum accelerationAtTrajectoryV1_2(PG_FUNCTION_ARGS) {
	float8 ret;

	if (accelerationAtTrajectory(PG_GETARG_DATUM(0), PG_GETARG_INT32(1), NULL, validateInterpolationPrecision(PG_GETARG_INT32(2)), &ret) == NULL)
		PG_RETURN_NULL();

	PG_RETURN_FLOAT8(ret);
}
PG_FUNCTION_INFO_V1(accelerationAtTrajectoryV1_2);

float8 *speedAtTTrajectory(Datum trajectory, Timestamp ts, int32 precision, float8 *result) {
	Trajectory *Tdt = NULL;
	Datum Td;

	int32 i, j;
	float8 f;

	float8 s0, acceleration, Dt;

	int32 NrPoints;
	float8 samplingPeriod;
	Timestamp ti;

	PointST point;
	
	float8 *ret = NULL;

	NrPoints = NrPointsTrajectory(trajectory);
	if (NrPoints > 1) {
		pointAtTrajectory(trajectory, 1, &point);
		if (ts == point.t)
			return speedAtTrajectory(trajectory, 1, NULL, precision, result);
		if (ts < point.t)
			return NULL;

		ti = point.t;

		pointAtTrajectory(trajectory, NrPoints, &point);
		if (ts == point.t)
			return speedAtTrajectory(trajectory, NrPoints, NULL, precision, result);
		if (ts > point.t)
			return NULL;

		samplingPeriod = samplingPeriodTrajectory(trajectory);

		if (samplingPeriod > 0.0) {
			f = (timestamp2seconds(ts) - timestamp2seconds(ti)) / samplingPeriod;
			i = (int32) floor(f) + 1;
			j = (int32) ceil(f) + 1;

			if (i < 1 || NrPoints < j)
				return NULL;

			if (i == j)
				return speedAtTrajectory(trajectory, i, NULL, precision, result);

			Td = trajectory;
		} else {
			Tdt = (Trajectory *) PG_DETOAST_DATUM(trajectory);

			for (i = 2; i <= Tdt->NrPoints; i++) {
				if (ts == Tdt->points[i - 1].t)
					return speedAtTrajectory(PointerGetDatum(Tdt), i, NULL, precision, result);
				else if (ts < Tdt->points[i - 1].t) {
					i--;

					break;
				}
			}

			if (Tdt->NrPoints < i)
				return NULL;

			Td = PointerGetDatum(Tdt);
		}

		ret = result ? result : (float8 *) palloc(sizeof(*ret));

		speedAtTrajectory(Td, i, NULL, precision - 1, &s0);
		accelerationAtTrajectory(Td, i, &s0, 0, &acceleration);

		if (samplingPeriod > 0.0)
			Dt = differenceInSeconds(ts, timestampPlusSeconds(ti, (i - 1) * samplingPeriod));
		else {
			Dt = differenceInSeconds(ts, Tdt->points[i - 1].t);

			if ((Pointer) Tdt != DatumGetPointer(trajectory))
				pfree(Tdt);
		}
		
		*ret = s0 + acceleration * Dt;

		if (*ret < 0.0)
			*ret = 0.0;
	}

	return ret;
}

float8 *accelerationAtTTrajectory(Datum trajectory, Timestamp ts, int32 precision, float8 *result) {
	Trajectory *Tdt = NULL;
	Datum Td;

	int32 i, j;
	float8 f;

	int32 NrPoints;
	float8 samplingPeriod;
	Timestamp ti;

	PointST point;

	float8 *ret = NULL;

	NrPoints = NrPointsTrajectory(trajectory);
	if (NrPoints > 1) {
		pointAtTrajectory(trajectory, 1, &point);
		if (ts == point.t)
			return accelerationAtTrajectory(trajectory, 1, NULL, precision, result);
		if (ts < point.t)
			return NULL;

		ti = point.t;

		pointAtTrajectory(trajectory, NrPoints, &point);
		if (ts == point.t)
			return accelerationAtTrajectory(trajectory, NrPoints - 1, NULL, precision, result);
		if (ts > point.t)
			return NULL;

		samplingPeriod = samplingPeriodTrajectory(trajectory);

		if (samplingPeriod > 0.0) {
			f = (timestamp2seconds(ts) - timestamp2seconds(ti)) / samplingPeriod;
			i = (int32) floor(f) + 1;
			j = (int32) ceil(f) + 1;

			if (i < 1 || NrPoints < j)
				return NULL;

			if (i == j)
				i--;

			Td = trajectory;
		} else {
			Tdt = (Trajectory *) PG_DETOAST_DATUM(trajectory);

			for (i = 2; i <= Tdt->NrPoints; i++) {
				if (ts <= Tdt->points[i - 1].t) {
					i--;

					break;
				}
			}

			if (Tdt->NrPoints < i)
				return NULL;

			Td = PointerGetDatum(Tdt);
		}

		ret = accelerationAtTrajectory(Td, i, NULL, precision, result);

		if (samplingPeriod == 0.0 && (Pointer) Tdt != DatumGetPointer(trajectory))
			pfree(Tdt);
	}

	return ret;
}

PGDLLEXPORT Datum speedAtTTrajectoryV1(PG_FUNCTION_ARGS) {
	float8 ret;

	if (speedAtTTrajectory(PG_GETARG_DATUM(0), PG_GETARG_TIMESTAMP(1), validateInterpolationPrecision(PG_GETARG_INT32(2)), &ret) == NULL)
		PG_RETURN_NULL();

	PG_RETURN_FLOAT8(ret);
}
PG_FUNCTION_INFO_V1(speedAtTTrajectoryV1);

PGDLLEXPORT Datum accelerationAtTTrajectoryV1(PG_FUNCTION_ARGS) {
	float8 ret;

	if (accelerationAtTTrajectory(PG_GETARG_DATUM(0), PG_GETARG_TIMESTAMP(1), validateInterpolationPrecision(PG_GETARG_INT32(2)), &ret) == NULL)
		PG_RETURN_NULL();

	PG_RETURN_FLOAT8(ret);
}
PG_FUNCTION_INFO_V1(accelerationAtTTrajectoryV1);

PointST *atInstantTrajectory(Datum trajectory, Timestamp ts, int32 precision, PointST *result) {
	Trajectory *Tdt = NULL, *subtraj = NULL;
	Datum Td;

	int32 i, j;
	float8 f;

	float8 s0, acceleration, Dsp, Dt, length;

	int32 NrPoints;
	float8 samplingPeriod;
	Timestamp ti;

	PointST point;

	PointST *ret = NULL;

	NrPoints = NrPointsTrajectory(trajectory);
	if (NrPoints > 0) {
		pointAtTrajectory(trajectory, 1, &point);
		if (ts == point.t)
			return clonePointST(&point, result);
		if (ts < point.t)
			return NULL;

		ti = point.t;

		pointAtTrajectory(trajectory, NrPoints, &point);
		if (ts == point.t)
			return clonePointST(&point, result);
		if (ts > point.t)
			return NULL;

		samplingPeriod = samplingPeriodTrajectory(trajectory);

		if (samplingPeriod > 0.0) {
			f = (timestamp2seconds(ts) - timestamp2seconds(ti)) / samplingPeriod;
			i = (int32) floor(f) + 1;
			j = (int32) ceil(f) + 1;

			if (i < 1 || NrPoints < j)
				return NULL;

			if (i == j)
				return pointAtTrajectory(trajectory, i, result);

			Td = trajectory;
		} else {
			Tdt = (Trajectory *) PG_DETOAST_DATUM(trajectory);

			for (i = 2; i <= Tdt->NrPoints; i++) {
				if (ts == Tdt->points[i - 1].t)
					return clonePointST(&Tdt->points[i - 1], result);
				else if (ts < Tdt->points[i - 1].t) {
					i--;

					break;
				}
			}

			if (Tdt->NrPoints < i)
				return NULL;

			Td = PointerGetDatum(Tdt);
		}

		ret = result ? result : (PointST *) palloc(sizeof(*ret));
		ret->t = ts;

		subtraj = subTrajectory(Td, i, i + 1);

		length = distancePointSP_PointSP(&subtraj->points[1].sp, &subtraj->points[0].sp);

		if (length == 0.0)
			ret->sp = subtraj->points[1].sp;
		else {
			speedAtTrajectory(Td, i, NULL, precision - 1, &s0);
			accelerationAtTrajectory(PointerGetDatum(subtraj), 1, &s0, 0, &acceleration);

			Dt = differenceInSeconds(ts, subtraj->points[0].t);
			Dsp = s0 * Dt + acceleration * pow(Dt, 2.0) / 2.0;

			if (Dsp > length) {
				ret->sp = subtraj->points[1].sp;

				//ereport(INFO, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("Dsp > length: %f > %f", Dsp, length)));
			} else if (Dsp < 0.0) {
				ret->sp = subtraj->points[0].sp;

				//ereport(INFO, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("Dsp < 0.0: %f", Dsp)));
			} else {
				ret->sp.x = (int32) round(subtraj->points[0].sp.x + Dsp * (subtraj->points[1].sp.x - subtraj->points[0].sp.x) / length);
				ret->sp.y = (int32) round(subtraj->points[0].sp.y + Dsp * (subtraj->points[1].sp.y - subtraj->points[0].sp.y) / length);
			}
		}

		pfree(subtraj);

		if (samplingPeriod == 0.0 && (Pointer) Tdt != DatumGetPointer(trajectory))
			pfree(Tdt);
	}

	return ret;
}

PGDLLEXPORT Datum atInstantTrajectoryV1(PG_FUNCTION_ARGS) {
	PointST *ret = atInstantTrajectory(PG_GETARG_DATUM(0), PG_GETARG_TIMESTAMP(1), validateInterpolationPrecision(PG_GETARG_INT32(2)), NULL);

	if (ret == NULL)
		PG_RETURN_NULL();

	PG_RETURN_POINTER(ret);
}
PG_FUNCTION_INFO_V1(atInstantTrajectoryV1);

Trajectory *atPeriodTrajectory(Datum trajectory, const Period *period, int32 precision) {
	Trajectory *Tdt = NULL;
	Datum Td;

	int32 i, j, c;
	float8 f;

	int32 NrPoints;
	float8 samplingPeriod;
	Timestamp ti;

	PointST point, point2;
	Period period_tmp;

	Trajectory *ret = NULL;

	NrPoints = NrPointsTrajectory(trajectory);
	if (NrPoints > 0) {
		pointAtTrajectory(trajectory, 1, &point);
		period_tmp.i = point.t;

		ti = point.t;

		pointAtTrajectory(trajectory, NrPoints, &point2);
		period_tmp.e = point2.t;

		if (intersectionPeriod_Period(&period_tmp, period, &period_tmp)) {
			if (NrPoints == 1 || point.t == period_tmp.e)
				return constructorPointTrajectory(&point);
			if (point2.t == period_tmp.i)
				return constructorPointTrajectory(&point2);

			samplingPeriod = samplingPeriodTrajectory(trajectory);

			if (samplingPeriod > 0.0) {
				f = (timestamp2seconds(period_tmp.i) - timestamp2seconds(ti)) / samplingPeriod;
				i = (int32) floor(f) + 1;

				f = (timestamp2seconds(period_tmp.e) - timestamp2seconds(ti)) / samplingPeriod;
				j = (int32) ceil(f) + 1;

				if (i < 1 || NrPoints < j)
					return NULL;

				Td = trajectory;
			} else {
				Tdt = (Trajectory *) PG_DETOAST_DATUM(trajectory);

				i = j = 0;
				for (c = 1; c < Tdt->NrPoints; c++) {
					if (period_tmp.i == period_tmp.e && period_tmp.e == Tdt->points[c - 1].t) {
						i = j = c;

						break;
					} else if (period_tmp.i == period_tmp.e && period_tmp.e == Tdt->points[c].t) {
						i = j = c + 1;

						break;
					} else {
						if (Tdt->points[c - 1].t <= period_tmp.i && period_tmp.i < Tdt->points[c].t)
							i = c;

						if (Tdt->points[c - 1].t < period_tmp.e && period_tmp.e <= Tdt->points[c].t) {
							j = c + 1;

							break;
						}
					}
				}

				Td = PointerGetDatum(Tdt);
			}

			atInstantTrajectory(Td, period_tmp.i, precision, &point);

			if (period_tmp.i == period_tmp.e)
				ret = constructorPointTrajectory(&point);
			else {
				ret = subTrajectory(Td, i, j);

				ret->points[0] = point;

				atInstantTrajectory(Td, period_tmp.e, precision, &point);
				ret->points[ret->NrPoints - 1] = point;
			}

			if (samplingPeriod == 0.0 && (Pointer) Tdt != DatumGetPointer(trajectory))
				pfree(Tdt);
		}
	}

	return ret;
}

PGDLLEXPORT Datum atPeriodTrajectoryV1(PG_FUNCTION_ARGS) {
	Trajectory *ret = atPeriodTrajectory(PG_GETARG_DATUM(0), (Period *) PG_GETARG_POINTER(1), validateInterpolationPrecision(PG_GETARG_INT32(2)));

	if (ret == NULL)
		PG_RETURN_NULL();

	PG_RETURN_POINTER(updateMetadataTrajectory(ret));
}
PG_FUNCTION_INFO_V1(atPeriodTrajectoryV1);

PointST *atDistanceTrajectory(const Trajectory *trajectory, float8 distance, int32 precision, PointST *result) {
	int32 i;
	SegmentST segment;
	SegmentSP tmp;
	float8 speed_initial, speed_ending, acceleration, Dt;
	float8 length, sum_length = 0.0;

	PointST *ret = NULL;

	if (trajectory->NrPoints > 0) {
		if (distance == 0.0)
			ret = clonePointST(&trajectory->points[0], NULL);
		else if (distance > 0.0) {
			for (i = 1; i <= trajectory->NrPoints - 1; i++) {
				segment.t.i = trajectory->points[i - 1].t;
				segment.sp.i = trajectory->points[i - 1].sp;
				segment.t.e = trajectory->points[i].t;
				segment.sp.e = trajectory->points[i].sp;

				length = lengthSegmentSP(&segment.sp);

				if (sum_length + length >= distance) {
					ret = result ? result : (PointST *) palloc(sizeof(*ret));

					resizeSegmentSP(&segment.sp, distance - sum_length, &tmp);
					ret->sp = tmp.e;

					speedAtTrajectory(PointerGetDatum(trajectory), i, NULL, precision - 1, &speed_initial);
					accelerationAtTrajectory(PointerGetDatum(trajectory), i, &speed_initial, 0, &acceleration);

					speed_ending = sqrt(pow(speed_initial, 2.0) + 2.0 * acceleration * (distance - sum_length));
					Dt = 2.0 * (distance - sum_length) / (speed_ending + speed_initial);

					if (Dt < 0.0)
						ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("Dt < 0.0: %f", Dt)));

					ret->t = timestampPlusSeconds(segment.t.i, Dt);

					break;
				}

				sum_length += length;
			}
		}
	}

	return ret;
}

PGDLLEXPORT Datum atDistanceTrajectoryV1(PG_FUNCTION_ARGS) {
	PointST *ret = atDistanceTrajectory((Trajectory *) PG_GETARG_VARLENA_P(0), PG_GETARG_FLOAT8(1), validateInterpolationPrecision(PG_GETARG_INT32(2)), NULL);

	if (ret == NULL)
		PG_RETURN_NULL();

	PG_RETURN_POINTER(ret);
}
PG_FUNCTION_INFO_V1(atDistanceTrajectoryV1);

int32 atPointTrajectory(const Trajectory *trajectory, const PointSP *point, int32 precision, PointST ***result) {
	int32 i;
	SegmentST segment;
	float8 speed_initial, speed_ending, acceleration, Dsp, Dt;

	int32 nr = 0;

	if (result)
		*result = (PointST **) palloc(sizeof(*result));
	
	if (trajectory->NrPoints == 1 && distancePointSP_PointSP(&trajectory->points[0].sp, point) < zeroTolerance) {
		if (result) {
			(*result)[0] = (PointST *) palloc(sizeof(*(*result)[0]));
			(*result)[0]->t = trajectory->points[0].t;
			(*result)[0]->sp = *point;
		}

		nr = 1;
	} else {
		for (i = 1; i <= trajectory->NrPoints - 1; i++) {
			segment.t.i = trajectory->points[i - 1].t;
			segment.sp.i = trajectory->points[i - 1].sp;
			segment.t.e = trajectory->points[i].t;
			segment.sp.e = trajectory->points[i].sp;

			if (distanceSegmentSP_PointSP(&segment.sp, point) < zeroTolerance) {
				if (i > 1 && equalsPointSP(&segment.sp.i, point))
					continue;

				if (result) {
					*result = (PointST **) repalloc(*result, (nr + 1) * sizeof(*result));

					speedAtTrajectory(PointerGetDatum(trajectory), i, NULL, precision - 1, &speed_initial);
					accelerationAtTrajectory(PointerGetDatum(trajectory), i, &speed_initial, 0, &acceleration);

					Dsp = distancePointSP_PointSP(&segment.sp.i, point);
					speed_ending = sqrt(pow(speed_initial, 2.0) + 2.0 * acceleration * Dsp);
					Dt = 2.0 * Dsp / (speed_ending + speed_initial);

					if (Dt < 0.0)
						ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("Dt < 0.0: %f", Dt)));

					(*result)[nr] = (PointST *) palloc(sizeof(*(*result)[nr]));
					(*result)[nr]->t = timestampPlusSeconds(segment.t.i, Dt);
					(*result)[nr]->sp = *point;
				}

				nr++;
			}
		}
	}

	if (nr == 0 && result)
		pfree(*result);

	return nr;
}

PGDLLEXPORT Datum atPointTrajectoryV1(PG_FUNCTION_ARGS) {
	FuncCallContext *funcctx;

	if (SRF_IS_FIRSTCALL()) {
		MemoryContext oldcontext;

		funcctx = SRF_FIRSTCALL_INIT();
		oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

		funcctx->max_calls = atPointTrajectory((Trajectory *) PG_GETARG_VARLENA_P(0), (PointSP *) PG_GETARG_POINTER(1), validateInterpolationPrecision(PG_GETARG_INT32(2)), (PointST ***) &funcctx->user_fctx);

		MemoryContextSwitchTo(oldcontext);
	}

	funcctx = SRF_PERCALL_SETUP();

	if (funcctx->call_cntr < funcctx->max_calls) {
		uint32 i = funcctx->call_cntr;
		SRF_RETURN_NEXT(funcctx, PointerGetDatum(((PointST **) funcctx->user_fctx)[i]));
	} else {
		if (funcctx->max_calls > 0)
			pfree(funcctx->user_fctx);

		SRF_RETURN_DONE(funcctx);
	}
}
PG_FUNCTION_INFO_V1(atPointTrajectoryV1);

int32 atBoxTrajectory(const Trajectory *trajectory, const BoxSP *box, int32 precision, Trajectory ***result) {
	int32 i;
	SegmentST segment, expSegment;
	SegmentSP commonSegment;
	PointSP commonPoint;
	float8 speed_initial, speed_ending, acceleration, Dsp, Dt;

	int32 nr = 0, nr_intersection_points, enteredAfterLeave = -1;
	bool wasInside = false;

	if (result) {
		*result = (Trajectory **) palloc(sizeof(*result));
		(*result)[0] = allocateTrajectory(0);
	}

	if (trajectory->NrPoints == 1 && containsBoxSP_PointSP(box, &trajectory->points[0].sp)) {
		if (result)
			(*result)[0] = expandOnPointTrajectory((*result)[0], &trajectory->points[0]);

		wasInside = true;
	} else {
		for (i = 1; i <= trajectory->NrPoints - 1; i++) {
			segment.t.i = trajectory->points[i - 1].t;
			segment.sp.i = trajectory->points[i - 1].sp;
			segment.t.e = trajectory->points[i].t;
			segment.sp.e = trajectory->points[i].sp;

			nr_intersection_points = intersectionSegmentSP_BoxSP(&segment.sp, box, false, &commonSegment, &commonPoint);
			if (nr_intersection_points == 2) {
				if (containsBoxSP_PointSP(box, &segment.sp.i)) {
					if (result) {
						expSegment.t.i = segment.t.i;
						expSegment.sp.i = segment.sp.i;

						speedAtTrajectory(PointerGetDatum(trajectory), i, NULL, precision - 1, &speed_initial);
						accelerationAtTrajectory(PointerGetDatum(trajectory), i, &speed_initial, 0, &acceleration);

						Dsp = distancePointSP_PointSP(&segment.sp.i, &commonSegment.e);
						speed_ending = sqrt(pow(speed_initial, 2.0) + 2.0 * acceleration * Dsp);
						Dt = 2.0 * Dsp / (speed_ending + speed_initial);

						if (Dt < 0.0)
							ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("Dt < 0.0: %f", Dt)));

						expSegment.t.e = timestampPlusSeconds(segment.t.i, Dt);
						expSegment.sp.e = commonSegment.e;

						(*result)[nr] = expandOnSegmentTrajectory((*result)[nr], &expSegment);

						*result = (Trajectory **) repalloc(*result, (nr + 2) * sizeof(*result));
						(*result)[nr + 1] = allocateTrajectory(0);
					}

					nr++;
				} else if (containsBoxSP_PointSP(box, &segment.sp.e)) {
					if (result) {
						speedAtTrajectory(PointerGetDatum(trajectory), i, NULL, precision - 1, &speed_initial);
						accelerationAtTrajectory(PointerGetDatum(trajectory), i, &speed_initial, 0, &acceleration);

						Dsp = distancePointSP_PointSP(&segment.sp.i, &commonSegment.i);
						speed_ending = sqrt(pow(speed_initial, 2.0) + 2.0 * acceleration * Dsp);
						Dt = 2.0 * Dsp / (speed_ending + speed_initial);

						if (Dt < 0.0)
							ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("Dt < 0.0: %f", Dt)));

						expSegment.t.i = timestampPlusSeconds(segment.t.i, Dt);
						expSegment.sp.i = commonSegment.i;

						expSegment.t.e = segment.t.e;
						expSegment.sp.e = segment.sp.e;

						(*result)[nr] = expandOnSegmentTrajectory((*result)[nr], &expSegment);
					}

					wasInside = true;
					enteredAfterLeave = nr;
				} else {
					if (result) {
						speedAtTrajectory(PointerGetDatum(trajectory), i, NULL, precision - 1, &speed_initial);
						accelerationAtTrajectory(PointerGetDatum(trajectory), i, &speed_initial, 0, &acceleration);

						Dsp = distancePointSP_PointSP(&segment.sp.i, &commonSegment.i);
						speed_ending = sqrt(pow(speed_initial, 2.0) + 2.0 * acceleration * Dsp);
						Dt = 2.0 * Dsp / (speed_ending + speed_initial);

						if (Dt < 0.0)
							ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("Dt < 0.0: %f", Dt)));

						expSegment.t.i = timestampPlusSeconds(segment.t.i, Dt);
						expSegment.sp.i = commonSegment.i;

						Dsp = distancePointSP_PointSP(&segment.sp.i, &commonSegment.e);
						speed_ending = sqrt(pow(speed_initial, 2.0) + 2.0 * acceleration * Dsp);
						Dt = 2.0 * Dsp / (speed_ending + speed_initial);

						if (Dt < 0.0)
							ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("Dt < 0.0: %f", Dt)));

						expSegment.t.e = timestampPlusSeconds(segment.t.i, Dt);
						expSegment.sp.e = commonSegment.e;

						(*result)[nr] = expandOnSegmentTrajectory((*result)[nr], &expSegment);

						*result = (Trajectory **) repalloc(*result, (nr + 2) * sizeof(*result));
						(*result)[nr + 1] = allocateTrajectory(0);
					}

					nr++;
				}
			} else if (nr_intersection_points == 1) {
				//do nothing
			} else {
				if (containedSegmentSP_BoxSP(&segment.sp, box)) {
					if (result)
						(*result)[nr] = expandOnSegmentTrajectory((*result)[nr], &segment);

					wasInside = true;
				}
			}
		}
	}

	if (nr == 0) {
		if (wasInside) {
			nr = 1;
		} else {
			if (result) {
				pfree((*result)[0]);
				pfree(*result);

				*result = NULL;
			}
		}
	} else {
		if (result) {
			if (((*result)[nr])->NrPoints == 0) {
				pfree((*result)[nr]);
			} else {
				nr++;
			}
		} else {
			if (enteredAfterLeave == nr) {
				nr++;
			}
		}
	}

	return nr;
}

PGDLLEXPORT Datum atBoxTrajectoryV1(PG_FUNCTION_ARGS) {
	FuncCallContext *funcctx;

	if (SRF_IS_FIRSTCALL()) {
		MemoryContext oldcontext;

		funcctx = SRF_FIRSTCALL_INIT();
		oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

		funcctx->max_calls = atBoxTrajectory((Trajectory *) PG_GETARG_VARLENA_P(0), (BoxSP *) PG_GETARG_POINTER(1), validateInterpolationPrecision(PG_GETARG_INT32(2)), (Trajectory ***) &funcctx->user_fctx);

		MemoryContextSwitchTo(oldcontext);
	}

	funcctx = SRF_PERCALL_SETUP();

	if (funcctx->call_cntr < funcctx->max_calls) {
		uint32 i = funcctx->call_cntr;
		SRF_RETURN_NEXT(funcctx, PointerGetDatum(updateMetadataTrajectory(((Trajectory **) funcctx->user_fctx)[i])));
	} else {
		if (funcctx->max_calls > 0)
			pfree(funcctx->user_fctx);

		SRF_RETURN_DONE(funcctx);
	}
}
PG_FUNCTION_INFO_V1(atBoxTrajectoryV1);

int32 atBoxSTTrajectory(Datum trajectory, const BoxST *box, int32 precision, Trajectory ***result) {
	Trajectory *traj = NULL;
	int32 ret = 0;
	
	traj = atPeriodTrajectory(trajectory, &box->t, precision);
	if (traj != NULL) {
		ret = atBoxTrajectory(traj, &box->sp, precision, result);

		pfree(traj);
	}
	
	return ret;
}

PGDLLEXPORT Datum atBoxSTTrajectoryV1(PG_FUNCTION_ARGS) {
	FuncCallContext *funcctx;

	if (SRF_IS_FIRSTCALL()) {
		MemoryContext oldcontext;

		funcctx = SRF_FIRSTCALL_INIT();
		oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

		funcctx->max_calls = atBoxSTTrajectory(PG_GETARG_DATUM(0), (BoxST *) PG_GETARG_POINTER(1), validateInterpolationPrecision(PG_GETARG_INT32(2)), (Trajectory ***) &funcctx->user_fctx);

		MemoryContextSwitchTo(oldcontext);
	}

	funcctx = SRF_PERCALL_SETUP();

	if (funcctx->call_cntr < funcctx->max_calls) {
		uint32 i = funcctx->call_cntr;
		SRF_RETURN_NEXT(funcctx, PointerGetDatum(updateMetadataTrajectory(((Trajectory **) funcctx->user_fctx)[i])));
	} else {
		if (funcctx->max_calls > 0)
			pfree(funcctx->user_fctx);

		SRF_RETURN_DONE(funcctx);
	}
}
PG_FUNCTION_INFO_V1(atBoxSTTrajectoryV1);
