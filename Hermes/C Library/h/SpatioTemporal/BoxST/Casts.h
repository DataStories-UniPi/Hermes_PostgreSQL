/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_BOXST_CASTS_H_
#define HE_BOXST_CASTS_H_

#include "Definition.h"
#include "../RangeST/Definition.h"

RangeST *BoxST2RangeST(const BoxST *box, RangeST *result);
PGDLLEXPORT Datum BoxST2RangeSTV1(PG_FUNCTION_ARGS);

#endif
