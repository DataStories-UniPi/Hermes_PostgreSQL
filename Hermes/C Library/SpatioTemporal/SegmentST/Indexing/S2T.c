/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../../h/SpatioTemporal/SegmentST/Indexing/S2T.h"
#include "../../../h/Utilities/Utilities.h"
#include <funcapi.h>
#include <access/htup_details.h>
#include <utils/builtins.h>

float8 S2T_TemporalBufferSize = 0.0;

PGDLLEXPORT Datum getS2T_TemporalBufferSize(PG_FUNCTION_ARGS) {
	PG_RETURN_INTERVAL_P(seconds2interval(S2T_TemporalBufferSize, NULL));
}
PG_FUNCTION_INFO_V1(getS2T_TemporalBufferSize);

PGDLLEXPORT Datum setS2T_TemporalBufferSize(PG_FUNCTION_ARGS) {
	S2T_TemporalBufferSize = interval2seconds(PG_GETARG_INTERVAL_P(0));

	PG_RETURN_VOID();
}
PG_FUNCTION_INFO_V1(setS2T_TemporalBufferSize);

int32 S2T_SpatialBufferSize = 0;

PGDLLEXPORT Datum getS2T_SpatialBufferSize(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(S2T_SpatialBufferSize);
}
PG_FUNCTION_INFO_V1(getS2T_SpatialBufferSize);

PGDLLEXPORT Datum setS2T_SpatialBufferSize(PG_FUNCTION_ARGS) {
	int32 input = PG_GETARG_INT32(0);

	if (input < 0)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("S2T_SpatialBufferSize cannot be negative")));

	S2T_SpatialBufferSize = input;

	PG_RETURN_VOID();
}
PG_FUNCTION_INFO_V1(setS2T_SpatialBufferSize);

int32 S2T_SimplificationMethod = 0;

float8 S2T_SimplificationSamplingPeriod = 0.0;
float8 S2T_SimplificationEpsDP = 0.0;

PGDLLEXPORT Datum getS2T_SimplificationMethod(PG_FUNCTION_ARGS) {
	if (S2T_SimplificationMethod == 1)
		PG_RETURN_TEXT_P(cstring_to_text("T"));
	else if (S2T_SimplificationMethod == 2)
		PG_RETURN_TEXT_P(cstring_to_text("DP"));
	else if (S2T_SimplificationMethod == 3)
		PG_RETURN_TEXT_P(cstring_to_text("DP+"));
	else
		PG_RETURN_NULL();
}
PG_FUNCTION_INFO_V1(getS2T_SimplificationMethod);

PGDLLEXPORT Datum setS2T_SimplificationMethodT(PG_FUNCTION_ARGS) {
	S2T_SimplificationMethod = 1;
	S2T_SimplificationSamplingPeriod = interval2seconds(PG_GETARG_INTERVAL_P(0));

	PG_RETURN_VOID();
}
PG_FUNCTION_INFO_V1(setS2T_SimplificationMethodT);

PGDLLEXPORT Datum setS2T_SimplificationMethodDP(PG_FUNCTION_ARGS) {
	S2T_SimplificationMethod = 2;
	S2T_SimplificationEpsDP = PG_GETARG_FLOAT8(0);

	PG_RETURN_VOID();
}
PG_FUNCTION_INFO_V1(setS2T_SimplificationMethodDP);

PGDLLEXPORT Datum setS2T_SimplificationMethodDPP(PG_FUNCTION_ARGS) {
	S2T_SimplificationMethod = 3;
	S2T_SimplificationEpsDP = PG_GETARG_FLOAT8(0);

	PG_RETURN_VOID();
}
PG_FUNCTION_INFO_V1(setS2T_SimplificationMethodDPP);

PGDLLEXPORT Datum setNoneS2T_SimplificationMethod(PG_FUNCTION_ARGS) {
	S2T_SimplificationMethod = 0;

	PG_RETURN_VOID();
}
PG_FUNCTION_INFO_V1(setNoneS2T_SimplificationMethod);

int32 S2T_VotingMethod = 0;

PGDLLEXPORT Datum getS2T_VotingMethod(PG_FUNCTION_ARGS) {
	if (S2T_VotingMethod == 1)
		PG_RETURN_TEXT_P(cstring_to_text("Count"));
	else if (S2T_VotingMethod == 2)
		PG_RETURN_TEXT_P(cstring_to_text("Trapezoidal"));
	else
		PG_RETURN_NULL();
}
PG_FUNCTION_INFO_V1(getS2T_VotingMethod);

PGDLLEXPORT Datum setS2T_VotingMethod(PG_FUNCTION_ARGS) {
	char *input = text_to_cstring(PG_GETARG_TEXT_P(0));

	if (strcmp(input, "Count") == 0)
		S2T_VotingMethod = 1;
	else if (strcmp(input, "Trapezoidal") == 0)
		S2T_VotingMethod = 2;
	else
		S2T_VotingMethod = 0;

	pfree(input);

	PG_RETURN_VOID();
}
PG_FUNCTION_INFO_V1(setS2T_VotingMethod);

PGDLLEXPORT Datum setNoneS2T_VotingMethod(PG_FUNCTION_ARGS) {
	S2T_VotingMethod = 0;

	PG_RETURN_VOID();
}
PG_FUNCTION_INFO_V1(setNoneS2T_VotingMethod);

float8 S2T_Sigma = 1.0;
float8 S2T_sqSigma_times_two = 2.0;

PGDLLEXPORT Datum getS2T_Sigma(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(S2T_Sigma);
}
PG_FUNCTION_INFO_V1(getS2T_Sigma);

PGDLLEXPORT Datum setS2T_Sigma(PG_FUNCTION_ARGS) {
	float8 input = PG_GETARG_FLOAT8(0);

	if (input <= 0.0)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("S2T_Sigma cannot be negative or zero")));

	S2T_Sigma = input;
	S2T_sqSigma_times_two = 2.0 * pow(S2T_Sigma, 2.0);

	PG_RETURN_VOID();
}
PG_FUNCTION_INFO_V1(setS2T_Sigma);

MemoryContext S2T_MemoryContext = NULL;

int32 S2T_NrSegments = 0;
float8 *S2T_Voting = NULL;
float8 *S2T_Lifespan = NULL;

PGDLLEXPORT Datum getS2T_VotingSignal(PG_FUNCTION_ARGS) {
	FuncCallContext *funcctx;
	Datum result;

	bool nulls[2] = { false, false };
	Datum values[2];
	HeapTuple tuple;

	Interval interval;

	if (SRF_IS_FIRSTCALL()) {
		MemoryContext oldcontext;

		int32 i;
		float8 maxVoting;
		float8 sumLifespan;

		funcctx = SRF_FIRSTCALL_INIT();
		oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

		if (get_call_result_type(fcinfo, NULL, &funcctx->tuple_desc) != TYPEFUNC_COMPOSITE)
			ereport(ERROR, (errcode(ERRCODE_FEATURE_NOT_SUPPORTED), errmsg("function returning record called in context that cannot accept type record")));
		BlessTupleDesc(funcctx->tuple_desc);

		if (S2T_Voting != NULL && S2T_Lifespan != NULL) {
			funcctx->max_calls = S2T_NrSegments;

			if (S2T_NrSegments > 0) {
				maxVoting = S2T_Voting[0];
				sumLifespan = S2T_Lifespan[0];
				for (i = 1; i < S2T_NrSegments; i++) {
					if (S2T_Voting[i] > maxVoting)
						maxVoting = S2T_Voting[i];

					sumLifespan += S2T_Lifespan[i];
				}

				funcctx->user_fctx = palloc(sizeof(maxVoting) + sizeof(sumLifespan));
				((float8 *) funcctx->user_fctx)[0] = maxVoting == 0.0 ? 1.0 : maxVoting;
				((float8 *) funcctx->user_fctx)[1] = sumLifespan;
			}
		} else
			funcctx->max_calls = 0;

		MemoryContextSwitchTo(oldcontext);
	}

	funcctx = SRF_PERCALL_SETUP();

	if (funcctx->call_cntr < funcctx->max_calls) {
		values[0] = Float8GetDatum(S2T_Voting[funcctx->call_cntr] / ((float8 *) funcctx->user_fctx)[0]);
		values[1] = IntervalPGetDatum(seconds2interval(S2T_Lifespan[funcctx->call_cntr] / ((float8 *) funcctx->user_fctx)[1], &interval));

		tuple = heap_form_tuple(funcctx->tuple_desc, values, nulls);
		result = HeapTupleGetDatum(tuple);

		SRF_RETURN_NEXT(funcctx, result);
	} else {
		SRF_RETURN_DONE(funcctx);
	}
}
PG_FUNCTION_INFO_V1(getS2T_VotingSignal);
