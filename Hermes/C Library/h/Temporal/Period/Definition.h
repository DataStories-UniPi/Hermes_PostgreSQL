/*
* @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the Period data type.
 *
 * @see @ref Period
 * @see @ref data_types_temporal
 */

#ifndef HE_PERIOD_DEFINITION_H_
#define	HE_PERIOD_DEFINITION_H_

#include <postgres.h>
#include <fmgr.h>
#include <utils/timestamp.h>


/**
 * A period is comprised of two timestamps (i, e),
 * meaning initial and ending, thus an interval can be
 * computed from of a period.
 *
 * @see @ref Period/Casts.sql
 * @see @ref Period/Definition.sql
 * @see @ref Period/Distance.sql
 * @see @ref Period/Interactions.sql
 * @see @ref Period/Properties.sql
 * @see @ref Period/RelationalOperators.sql
 */
typedef struct {
	Timestamp i; /**< the initial timestimamp */
	Timestamp e; /**< the ending timestimamp */
} Period;

Period *clonePeriod(const Period *period, Period *result);
PGDLLEXPORT Datum clonePeriodV1(PG_FUNCTION_ARGS);

Period *constructorPeriod(Timestamp i, Timestamp e, Period *result, char **error);
PGDLLEXPORT Datum constructorPeriodV1(PG_FUNCTION_ARGS);

Period *inPeriod(const char *str, Period *result, char **error);
PGDLLEXPORT Datum inPeriodV1(PG_FUNCTION_ARGS);

char *outPeriod(const Period *period);
PGDLLEXPORT Datum outPeriodV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum recvPeriod(PG_FUNCTION_ARGS);

bytea *Period2Bytea(const Period *period);
PGDLLEXPORT Datum sendPeriod(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum hashPeriod(PG_FUNCTION_ARGS);

bool equalsPeriod(const Period *periodA, const Period *periodB);
PGDLLEXPORT Datum equalsPeriodV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum getIPeriod(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getEPeriod(PG_FUNCTION_ARGS);

#endif
