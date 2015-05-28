/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_BOXSP_DISTANCE_H_
#define HE_BOXSP_DISTANCE_H_

#include "Definition.h"
#include "../SegmentSP/Definition.h"

float8 distanceBoxSP_PointSP(const BoxSP *box, const PointSP *point); //TODO this implementation is poor. it works but should be fixed in the future.
PGDLLEXPORT Datum distanceBoxSP_PointSPV1(PG_FUNCTION_ARGS);

float8 distanceBoxSP_SegmentSP(const BoxSP *box, const SegmentSP *segment);
PGDLLEXPORT Datum distanceBoxSP_SegmentSPV1(PG_FUNCTION_ARGS);

float8 distanceBoxSP_BoxSP(const BoxSP *boxA, const BoxSP *boxB);
PGDLLEXPORT Datum distanceBoxSP_BoxSPV1(PG_FUNCTION_ARGS);

#endif
