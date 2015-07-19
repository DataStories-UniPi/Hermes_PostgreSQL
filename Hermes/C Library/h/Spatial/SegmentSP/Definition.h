/*
* @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the SegmentSP data type.
 *
 * @see @ref SegmentSP
 * @see @ref data_types_spatial
 */

#ifndef HE_SEGMENTSP_DEFINITION_H_
#define	HE_SEGMENTSP_DEFINITION_H_

#include "../PointSP/Definition.h"

/**
 * PointSP is spatial data type and is comprised of
 * two PointSP(i,e) representing the segment (of a line)
 *
 * @see @ref SegmentSP/ArithmeticOperators.sql
 * @see @ref SegmentSP/Casts.sql
 * @see @ref SegmentSP/Definition.sql
 * @see @ref SegmentSP/Distance.sql
 * @see @ref SegmentSP/Interactions.sql
 * @see @ref SegmentSP/Properties.sql
 * @see @ref SegmentSP/RelationalOperators.sql
 *
 */
typedef struct {
	PointSP i; /**< the initial point of the segment */
	PointSP e; /**< the ending point of the segment */
} SegmentSP;

SegmentSP *cloneSegmentSP(const SegmentSP *segment, SegmentSP *result);
PGDLLEXPORT Datum cloneSegmentSPV1(PG_FUNCTION_ARGS);

SegmentSP *constructorSegmentSP(int32 ix, int32 iy, int32 ex, int32 ey, SegmentSP *result, char **error);
PGDLLEXPORT Datum constructorSegmentSPV1(PG_FUNCTION_ARGS);

SegmentSP *constructorHighSegmentSP(const PointSP *i, const PointSP *e, SegmentSP *result, char **error);
PGDLLEXPORT Datum constructorHighSegmentSPV1(PG_FUNCTION_ARGS);

SegmentSP *inSegmentSP(const char *str, SegmentSP *result, char **error);
PGDLLEXPORT Datum inSegmentSPV1(PG_FUNCTION_ARGS);

char *outSegmentSP(const SegmentSP *segment);
PGDLLEXPORT Datum outSegmentSPV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum recvSegmentSP(PG_FUNCTION_ARGS);

bytea *SegmentSP2Bytea(const SegmentSP *segment);
PGDLLEXPORT Datum sendSegmentSP(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum hashSegmentSP(PG_FUNCTION_ARGS);

bool equalsSegmentSP(const SegmentSP *segmentA, const SegmentSP *segmentB);
PGDLLEXPORT Datum equalsSegmentSPV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum getISegmentSP(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getIxSegmentSP(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getIySegmentSP(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getESegmentSP(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getExSegmentSP(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getEySegmentSP(PG_FUNCTION_ARGS);

#endif
