/*
* @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the RangeSP data type.
 *
 * @see @ref RangeSP
 * @see @ref data_types_spatial
 */

#ifndef HE_RANGESP_PROPERTIES_H_
#define HE_RANGESP_PROPERTIES_H_

#include "Definition.h"

float8 areaRangeSP(const RangeSP *range);
PGDLLEXPORT Datum areaRangeSPV1(PG_FUNCTION_ARGS);

#endif
