/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_BOXSP_INTERACTIONS_H_
#define HE_BOXSP_INTERACTIONS_H_

#include "Definition.h"
#include "../SegmentSP/Definition.h"

BoxSP *intersectionBoxSP_BoxSP(const BoxSP *boxA, const BoxSP *boxB, BoxSP *result);
PGDLLEXPORT Datum intersectionBoxSP_BoxSPV1(PG_FUNCTION_ARGS);

BoxSP *unionBoxSP_PointSP(const BoxSP *box, const PointSP *point, BoxSP *result);
PGDLLEXPORT Datum unionBoxSP_PointSPV1(PG_FUNCTION_ARGS);

BoxSP *unionBoxSP_SegmentSP(const BoxSP *box, const SegmentSP *segment, BoxSP *result);
PGDLLEXPORT Datum unionBoxSP_SegmentSPV1(PG_FUNCTION_ARGS);

BoxSP *unionBoxSP_BoxSP(const BoxSP *boxA, const BoxSP *boxB, BoxSP *result);
PGDLLEXPORT Datum unionBoxSP_BoxSPV1(PG_FUNCTION_ARGS);

#endif
