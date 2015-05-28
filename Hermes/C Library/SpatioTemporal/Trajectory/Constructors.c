/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/SpatioTemporal/Trajectory/Constructors.h"
#include <utils/array.h>
#include <utils/lsyscache.h>
#include <utils/memutils.h>

PGDLLEXPORT Datum constructorEmptyTrajectory(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(allocateTrajectory(0));
}
PG_FUNCTION_INFO_V1(constructorEmptyTrajectory);

Trajectory *constructorPointTrajectory(const PointST *point) {
	Trajectory *result = allocateTrajectory(1);

	result->points[0] = *point;

	return result;
}

PGDLLEXPORT Datum constructorPointTrajectoryV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(updateMetadataTrajectory(constructorPointTrajectory((PointST *) PG_GETARG_POINTER(0))));
}
PG_FUNCTION_INFO_V1(constructorPointTrajectoryV1);

PGDLLEXPORT Datum constructorPointArrayTrajectory(PG_FUNCTION_ARGS) {
	ArrayType *array = PG_GETARG_ARRAYTYPE_P(0);

	int32 NrPoints;

	ArrayIterator array_iterator;
	Datum element;
	bool isnull;
	int32 i;

	Trajectory *result = NULL;

	NrPoints = ArrayGetNItems(ARR_NDIM(array), ARR_DIMS(array));
	if (NrPoints > 0) {
		result = allocateTrajectory(NrPoints);

		array_iterator = array_create_iterator(array, 0);
		i = 0;
		while (array_iterate(array_iterator, &element, &isnull)) {
			result->points[i] = *(PointST *) DatumGetPointer(element);

			if (i > 0 && result->points[i].t <= result->points[i - 1].t) {
				pfree(result);

				ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("invalid time sequence for Trajectory")));
			}

			i++;
		}
		array_free_iterator(array_iterator);
	} else
		result = allocateTrajectory(0);

	PG_RETURN_POINTER(updateMetadataTrajectory(result));
}
PG_FUNCTION_INFO_V1(constructorPointArrayTrajectory);

MemoryContext TrajCache_MemoryContext = NULL;

int32 TrajCache_NrCaches = 0;

PointST **TrajCache = NULL;
int32 *TrajCache_Length = NULL;
int32 *TrajCache_Index = NULL;

PGDLLEXPORT Datum TrajCache_Allocate(PG_FUNCTION_ARGS) {
	int32 length = PG_GETARG_INT32(0);
	int32 i, cache_id = -1;

	if (length <= 0)
		ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("length cannot be negative or zero")));

	if (TrajCache_MemoryContext == NULL) {
		TrajCache_MemoryContext = AllocSetContextCreate(PostmasterContext, "TrajCache", ALLOCSET_DEFAULT_MINSIZE, ALLOCSET_DEFAULT_INITSIZE, ALLOCSET_DEFAULT_MAXSIZE);

		TrajCache = (PointST **) MemoryContextAlloc(TrajCache_MemoryContext, sizeof(*TrajCache));
		TrajCache_Length = (int32 *) MemoryContextAlloc(TrajCache_MemoryContext, sizeof(*TrajCache_Length));
		TrajCache_Index = (int32 *) MemoryContextAlloc(TrajCache_MemoryContext, sizeof(*TrajCache_Index));

		cache_id = 0;
	} else {
		cache_id = TrajCache_NrCaches;

		for (i = 0; i < TrajCache_NrCaches; i++) {
			if (TrajCache[i] == NULL) {
				cache_id = i;
				break;
			}
		}

		if (cache_id == TrajCache_NrCaches) {
			TrajCache = (PointST **) repalloc(TrajCache, (TrajCache_NrCaches + 1) * sizeof(*TrajCache));
			TrajCache_Length = (int32 *) repalloc(TrajCache_Length, (TrajCache_NrCaches + 1) * sizeof(*TrajCache_Length));
			TrajCache_Index = (int32 *) repalloc(TrajCache_Index, (TrajCache_NrCaches + 1) * sizeof(*TrajCache_Index));
		}
	}

	TrajCache[cache_id] = (PointST *) MemoryContextAlloc(TrajCache_MemoryContext, length * sizeof(**TrajCache));
	TrajCache_Length[cache_id] = length;
	TrajCache_Index[cache_id] = -1;

	TrajCache_NrCaches++;

	PG_RETURN_INT32(cache_id);
}
PG_FUNCTION_INFO_V1(TrajCache_Allocate);

PGDLLEXPORT Datum TrajCache_Free(PG_FUNCTION_ARGS) {
	int32 cache_id = PG_GETARG_INT32(0);

	if (0 <= cache_id && cache_id < TrajCache_NrCaches && TrajCache[cache_id] != NULL) {
		pfree(TrajCache[cache_id]);
		TrajCache[cache_id] = NULL;

		TrajCache_Length[cache_id] = 0;
		TrajCache_Index[cache_id] = -1;
	}

	PG_RETURN_VOID();
}
PG_FUNCTION_INFO_V1(TrajCache_Free);

PGDLLEXPORT Datum TrajCache_Clean(PG_FUNCTION_ARGS) {
	if (TrajCache_MemoryContext != NULL) {
		MemoryContextDelete(TrajCache_MemoryContext);
		TrajCache_MemoryContext = NULL;

		TrajCache_NrCaches = 0;

		TrajCache = NULL;
		TrajCache_Length = NULL;
		TrajCache_Index = NULL;
	}

	PG_RETURN_VOID();
}
PG_FUNCTION_INFO_V1(TrajCache_Clean);

PGDLLEXPORT Datum TrajCache_ResetIndex(PG_FUNCTION_ARGS) {
	int32 cache_id = PG_GETARG_INT32(0);

	if (0 <= cache_id && cache_id < TrajCache_NrCaches)
		TrajCache_Index[cache_id] = -1;

	PG_RETURN_VOID();
}
PG_FUNCTION_INFO_V1(TrajCache_ResetIndex);

PGDLLEXPORT Datum TrajCache_Rewind(PG_FUNCTION_ARGS) {
	int32 cache_id = PG_GETARG_INT32(0);

	if (0 <= cache_id && cache_id < TrajCache_NrCaches && TrajCache_Index[cache_id] > -1)
		TrajCache_Index[cache_id]--;

	PG_RETURN_VOID();
}
PG_FUNCTION_INFO_V1(TrajCache_Rewind);

PGDLLEXPORT Datum TrajCache_Append(PG_FUNCTION_ARGS) {
	int32 cache_id = PG_GETARG_INT32(0);
	PointST *point = (PointST *) PG_GETARG_POINTER(1);
	
	if (0 <= cache_id && cache_id < TrajCache_NrCaches && TrajCache[cache_id] != NULL) {
		if (TrajCache_Index[cache_id] == TrajCache_Length[cache_id] - 1)
			ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("TrajCache: array overflow")));

		if (TrajCache_Index[cache_id] >= 0 && point->t <= TrajCache[cache_id][TrajCache_Index[cache_id]].t)
			ereport(ERROR, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("invalid time sequence for Trajectory")));

		TrajCache_Index[cache_id] = TrajCache_Index[cache_id] + 1;
		TrajCache[cache_id][TrajCache_Index[cache_id]] = *point;
	}

	PG_RETURN_VOID();
}
PG_FUNCTION_INFO_V1(TrajCache_Append);

PGDLLEXPORT Datum TrajCache_getIndex(PG_FUNCTION_ARGS) {
	int32 cache_id = PG_GETARG_INT32(0);

	if (0 <= cache_id && cache_id < TrajCache_NrCaches)
		PG_RETURN_INT32(TrajCache_Index[cache_id] + 1);

	PG_RETURN_NULL();
}
PG_FUNCTION_INFO_V1(TrajCache_getIndex);

PGDLLEXPORT Datum TrajCache_Point(PG_FUNCTION_ARGS) {
	int32 cache_id = PG_GETARG_INT32(0);
	int32 i = PG_GETARG_INT32(1);

	if (0 <= cache_id && cache_id < TrajCache_NrCaches && 0 < i && i <= TrajCache_Index[cache_id] + 1 && TrajCache[cache_id] != NULL)
		PG_RETURN_POINTER(&TrajCache[cache_id][i - 1]);

	PG_RETURN_NULL();
}
PG_FUNCTION_INFO_V1(TrajCache_Point);

PGDLLEXPORT Datum TrajCache2Trajectory(PG_FUNCTION_ARGS) {
	int32 cache_id = PG_GETARG_INT32(0);

	int32 NrPoints;
	Trajectory *result = NULL;

	if (0 <= cache_id && cache_id < TrajCache_NrCaches && TrajCache[cache_id] != NULL) {
		NrPoints = TrajCache_Index[cache_id] + 1;

		if (NrPoints > 0) {
			result = allocateTrajectory(NrPoints);

			memcpy(result->points, TrajCache[cache_id], NrPoints * sizeof(PointST));
			
			PG_RETURN_POINTER(updateMetadataTrajectory(result));
		}
	}

	PG_RETURN_NULL();
}
PG_FUNCTION_INFO_V1(TrajCache2Trajectory);
