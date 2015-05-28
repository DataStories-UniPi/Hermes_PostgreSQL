/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_TRAJECTORY_CASTS_H_
#define	HE_TRAJECTORY_CASTS_H_

#include "Definition.h"
#include "../BoxST/Definition.h"

Period *Trajectory2Period(Datum trajectory, Period *result);
BoxSP *Trajectory2BoxSP(const Trajectory *trajectory, BoxSP *result);
BoxST *Trajectory2BoxST(const Trajectory *trajectory, BoxST *result);
PGDLLEXPORT Datum Trajectory2PeriodV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum Trajectory2BoxSPV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum Trajectory2BoxSTV1(PG_FUNCTION_ARGS);

#endif
