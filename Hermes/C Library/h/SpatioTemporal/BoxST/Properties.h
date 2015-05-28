/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_BOXST_PROPERTIES_H_
#define HE_BOXST_PROPERTIES_H_

#include "Definition.h"

bool isPunctalBoxST(const BoxST *box);
PGDLLEXPORT Datum isPunctalBoxSTV1(PG_FUNCTION_ARGS);

#endif
