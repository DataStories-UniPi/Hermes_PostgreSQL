/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_BOXST_INTERACTIONS_H_
#define HE_BOXST_INTERACTIONS_H_

#include "Definition.h"
#include "../PointST/Definition.h"
#include "../SegmentST/Definition.h"

BoxST *intersectionBoxST_BoxST(const BoxST *boxA, const BoxST *boxB, BoxST *result);
PGDLLEXPORT Datum intersectionBoxST_BoxSTV1(PG_FUNCTION_ARGS);

BoxST *unionBoxST_PointST(const BoxST *box, const PointST *point, BoxST *result);
PGDLLEXPORT Datum unionBoxST_PointSTV1(PG_FUNCTION_ARGS);

BoxST *unionBoxST_SegmentST(const BoxST *box, const SegmentST *segment, BoxST *result);
PGDLLEXPORT Datum unionBoxST_SegmentSTV1(PG_FUNCTION_ARGS);

BoxST *unionBoxST_BoxST(const BoxST *boxA, const BoxST *boxB, BoxST *result);
PGDLLEXPORT Datum unionBoxST_BoxSTV1(PG_FUNCTION_ARGS);

#endif
