/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_TRAJECTORY_RELATIONAL_OPERATORS_H_
#define HE_TRAJECTORY_RELATIONAL_OPERATORS_H_

#include "Definition.h"
#include "../BoxST/Definition.h"

bool intersectsTrajectory_BoxST(Datum trajectory, const BoxST *box);
bool intersectsTrajectory_Period(Datum trajectory, const Period *period);
bool intersectsTrajectory_BoxSP(const Trajectory *trajectory, const BoxSP *box);
PGDLLEXPORT Datum intersectsTrajectory_BoxSTV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum intersectsTrajectory_PeriodV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum intersectsTrajectory_BoxSPV1(PG_FUNCTION_ARGS);

#endif
