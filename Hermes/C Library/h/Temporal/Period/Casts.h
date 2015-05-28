/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_PERIOD_CASTS_H_
#define HE_PERIOD_CASTS_H_

#include "Definition.h"
#include "../RangeT/Definition.h"

RangeT *Period2RangeT(const Period *period, RangeT *result);
PGDLLEXPORT Datum Period2RangeTV1(PG_FUNCTION_ARGS);

#endif
