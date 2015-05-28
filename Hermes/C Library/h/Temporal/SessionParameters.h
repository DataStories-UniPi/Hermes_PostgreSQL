/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_TEMPORAL_SESSION_PARAMETERS_H_
#define	HE_TEMPORAL_SESSION_PARAMETERS_H_

#include <postgres.h>
#include <fmgr.h>

extern float8 temporalTolerance;
PGDLLEXPORT Datum getTemporalTolerance(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum setTemporalTolerance(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum setTemporalToleranceToSecond(PG_FUNCTION_ARGS);

#endif
