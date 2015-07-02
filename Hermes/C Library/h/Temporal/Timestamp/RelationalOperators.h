/*
* @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the Timestamp data type.
 *
 * @see @ref data_types_temporal
 */

#ifndef HE_TIMESTAMP_RELATIONAL_OPERATORS_H_
#define HE_TIMESTAMP_RELATIONAL_OPERATORS_H_

#include "../Period/Definition.h"

bool meetsTimestamp_Period(Timestamp ts, const Period *period);
PGDLLEXPORT Datum meetsTimestamp_PeriodV1(PG_FUNCTION_ARGS);

#endif
