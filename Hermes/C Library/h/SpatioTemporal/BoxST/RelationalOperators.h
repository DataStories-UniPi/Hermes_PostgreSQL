/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_BOXST_RELATIONAL_OPERATORS_H_
#define HE_BOXST_RELATIONAL_OPERATORS_H_

#include "Definition.h"
#include "../PointST/Definition.h"

bool intersectsBoxST_BoxST(const BoxST *boxA, const BoxST *boxB);
PGDLLEXPORT Datum intersectsBoxST_BoxSTV1(PG_FUNCTION_ARGS);

bool containsBoxST_PointST(const BoxST *box, const PointST *point);
bool containsBoxST_BoxST(const BoxST *boxA, const BoxST *boxB);
PGDLLEXPORT Datum containsBoxST_PointSTV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum containsBoxST_BoxSTV1(PG_FUNCTION_ARGS);

bool containsProperlyBoxST_PointST(const BoxST *box, const PointST *point);
bool containsProperlyBoxST_BoxST(const BoxST *boxA, const BoxST *boxB);
PGDLLEXPORT Datum containsProperlyBoxST_PointSTV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum containsProperlyBoxST_BoxSTV1(PG_FUNCTION_ARGS);

#endif
