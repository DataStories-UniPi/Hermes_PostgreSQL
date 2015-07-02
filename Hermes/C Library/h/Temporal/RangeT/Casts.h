/*
* @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the RangeT data type.
 *
 * @see @ref RangeT
 * @see @ref data_types_temporal
 */

#ifndef HE_RANGET_CASTS_H_
#define HE_RANGET_CASTS_H_

#include "Definition.h"
#include "../Period/Definition.h"

Period *RangeT2Period(const RangeT *range, Period *result);
PGDLLEXPORT Datum RangeT2PeriodV1(PG_FUNCTION_ARGS);

#endif
