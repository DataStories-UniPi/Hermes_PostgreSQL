/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_SPATIAL_SESSION_PARAMETERS_H_
#define	HE_SPATIAL_SESSION_PARAMETERS_H_

#include <postgres.h>
#include <fmgr.h>

extern float8 spatialTolerance;
PGDLLEXPORT Datum getSpatialTolerance(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum setSpatialTolerance(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum setSpatialToleranceToCentimetre(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum setSpatialToleranceToMetre(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum setSpatialToleranceToKilometre(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum setSpatialToleranceToFoot(PG_FUNCTION_ARGS);

#endif
