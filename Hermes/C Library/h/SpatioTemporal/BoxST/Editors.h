/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_BOXST_EDITORS_H_
#define	HE_BOXST_EDITORS_H_

#include "Definition.h"

BoxST *bufferBoxST(const BoxST *box, float8 tSize, int32 xSize, int32 ySize, BoxST *result);
PGDLLEXPORT Datum bufferBoxSTV1(PG_FUNCTION_ARGS);

BoxST *enlargeBoxST(const BoxST *box, float8 tSize, int32 spSize, BoxST *result);
PGDLLEXPORT Datum enlargeBoxSTV1(PG_FUNCTION_ARGS);

#endif
