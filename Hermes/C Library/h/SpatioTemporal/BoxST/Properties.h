/*
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the BoxST data type.
 *
 * @see @ref BoxST
 * @see @ref data_types_spatio_temp
 */

#ifndef HE_BOXST_PROPERTIES_H_
#define HE_BOXST_PROPERTIES_H_

#include "Definition.h"

bool isPunctalBoxST(const BoxST *box);
PGDLLEXPORT Datum isPunctalBoxSTV1(PG_FUNCTION_ARGS);

#endif
