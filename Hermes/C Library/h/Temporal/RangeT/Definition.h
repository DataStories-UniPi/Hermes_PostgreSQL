/*
* @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the RangeT data type.
 *
 * @see @ref RangeT
 * @see @ref data_types_temporal
 */

#ifndef HE_RANGET_DEFINITION_H_
#define	HE_RANGET_DEFINITION_H_

#include <postgres.h>
#include <fmgr.h>
#include <utils/timestamp.h>

/**
 * A RangeT is comprised of a timestamp and a Interval
 *
 * @see @ref Period/Casts.sql
 * @see @ref Period/Definition.sql
 * @see @ref Period/Distance.sql
 * @see @ref Period/Interactions.sql
 * @see @ref Period/Properties.sql
 * @see @ref Period/RelationalOperators.sql
 */
typedef struct {
	Interval r; /**< the interval r  */
	Timestamp c; /**< the timestamp c  */
} RangeT;

RangeT *cloneRangeT(const RangeT *range, RangeT *result);
PGDLLEXPORT Datum cloneRangeTV1(PG_FUNCTION_ARGS);

RangeT *constructorRangeT(const Interval *r, Timestamp c, RangeT *result, char **error);
PGDLLEXPORT Datum constructorRangeTV1(PG_FUNCTION_ARGS);

RangeT *inRangeT(const char *str, RangeT *result, char **error);
PGDLLEXPORT Datum inRangeTV1(PG_FUNCTION_ARGS);

char *outRangeT(const RangeT *range);
PGDLLEXPORT Datum outRangeTV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum recvRangeT(PG_FUNCTION_ARGS);

bytea *RangeT2Bytea(const RangeT *range);
PGDLLEXPORT Datum sendRangeT(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum hashRangeT(PG_FUNCTION_ARGS);

bool equalsRangeT(const RangeT *rangeA, const RangeT *rangeB);
PGDLLEXPORT Datum equalsRangeTV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum getRRangeT(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getCRangeT(PG_FUNCTION_ARGS);

#endif
