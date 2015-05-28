/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_RANGESP_PROPERTIES_H_
#define HE_RANGESP_PROPERTIES_H_

#include "Definition.h"

float8 areaRangeSP(const RangeSP *range);
PGDLLEXPORT Datum areaRangeSPV1(PG_FUNCTION_ARGS);

#endif
