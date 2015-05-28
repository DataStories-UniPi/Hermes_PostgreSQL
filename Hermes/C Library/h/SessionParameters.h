/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_SESSION_PARAMETERS_H_
#define	HE_SESSION_PARAMETERS_H_

#include <postgres.h>
#include <fmgr.h>

extern float8 zeroTolerance;
PGDLLEXPORT Datum getZeroTolerance(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum setZeroTolerance(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum setDefaultZeroTolerance(PG_FUNCTION_ARGS);

extern int32 verbosityLevel;
PGDLLEXPORT Datum isVerbose(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum toggleVerbose(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getVerbosityLevel(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum setVerbosityLevel(PG_FUNCTION_ARGS);

extern int32 currentDatasetID;
PGDLLEXPORT Datum getCurrentDatasetID(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum setCurrentDatasetID(PG_FUNCTION_ARGS);

#endif
