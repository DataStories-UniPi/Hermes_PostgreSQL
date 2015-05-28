/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_RANGEST_CASTS_H_
#define HE_RANGEST_CASTS_H_

#include "Definition.h"
#include "../BoxST/Definition.h"

BoxST *RangeST2BoxST(const RangeST *range, BoxST *result);
PGDLLEXPORT Datum RangeST2BoxSTV1(PG_FUNCTION_ARGS);

#endif
