/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_BOXSP_CASTS_H_
#define HE_BOXSP_CASTS_H_

#include "Definition.h"
#include "../RangeSP/Definition.h"

RangeSP *BoxSP2RangeSP(const BoxSP *box, RangeSP *result);
PGDLLEXPORT Datum BoxSP2RangeSPV1(PG_FUNCTION_ARGS);

#endif
