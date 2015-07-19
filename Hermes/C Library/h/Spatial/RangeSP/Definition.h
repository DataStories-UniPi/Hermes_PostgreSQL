/*
* @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the RangeSP data type.
 *
 * @see @ref RangeSP
 * @see @ref data_types_spatial
 */

#ifndef HE_RANGESP_DEFINITION_H_
#define	HE_RANGESP_DEFINITION_H_

#include "../PointSP/Definition.h"

/**
 * RangeSP is spatial data type and is comprised of
 * one PointSP() representing a circle with center
 * the point and radius r
 *
 * @see @ref RangeSP/Casts.sql
 * @see @ref RangeSP/Definition.sql
 * @see @ref RangeSP/Properties.sql
 */
typedef struct {
	int32 r;   /**< the radius of the circle */
	PointSP c; /**< the center of the circle */
} RangeSP;

RangeSP *cloneRangeSP(const RangeSP *range, RangeSP *result);
PGDLLEXPORT Datum cloneRangeSPV1(PG_FUNCTION_ARGS);

RangeSP *constructorRangeSP(int32 r, int32 cx, int32 cy, RangeSP *result, char **error);
PGDLLEXPORT Datum constructorRangeSPV1(PG_FUNCTION_ARGS);

RangeSP *constructorHighRangeSP(int32 r, const PointSP *c, RangeSP *result, char **error);
PGDLLEXPORT Datum constructorHighRangeSPV1(PG_FUNCTION_ARGS);

RangeSP *inRangeSP(const char *str, RangeSP *result, char **error);
PGDLLEXPORT Datum inRangeSPV1(PG_FUNCTION_ARGS);

char *outRangeSP(const RangeSP *range);
PGDLLEXPORT Datum outRangeSPV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum recvRangeSP(PG_FUNCTION_ARGS);

bytea *RangeSP2Bytea(const RangeSP *range);
PGDLLEXPORT Datum sendRangeSP(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum hashRangeSP(PG_FUNCTION_ARGS);

bool equalsRangeSP(const RangeSP *rangeA, const RangeSP *rangeB);
PGDLLEXPORT Datum equalsRangeSPV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum getRRangeSP(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getCRangeSP(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getXRangeSP(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getYRangeSP(PG_FUNCTION_ARGS);

#endif
