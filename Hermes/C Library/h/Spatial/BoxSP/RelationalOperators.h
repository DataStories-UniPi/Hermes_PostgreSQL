/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_BOXSP_RELATIONAL_OPERATORS_H_
#define HE_BOXSP_RELATIONAL_OPERATORS_H_

#include "Definition.h"

bool intersectsBoxSP_BoxSP(const BoxSP *boxA, const BoxSP *boxB);
PGDLLEXPORT Datum intersectsBoxSP_BoxSPV1(PG_FUNCTION_ARGS);

bool containsBoxSP_PointSP(const BoxSP *box, const PointSP *point);
bool containsBoxSP_BoxSP(const BoxSP *boxA, const BoxSP *boxB);
PGDLLEXPORT Datum containsBoxSP_PointSPV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum containsBoxSP_BoxSPV1(PG_FUNCTION_ARGS);

bool containsProperlyBoxSP_PointSP(const BoxSP *box, const PointSP *point);
bool containsProperlyBoxSP_BoxSP(const BoxSP *boxA, const BoxSP *boxB);
PGDLLEXPORT Datum containsProperlyBoxSP_PointSPV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum containsProperlyBoxSP_BoxSPV1(PG_FUNCTION_ARGS);

bool northBoxSP_PointSP(const BoxSP *box, const PointSP *point);
bool northBoxSP_BoxSP(const BoxSP *boxA, const BoxSP *boxB);
PGDLLEXPORT Datum northBoxSP_PointSPV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum northBoxSP_BoxSPV1(PG_FUNCTION_ARGS);

bool southBoxSP_PointSP(const BoxSP *box, const PointSP *point);
bool southBoxSP_BoxSP(const BoxSP *boxA, const BoxSP *boxB);
PGDLLEXPORT Datum southBoxSP_PointSPV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum southBoxSP_BoxSPV1(PG_FUNCTION_ARGS);

bool eastBoxSP_PointSP(const BoxSP *box, const PointSP *point);
bool eastBoxSP_BoxSP(const BoxSP *boxA, const BoxSP *boxB);
PGDLLEXPORT Datum eastBoxSP_PointSPV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum eastBoxSP_BoxSPV1(PG_FUNCTION_ARGS);

bool westBoxSP_PointSP(const BoxSP *box, const PointSP *point);
bool westBoxSP_BoxSP(const BoxSP *boxA, const BoxSP *boxB);
PGDLLEXPORT Datum westBoxSP_PointSPV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum westBoxSP_BoxSPV1(PG_FUNCTION_ARGS);

#endif
