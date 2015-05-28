/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_TRAJECTORY_SESSION_PARAMETERS_H_
#define	HE_TRAJECTORY_SESSION_PARAMETERS_H_

#include <postgres.h>
#include <fmgr.h>

extern int32 interpolationPrecision;
int32 validateInterpolationPrecision(int32 interpolationPrecision);
PGDLLEXPORT Datum getInterpolationPrecision(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum setInterpolationPrecision(PG_FUNCTION_ARGS);

#endif
