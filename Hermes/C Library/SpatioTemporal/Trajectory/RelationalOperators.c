/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/SpatioTemporal/Trajectory/RelationalOperators.h"
#include "../../h/SpatioTemporal/Trajectory/Casts.h"
#include "../../h/SpatioTemporal/Trajectory/Interpolation.h"
#include "../../h/SpatioTemporal/Trajectory/SessionParameters.h"
#include "../../h/Temporal/Period/RelationalOperators.h"
#include "../../h/Spatial/SegmentSP/RelationalOperators.h"

bool intersectsTrajectory_BoxST(Datum trajectory, const BoxST *box) {
	if (atBoxSTTrajectory(trajectory, box, interpolationPrecision, NULL))
		return true;

	return false;
}

bool intersectsTrajectory_Period(Datum trajectory, const Period *period) {
	Period mbp;

	if (Trajectory2Period(trajectory, &mbp))
		return intersectsPeriod_Period(period, &mbp);

	return false;
}

bool intersectsTrajectory_BoxSP(const Trajectory *trajectory, const BoxSP *box) {
	int32 i;
	SegmentSP segment;

	for (i = 1; i < trajectory->NrPoints; i++) {
		segment.i = trajectory->points[i - 1].sp;
		segment.e = trajectory->points[i].sp;

		if (intersectsSegmentSP_BoxSP(&segment, box))
			return true;
	}

	return false;
}

/*
bool intersectsTrajectory_Trajectory(const Trajectory *trajectoryA, const Trajectory *trajectoryB) {
	Period mbpA, mbpB, commonPeriod;
	Trajectory *trajA = NULL, *trajB = NULL;
	int32 NrPointsA, NrPointsB, i, j;
	SegmentST segmentA, segmentB;
	bool foundCommon;
	SegmentSP commonSegment;
	PointSP commonPoint;

	if (intersectionPeriod_Period(Trajectory2Period(trajectoryA, &mbpA), Trajectory2Period(trajectoryB, &mbpB), &commonPeriod)) {
		NrPointsA = NrPointsTrajectory(trajectoryA);
		NrPointsB = NrPointsTrajectory(trajectoryB);

		for (i = 1; i <= NrPointsA - 1; i++) {
			segmentAtTrajectory(trajectoryA, i, &segmentA);

			if (intersectsPeriod_Period(&segmentA.t, &commonPeriod)) {
				for (j = 1; j <= NrPointsB - 1; j++) {
					segmentAtTrajectory(trajectoryB, j, &segmentB);

					if (intersectsPeriod_Period(&segmentA.t, &segmentB.t)) {
						if (intersectionSegmentSP_SegmentSP(&segmentA.sp, &segmentB.sp, &commonSegment, &commonPoint)) {

						}
					}
				}
			}
		}


		if (trajA = atPeriodTrajectory(trajectoryA, &commonPeriod, interpolationPrecision, interpolationVelocityIsPositive)) {
			if (trajB = atPeriodTrajectory(trajectoryB, &commonPeriod, interpolationPrecision, interpolationVelocityIsPositive)) {
				NrPointsA = NrPointsTrajectory(trajA);
				NrPointsB = NrPointsTrajectory(trajB);

				if (NrPointsA == 1) {
					if (NrPointsB == 1) {

					} else if (NrPointsB > 1) {

					} else
						ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("intersectsTrajectory_Trajectory: NrPointsB == 0")));
				} else if (NrPointsA > 1) {
					if (NrPointsB == 1) {

					} else if (NrPointsB > 1) {
						for (i = 1; i <= NrPointsA - 1; i++) {
							segmentAtTrajectory(trajA, i, &segmentA);

							foundCommon = false;
							for (j = 1; j <= NrPointsB - 1; j++) {
								segmentAtTrajectory(trajB, j, &segmentB);

								if (intersectsPeriod_Period(&segmentA.t, &segmentB.t)) {
									if (!foundCommon)
										foundCommon = true;

									if (intersectionSegmentSP_SegmentSP(&segmentA.sp, &segmentB.sp, &commonSegment, &commonPoint)) {
										if () {

										}
									}
								} else {
									if (foundCommon)
										break;
								}
							}
						}
					} else
						ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("intersectsTrajectory_Trajectory: NrPointsB == 0")));
				} else
					ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("intersectsTrajectory_Trajectory: NrPointsA == 0")));
			} else
				pfree(trajA);
		}
	}

	return false;
}
*/

PGDLLEXPORT Datum intersectsTrajectory_BoxSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(intersectsTrajectory_BoxST(PG_GETARG_DATUM(0), (BoxST *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(intersectsTrajectory_BoxSTV1);

PGDLLEXPORT Datum intersectsTrajectory_PeriodV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(intersectsTrajectory_Period(PG_GETARG_DATUM(0), (Period *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(intersectsTrajectory_PeriodV1);

PGDLLEXPORT Datum intersectsTrajectory_BoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(intersectsTrajectory_BoxSP((Trajectory *) PG_GETARG_VARLENA_P(0), (BoxSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(intersectsTrajectory_BoxSPV1);
