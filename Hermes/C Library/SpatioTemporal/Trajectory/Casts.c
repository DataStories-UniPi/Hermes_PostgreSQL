/**
 *	@file
 *	@author Marios Vodas (mvodas@gmail.com).
 *	@brief The implementation of Trajectory
 *
 *	@details These functions are documented in the @ref Trajectory/Casts.h
 *
 */

#include "../../h/SpatioTemporal/Trajectory/Casts.h"
#include "../../h/SpatioTemporal/Trajectory/Accessors.h"

Period *Trajectory2Period(Datum trajectory, Period *result) {
	int32 NrPoints;
	PointST point;
	Period *ret = NULL;

	NrPoints = NrPointsTrajectory(trajectory);
	if (NrPoints > 0) {
		ret = result ? result : (Period *) palloc(sizeof(*ret));

		pointAtTrajectory(trajectory, 1, &point);
		ret->i = point.t;

		pointAtTrajectory(trajectory, NrPoints, &point);
		ret->e = point.t;
	}

	return ret;
}

BoxSP *Trajectory2BoxSP(const Trajectory *trajectory, BoxSP *result) {
	int32 i;
	BoxSP *ret = NULL;

	if (trajectory->NrPoints > 0) {
		ret = result ? result : (BoxSP *) palloc(sizeof(*ret));

		ret->l = trajectory->points[0].sp;
		ret->h = trajectory->points[0].sp;

		for (i = 1; i < trajectory->NrPoints; i++) {
			ret->l.x = (ret->l.x < trajectory->points[i].sp.x) ? ret->l.x : trajectory->points[i].sp.x;
			ret->l.y = (ret->l.y < trajectory->points[i].sp.y) ? ret->l.y : trajectory->points[i].sp.y;
			ret->h.x = (ret->h.x > trajectory->points[i].sp.x) ? ret->h.x : trajectory->points[i].sp.x;
			ret->h.y = (ret->h.y > trajectory->points[i].sp.y) ? ret->h.y : trajectory->points[i].sp.y;
		}
	}

	return ret;
}

BoxST *Trajectory2BoxST(const Trajectory *trajectory, BoxST *result) {
	int32 i;
	BoxST *ret = NULL;

	if (trajectory->NrPoints > 0) {
		ret = result ? result : (BoxST *) palloc(sizeof(*ret));

		ret->t.i = trajectory->points[0].t;
		ret->t.e = trajectory->points[0].t;
		ret->sp.l = trajectory->points[0].sp;
		ret->sp.h = trajectory->points[0].sp;

		for (i = 1; i < trajectory->NrPoints; i++) {
			ret->t.e = trajectory->points[i].t;
			ret->sp.l.x = (ret->sp.l.x < trajectory->points[i].sp.x) ? ret->sp.l.x : trajectory->points[i].sp.x;
			ret->sp.l.y = (ret->sp.l.y < trajectory->points[i].sp.y) ? ret->sp.l.y : trajectory->points[i].sp.y;
			ret->sp.h.x = (ret->sp.h.x > trajectory->points[i].sp.x) ? ret->sp.h.x : trajectory->points[i].sp.x;
			ret->sp.h.y = (ret->sp.h.y > trajectory->points[i].sp.y) ? ret->sp.h.y : trajectory->points[i].sp.y;
		}
	}

	return ret;
}

PGDLLEXPORT Datum Trajectory2PeriodV1(PG_FUNCTION_ARGS) {
	Period *ret = Trajectory2Period(PG_GETARG_DATUM(0), NULL);

	if (ret == NULL)
		PG_RETURN_NULL();

	PG_RETURN_POINTER(ret);
}
PG_FUNCTION_INFO_V1(Trajectory2PeriodV1);

PGDLLEXPORT Datum Trajectory2BoxSPV1(PG_FUNCTION_ARGS) {
	BoxSP *ret = Trajectory2BoxSP((Trajectory *) PG_GETARG_VARLENA_P(0), NULL);

	if (ret == NULL)
		PG_RETURN_NULL();

	PG_RETURN_POINTER(ret);
}
PG_FUNCTION_INFO_V1(Trajectory2BoxSPV1);

PGDLLEXPORT Datum Trajectory2BoxSTV1(PG_FUNCTION_ARGS) {
	BoxST *ret = Trajectory2BoxST((Trajectory *) PG_GETARG_VARLENA_P(0), NULL);

	if (ret == NULL)
		PG_RETURN_NULL();

	PG_RETURN_POINTER(ret);
}
PG_FUNCTION_INFO_V1(Trajectory2BoxSTV1);
