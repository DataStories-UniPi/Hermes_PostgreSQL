/*
* @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the Period data type.
 *
 * @see @ref Period
 * @see @ref data_types_temporal
 */

#ifndef HE_PERIOD_CASTS_H_
#define HE_PERIOD_CASTS_H_

#include "Definition.h"
#include "../RangeT/Definition.h"

RangeT *Period2RangeT(const Period *period, RangeT *result);
PGDLLEXPORT Datum Period2RangeTV1(PG_FUNCTION_ARGS);

#endif
