/*
* @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the Timestamp data type.
 *
 * @see @ref data_types_temporal
 */

#include "../../h/Temporal/Timestamp/RelationalOperators.h"

bool meetsTimestamp_Period(Timestamp ts, const Period *period) {
	return ts == period->i;
}

PGDLLEXPORT Datum meetsTimestamp_PeriodV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(meetsTimestamp_Period(PG_GETARG_TIMESTAMP(0), (Period *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(meetsTimestamp_PeriodV1);
