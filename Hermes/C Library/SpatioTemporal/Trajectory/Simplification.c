/**
 *	@file
 *	@author Marios Vodas (mvodas@gmail.com).
 *	@brief The implementation of Trajectory
 *
 *	@details These functions are documented in the @ref Trajectory/Simplification.h
 *
 */

#include "../../h/SpatioTemporal/Trajectory/Simplification.h"
#include "../../h/SpatioTemporal/Trajectory/Interpolation.h"
#include "../../h/SpatioTemporal/Trajectory/SessionParameters.h"
#include "../../h/Spatial/SegmentSP/Distance.h"
#include "../../h/Utilities/Utilities.h"
#include <math.h>

Trajectory *simplifyTrajectory(const Trajectory *trajectory, float8 samplingPeriod, bool strict, int32 precision) {
	int32 NrPoints;
	Timestamp interpolatedInstant;
	int32 i;

	Trajectory *result = NULL;

	if (trajectory->NrPoints == 0)
		return NULL;
	else if (trajectory->NrPoints == 1)
		return cloneTrajectory(trajectory);

	if (strict)
		NrPoints = (int32) floor(differenceInSeconds(trajectory->points[trajectory->NrPoints - 1].t, trajectory->points[0].t) / samplingPeriod) + 1;
	else
		NrPoints = (int32) ceil(differenceInSeconds(trajectory->points[trajectory->NrPoints - 1].t, trajectory->points[0].t) / samplingPeriod) + 1;

	result = allocateTrajectory(NrPoints);

	result->points[0] = trajectory->points[0];

	NrPoints = strict ? NrPoints : NrPoints - 1;
	for (i = 1, interpolatedInstant = timestampPlusSeconds(trajectory->points[0].t, samplingPeriod); i < NrPoints; i++, interpolatedInstant = timestampPlusSeconds(interpolatedInstant, samplingPeriod))
		atInstantTrajectory(PointerGetDatum(trajectory), interpolatedInstant, precision, &result->points[i]);

	if (!strict)
		result->points[result->NrPoints - 1] = trajectory->points[trajectory->NrPoints - 1];

	return result;
}

Trajectory *simplifyEpochTrajectory(const Trajectory *trajectory, float8 samplingPeriod, bool strict, int32 precision) {
	int32 s, e;
	bool tailS, tailE;

	int32 i, j;

	int32 NrInterpolatedPoints;
	PointST *interpolatedPoints = NULL;

	Trajectory *result = NULL;

	if (trajectory->NrPoints == 0) {
		return NULL;
	} else if (trajectory->NrPoints == 1) {
		if (strict)
			return NULL;
		else
			return cloneTrajectory(trajectory);
	}

	s = (int32) ceil(timestamp2seconds(trajectory->points[0].t) / samplingPeriod);
	tailS = seconds2timestamp(s * samplingPeriod) == trajectory->points[0].t ? false : true;

	e = (int32) ceil(timestamp2seconds(trajectory->points[trajectory->NrPoints - 1].t) / samplingPeriod);
	tailE = seconds2timestamp(e * samplingPeriod) == trajectory->points[trajectory->NrPoints - 1].t ? false : true;
	
	NrInterpolatedPoints = e - s;
	if (NrInterpolatedPoints > 0) {
		interpolatedPoints = (PointST *) palloc(NrInterpolatedPoints * sizeof(*interpolatedPoints));
		for (i = s, j = 0; i < e; i++, j++)
			atInstantTrajectory(PointerGetDatum(trajectory), seconds2timestamp(i * samplingPeriod), precision, &interpolatedPoints[j]);
	}
	
	if (strict) {
		if (NrInterpolatedPoints == 1 && !tailE) {
				result = allocateTrajectory(2);

				result->points[0] = *interpolatedPoints;
				result->points[1] = trajectory->points[trajectory->NrPoints - 1];
		} else if (NrInterpolatedPoints > 1) {
			if (!tailE) {
				result = allocateTrajectory(NrInterpolatedPoints + 1);

				memcpy(result->points, interpolatedPoints, NrInterpolatedPoints * sizeof(PointST));
				result->points[result->NrPoints - 1] = trajectory->points[trajectory->NrPoints - 1];
			} else {
				result = allocateTrajectory(NrInterpolatedPoints);

				memcpy(result->points, interpolatedPoints, NrInterpolatedPoints * sizeof(PointST));
			}
		}
	} else {
		if (tailS) {
			result = allocateTrajectory(NrInterpolatedPoints + 2);

			result->points[0] = trajectory->points[0];
			memcpy(&result->points[1], interpolatedPoints, NrInterpolatedPoints * sizeof(PointST));
			result->points[result->NrPoints - 1] = trajectory->points[trajectory->NrPoints - 1];
		} else {
			result = allocateTrajectory(NrInterpolatedPoints + 1);

			memcpy(result->points, interpolatedPoints, NrInterpolatedPoints * sizeof(PointST));
			result->points[result->NrPoints - 1] = trajectory->points[trajectory->NrPoints - 1];
		}
	}

	if (NrInterpolatedPoints > 0)
		pfree(interpolatedPoints);

	return result;
}

PGDLLEXPORT Datum simplifyTrajectoryV1(PG_FUNCTION_ARGS) {
	Trajectory *ret = NULL;

	if (PG_GETARG_BOOL(3))
		ret = simplifyEpochTrajectory((Trajectory *) PG_GETARG_VARLENA_P(0), interval2seconds(PG_GETARG_INTERVAL_P(1)), PG_GETARG_BOOL(2), validateInterpolationPrecision(PG_GETARG_INT32(4)));
	else
		ret = simplifyTrajectory((Trajectory *) PG_GETARG_VARLENA_P(0), interval2seconds(PG_GETARG_INTERVAL_P(1)), PG_GETARG_BOOL(2), validateInterpolationPrecision(PG_GETARG_INT32(4)));

	if (ret == NULL)
		PG_RETURN_NULL();

	PG_RETURN_POINTER(updateMetadataTrajectory(ret));
}
PG_FUNCTION_INFO_V1(simplifyTrajectoryV1);

static int32 _simplifyDPTrajectory(const Trajectory *trajectory, int32 j, int32 k, float8 eps, bool plus, int32 **ret) {
	SegmentSP segment;
	
	float8 dist;
	int32 idist;

	float8 maxdist = 0.0;
	int32 mincend = (k - j) / 2;

	int32 i, z;
	int32 ceni = j + (k - j) / 2;
	
	int32 maxi = j;
	int32 minceni = -1;

	int32 nl, nr;
	int32 *l = NULL, *r = NULL;

	*ret = NULL;

	if (k <= j + 1) {
		*ret = (int32 *) palloc(2 * sizeof(**ret));
		(*ret)[0] = j;
		(*ret)[1] = k;

		return 2;
	}

	segment.i = trajectory->points[j].sp;
	segment.e = trajectory->points[k].sp;

	for (i = j + 1; i < k; i++) {
		dist = distanceSegmentSP_PointSP(&segment, &trajectory->points[i].sp);

		if (plus) {
			idist = abs(ceni - i);

			if (idist < mincend) {
				minceni = i;
				mincend = idist;
			}
		}

		if (dist > maxdist) {
			maxi = i;
			maxdist = dist;
		}
	}

	if (maxdist > eps) {
		if (plus && minceni > -1)
			maxi = minceni;

		nl = _simplifyDPTrajectory(trajectory, j, maxi, eps, plus, &l);
		nr = _simplifyDPTrajectory(trajectory, maxi, k, eps, plus, &r);

		*ret = (int32 *) palloc((nl + nr - 1) * sizeof(**ret));

		for (i = 0, z = 0; i < nl - 1; i++, z++)
			(*ret)[z] = l[i];
		for (i = 0; i < nr; i++, z++)
			(*ret)[z] = r[i];

		return nl + nr - 1;
	} else {
		*ret = (int32 *) palloc(2 * sizeof(**ret));
		(*ret)[0] = j;
		(*ret)[1] = k;

		return 2;
	}
}

Trajectory *simplifyDPTrajectory(const Trajectory *trajectory, float8 eps, bool plus) {
	int32 i;
	int32 *p = NULL;

	Trajectory *ret = NULL;

	if (trajectory->NrPoints == 1) {
		ret = cloneTrajectory(trajectory);
	} else if (trajectory->NrPoints > 1) {
		ret = allocateTrajectory(_simplifyDPTrajectory(trajectory, 0, trajectory->NrPoints - 1, eps, plus, &p));

		for (i = 0; i < ret->NrPoints; i++)
			ret->points[i] = trajectory->points[p[i]];
	}

	return ret;
}

PGDLLEXPORT Datum simplifyDPTrajectoryV1(PG_FUNCTION_ARGS) {
	Trajectory *ret = simplifyDPTrajectory((Trajectory *) PG_GETARG_VARLENA_P(0), PG_GETARG_FLOAT8(1), PG_GETARG_BOOL(2));

	if (ret == NULL)
		PG_RETURN_NULL();

	PG_RETURN_POINTER(updateMetadataTrajectory(ret));
}
PG_FUNCTION_INFO_V1(simplifyDPTrajectoryV1);
