/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_RANGESP_CASTS_H_
#define HE_RANGESP_CASTS_H_

#include "Definition.h"
#include "../BoxSP/Definition.h"

BoxSP *RangeSP2BoxSP(const RangeSP *range, BoxSP *result);
PGDLLEXPORT Datum RangeSP2BoxSPV1(PG_FUNCTION_ARGS);

#endif
