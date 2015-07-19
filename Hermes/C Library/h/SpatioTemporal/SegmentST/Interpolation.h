/*
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the SegmentST data type.
 *
 * @see @ref SegmentST
 * @see @ref data_types_spatio_temp
 */

#ifndef HE_SEGMENTST_INTERPOLATION_H_
#define HE_SEGMENTST_INTERPOLATION_H_

#include "Definition.h"
#include "../PointST/Definition.h"
#include "../BoxST/Definition.h"

PointSP *atInstantSPSegmentST(const SegmentST *segment, Timestamp ts, bool checkContainment, PointSP *result);
PointST *atInstantSegmentST(const SegmentST *segment, Timestamp ts, bool checkContainment, PointST *result);
PGDLLEXPORT Datum atInstantSPSegmentSTV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum atInstantSegmentSTV1(PG_FUNCTION_ARGS);

int32 atPeriodSPSegmentST(const SegmentST *segment, const Period *period, SegmentSP *segmentAt, PointSP *pointAt);
int32 atPeriodSegmentST(const SegmentST *segment, const Period *period, SegmentST *segmentAt, PointST *pointAt);
PGDLLEXPORT Datum atPeriodSPSegmentSTV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum atPeriodSegmentSTV1(PG_FUNCTION_ARGS);

Timestamp *atPointTSegmentST(const SegmentST *segment, const PointSP *point, bool checkContainment, Timestamp *result);
PointST *atPointSegmentST(const SegmentST *segment, const PointSP *point, bool checkContainment, PointST *result);
PGDLLEXPORT Datum atPointTSegmentSTV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum atPointSegmentSTV1(PG_FUNCTION_ARGS);

int32 atBoxTSegmentST(const SegmentST *segment, const BoxSP *box, Period *periodAt, Timestamp *tsAt);
int32 atBoxSegmentST(const SegmentST *segment, const BoxSP *box, SegmentST *segmentAt, PointST *pointAt);
PGDLLEXPORT Datum atBoxTSegmentSTV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum atBoxSegmentSTV1(PG_FUNCTION_ARGS);

int32 atBoxSTSegmentST(const SegmentST *segment, const BoxST *box, SegmentST *segmentAt, PointST *pointAt);
PGDLLEXPORT Datum atBoxSTSegmentSTV1(PG_FUNCTION_ARGS);

PointST *intersectionPointSegmentST_SegmentST(const SegmentST *segmentA, const SegmentST *segmentB, PointST *result);
PGDLLEXPORT Datum intersectionPointSegmentST_SegmentSTV1(PG_FUNCTION_ARGS);

#endif
