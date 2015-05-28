/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_UNIT_CONVERSION_H_
#define	HE_UNIT_CONVERSION_H_

#include <postgres.h>
#include <fmgr.h>

float8 km2metres(float8 km);
float8 metres2km(float8 metres);
PGDLLEXPORT Datum km2metresV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum metres2kmV1(PG_FUNCTION_ARGS);

float8 nm2metres(float8 nm);
float8 metres2nm(float8 metres);
PGDLLEXPORT Datum nm2metresV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum metres2nmV1(PG_FUNCTION_ARGS);

float8 kmph2mps(float8 kmph);
float8 mps2kmph(float8 mps);
PGDLLEXPORT Datum kmph2mpsV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum mps2kmphV1(PG_FUNCTION_ARGS);

float8 knots2mps(float8 knots);
float8 mps2knots(float8 mps);
PGDLLEXPORT Datum knots2mpsV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum mps2knotsV1(PG_FUNCTION_ARGS);

#endif
