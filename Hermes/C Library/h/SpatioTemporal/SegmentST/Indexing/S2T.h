/*
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the SegmentST data type.
 *
 * @see @ref SegmentST
 * @see @ref data_types_spatio_temp
 */

#ifndef HE_SEGMENTST_S2T_H_
#define HE_SEGMENTST_S2T_H_

#include <postgres.h>
#include <fmgr.h>

extern float8 S2T_TemporalBufferSize;
PGDLLEXPORT Datum getS2T_TemporalBufferSize(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum setS2T_TemporalBufferSize(PG_FUNCTION_ARGS);

extern int32 S2T_SpatialBufferSize;
PGDLLEXPORT Datum getS2T_SpatialBufferSize(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum setS2T_SpatialBufferSize(PG_FUNCTION_ARGS);

extern int32 S2T_SimplificationMethod;

extern float8 S2T_SimplificationSamplingPeriod;
extern float8 S2T_SimplificationEpsDP;

PGDLLEXPORT Datum getS2T_SimplificationMethod(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum setS2T_SimplificationMethodT(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum setS2T_SimplificationMethodDP(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum setS2T_SimplificationMethodDPP(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum setNoneS2T_SimplificationMethod(PG_FUNCTION_ARGS);

extern int32 S2T_VotingMethod;
PGDLLEXPORT Datum getS2T_VotingMethod(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum setS2T_VotingMethod(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum setNoneS2T_VotingMethod(PG_FUNCTION_ARGS);

extern float8 S2T_Sigma;
extern float8 S2T_sqSigma_times_two;
PGDLLEXPORT Datum getS2T_Sigma(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum setS2T_Sigma(PG_FUNCTION_ARGS);

extern MemoryContext S2T_MemoryContext;

extern int32 S2T_NrSegments;
extern float8 *S2T_Voting;
extern float8 *S2T_Lifespan;

PGDLLEXPORT Datum getS2T_VotingSignal(PG_FUNCTION_ARGS);

#endif
