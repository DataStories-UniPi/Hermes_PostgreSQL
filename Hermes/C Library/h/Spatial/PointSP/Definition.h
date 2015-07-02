/*
* @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the PointSP data type.
 *
 * @see @ref PointSP
 * @see @ref data_types_spatial
 */

#ifndef HE_POINTSP_DEFINITION_H_
#define	HE_POINTSP_DEFINITION_H_

#include <postgres.h>
#include <fmgr.h>

/**
 * PointSP is spatial data type and is comprised of
 * the coordinates (usually in meters)
 *
 * @see @ref PointSP/Definition.sql
 * @see @ref PointSP/Distance.sql
 * @see @ref PointSP/RelationalOperators.sql
 * @see @ref PointSP/Casts.sql
 */
typedef struct {
	int32 x; /**< the x coordinate */
	int32 y; /**< the y coordinate */
} PointSP;

PointSP *clonePointSP(const PointSP *point, PointSP *result);
PGDLLEXPORT Datum clonePointSPV1(PG_FUNCTION_ARGS);

/** @brief The function implements the sql constructor function.
 *
 * 	@see @ref PointSP/Definition.sql
 *
 */
PointSP *constructorPointSP(int32 x, int32 y, PointSP *result);
PGDLLEXPORT Datum constructorPointSPV1(PG_FUNCTION_ARGS);

/** @brief The function implements the sql inPointSP function.
 *
 * 	@see @ref PointSP/Definition.sql
 *
 */
PointSP *inPointSP(const char *str, PointSP *result, char **error);
PGDLLEXPORT Datum inPointSPV1(PG_FUNCTION_ARGS);

/** @brief The function implements the sql outPointSP function.
 *
 * 	@see @ref PointSP/Definition.sql
 *
 */
char *outPointSP(const PointSP *point);
PGDLLEXPORT Datum outPointSPV1(PG_FUNCTION_ARGS);

/** @brief The function implements the sql recvPointSP function.
 *
 * 	@see @ref PointSP/Definition.sql
 *
 */
PGDLLEXPORT Datum recvPointSP(PG_FUNCTION_ARGS);

bytea *PointSP2Bytea(const PointSP *point);
PGDLLEXPORT Datum sendPointSP(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum hashPointSP(PG_FUNCTION_ARGS);

bool equalsPointSP(const PointSP *pointA, const PointSP *pointB);
PGDLLEXPORT Datum equalsPointSPV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum getXPointSP(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getYPointSP(PG_FUNCTION_ARGS);

#endif
