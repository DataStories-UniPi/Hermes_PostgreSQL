/*
* @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the BoxSP data type.
 *
 * @see @ref BoxSP
 * @see @ref data_types_spatial
 */

#ifndef HE_BOXSP_CASTS_H_
#define HE_BOXSP_CASTS_H_

#include "Definition.h"
#include "../RangeSP/Definition.h"

RangeSP *BoxSP2RangeSP(const BoxSP *box, RangeSP *result);
PGDLLEXPORT Datum BoxSP2RangeSPV1(PG_FUNCTION_ARGS);

#endif
