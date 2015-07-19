/**
 *	@file
 *	@author Marios Vodas (mvodas@gmail.com).
 *	@brief The implementation of SegmentST
 *
 */

#include "../../../h/SpatioTemporal/SegmentST/Indexing/S2T.h"
#include "../../../h/SpatioTemporal/SegmentST/Casts.h"
#include "../../../h/SpatioTemporal/SegmentST/RelationalOperators.h"
#include "../../../h/SpatioTemporal/SegmentST/Interpolation.h"
#include "../../../h/SpatioTemporal/SegmentST/Similarity.h"
#include "../../../h/SpatioTemporal/BoxST/Interactions.h"
#include "../../../h/SpatioTemporal/BoxST/RelationalOperators.h"
#include "../../../h/SpatioTemporal/BoxST/Editors.h"
#include "../../../h/SpatioTemporal/RangeST/Casts.h"
#include "../../../h/SpatioTemporal/Trajectory/Simplification.h"
#include "../../../h/SpatioTemporal/Trajectory/SessionParameters.h"
#include "../../../h/Temporal/Period/Properties.h"
#include "../../../h/Temporal/Period/RelationalOperators.h"
#include "../../../h/Temporal/Period/Distance.h"
#include "../../../h/Spatial/SegmentSP/Casts.h"
#include "../../../h/Spatial/SegmentSP/Distance.h"
#include "../../../h/Spatial/BoxSP/Properties.h"
#include "../../../h/Spatial/BoxSP/RelationalOperators.h"
#include "../../../h/Spatial/BoxSP/Distance.h"
#include "../../../h/Spatial/SegmentSP/RelationalOperators.h"
#include "../../../h/Spatial/RangeSP/Casts.h"
#include "../../../h/Utilities/Utilities.h"
#include <access/gist.h>
#include <access/skey.h>
#include <parser/parse_type.h>
#include <utils/syscache.h>
#include <utils/memutils.h>

PGDLLEXPORT Datum consistentGiSTSegmentST(PG_FUNCTION_ARGS) {
	GISTENTRY *entry = (GISTENTRY *) PG_GETARG_POINTER(0);
	StrategyNumber strategy = (StrategyNumber) PG_GETARG_UINT16(2);
	Oid subtype = PG_GETARG_OID(3);
	bool *recheck = (bool *) PG_GETARG_POINTER(4);
	bool retval;
	Type typ;
	char *typname = NULL;

	BoxST tmpBoxST, tmpBoxST_2;
	BoxSP tmpBoxSP;
	SegmentST tmpSegmentST;
	Trajectory *tmpTrajectory = NULL;

	int32 i, n;
	float8 f;

	*recheck = false;

	if (PG_ARGISNULL(1))
		PG_RETURN_BOOL(false);

	retval = false;

	typ = typeidType(subtype);
	typname = typeTypeName(typ);
	ReleaseSysCache(typ);

	if (strategy == 1) {
		if (strcmp(typname, "boxst") == 0) {
			if (GIST_LEAF(entry))
				retval = intersectsSegmentST_BoxST((SegmentST *) DatumGetPointer(entry->key), (BoxST *) PG_GETARG_POINTER(1));
			else
				retval = intersectsBoxST_BoxST((BoxST *) DatumGetPointer(entry->key), (BoxST *) PG_GETARG_POINTER(1));
		}
	} else if (strategy == 2) {
		if (strcmp(typname, "segmentst") == 0) {
			if (GIST_LEAF(entry))
				retval = intersectsSegmentST_SegmentST((SegmentST *) DatumGetPointer(entry->key), (SegmentST *) PG_GETARG_POINTER(1));
			else
				retval = intersectsSegmentST_BoxST((SegmentST *) PG_GETARG_POINTER(1), (BoxST *) DatumGetPointer(entry->key));
		}
	} else if (strategy == 3) {
		if (strcmp(typname, "pointst") == 0) {
			if (GIST_LEAF(entry))
				retval = containsSegmentST_PointST((SegmentST *) DatumGetPointer(entry->key), (PointST *) PG_GETARG_POINTER(1));
			else
				retval = containsBoxST_PointST((BoxST *) DatumGetPointer(entry->key), (PointST *) PG_GETARG_POINTER(1));
		}
	} else if (strategy == 4) {
		if (strcmp(typname, "boxst") == 0) {
			if (GIST_LEAF(entry))
				retval = containsBoxST_BoxST((BoxST *) PG_GETARG_POINTER(1), SegmentST2BoxST((SegmentST *) DatumGetPointer(entry->key), &tmpBoxST));
			else
				retval = intersectsBoxST_BoxST((BoxST *) DatumGetPointer(entry->key), (BoxST *) PG_GETARG_POINTER(1));
		}
	} else if (strategy == 5) {
		if (strcmp(typname, "boxst") == 0) {
			if (GIST_LEAF(entry))
				retval = containsProperlyBoxST_BoxST((BoxST *) PG_GETARG_POINTER(1), SegmentST2BoxST((SegmentST *) DatumGetPointer(entry->key), &tmpBoxST));
			else
				retval = intersectsBoxST_BoxST((BoxST *) DatumGetPointer(entry->key), (BoxST *) PG_GETARG_POINTER(1));
		}
	} else if (strategy == 6) {
		if (strcmp(typname, "rangest") == 0) {
			if (GIST_LEAF(entry))
				retval = withinDistanceSegmentST_RangeST((SegmentST *) DatumGetPointer(entry->key), (RangeST *) PG_GETARG_POINTER(1));
			else
				retval = intersectsBoxST_BoxST((BoxST *) DatumGetPointer(entry->key), RangeST2BoxST((RangeST *) PG_GETARG_POINTER(1), &tmpBoxST));
		}
	} else if (strategy == 7) {
		if (strcmp(typname, "period") == 0) {
			if (GIST_LEAF(entry))
				retval = intersectsPeriod_Period(&((SegmentST *) DatumGetPointer(entry->key))->t, (Period *) PG_GETARG_POINTER(1));
			else
				retval = intersectsPeriod_Period(&((BoxST *) DatumGetPointer(entry->key))->t, (Period *) PG_GETARG_POINTER(1));
		}
	} else if (strategy == 8) {
		if (strcmp(typname, "boxsp") == 0) {
			if (GIST_LEAF(entry))
				retval = intersectsSegmentSP_BoxSP(&((SegmentST *) DatumGetPointer(entry->key))->sp, (BoxSP *) PG_GETARG_POINTER(1));
			else
				retval = intersectsBoxSP_BoxSP(&((BoxST *) DatumGetPointer(entry->key))->sp, (BoxSP *) PG_GETARG_POINTER(1));
		}
	} else if (strategy == 9) {
		if (strcmp(typname, "segmentsp") == 0) {
			if (GIST_LEAF(entry))
				retval = intersectsSegmentSP_SegmentSP(&((SegmentST *) DatumGetPointer(entry->key))->sp, (SegmentSP *) PG_GETARG_POINTER(1));
			else
				retval = intersectsSegmentSP_BoxSP((SegmentSP *) PG_GETARG_POINTER(1), &((BoxST *) DatumGetPointer(entry->key))->sp);
		}
	} else if (strategy == 10) {
		if (strcmp(typname, "timestamp") == 0) {
			if (GIST_LEAF(entry))
				retval = containsPeriod_Timestamp(&((SegmentST *) DatumGetPointer(entry->key))->t, PG_GETARG_TIMESTAMP(1));
			else
				retval = containsPeriod_Timestamp(&((BoxST *) DatumGetPointer(entry->key))->t, PG_GETARG_TIMESTAMP(1));
		}
	} else if (strategy == 11) {
		if (strcmp(typname, "period") == 0) {
			if (GIST_LEAF(entry))
				retval = containsPeriod_Period(&((SegmentST *) DatumGetPointer(entry->key))->t, (Period *) PG_GETARG_POINTER(1));
			else
				retval = containsPeriod_Period(&((BoxST *) DatumGetPointer(entry->key))->t, (Period *) PG_GETARG_POINTER(1));
		}
	} else if (strategy == 12) {
		if (strcmp(typname, "pointsp") == 0) {
			if (GIST_LEAF(entry))
				retval = containsSegmentSP_PointSP(&((SegmentST *) DatumGetPointer(entry->key))->sp, (PointSP *) PG_GETARG_POINTER(1));
			else
				retval = containsBoxSP_PointSP(&((BoxST *) DatumGetPointer(entry->key))->sp, (PointSP *) PG_GETARG_POINTER(1));
		}
	} else if (strategy == 13) {
		if (strcmp(typname, "period") == 0) {
			if (GIST_LEAF(entry))
				retval = containsPeriod_Period((Period *) PG_GETARG_POINTER(1), &((SegmentST *) DatumGetPointer(entry->key))->t);
			else
				retval = intersectsPeriod_Period(&((BoxST *) DatumGetPointer(entry->key))->t, (Period *) PG_GETARG_POINTER(1));
		}
	} else if (strategy == 14) {
		if (strcmp(typname, "boxsp") == 0) {
			if (GIST_LEAF(entry))
				retval = containsBoxSP_BoxSP((BoxSP *) PG_GETARG_POINTER(1), SegmentSP2BoxSP(&((SegmentST *) DatumGetPointer(entry->key))->sp, &tmpBoxSP));
			else
				retval = intersectsBoxSP_BoxSP(&((BoxST *) DatumGetPointer(entry->key))->sp, (BoxSP *) PG_GETARG_POINTER(1));
		}
	} else if (strategy == 15) {
		if (strcmp(typname, "rangesp") == 0) {
			if (GIST_LEAF(entry))
				retval = withinDistanceSegmentSP_RangeSP(&((SegmentST *) DatumGetPointer(entry->key))->sp, (RangeSP *) PG_GETARG_POINTER(1));
			else
				retval = intersectsBoxSP_BoxSP(&((BoxST *) DatumGetPointer(entry->key))->sp, RangeSP2BoxSP((RangeSP *) PG_GETARG_POINTER(1), &tmpBoxSP));
		}
	} else if (strategy == 21) {
		if (strcmp(typname, "trajectory") == 0) {
			if (fcinfo->flinfo->fn_extra == NULL) {
				if (S2T_MemoryContext == NULL)
					S2T_MemoryContext = AllocSetContextCreate(PostmasterContext, "S2T", ALLOCSET_DEFAULT_MINSIZE, ALLOCSET_DEFAULT_INITSIZE, ALLOCSET_DEFAULT_MAXSIZE);
				else
					MemoryContextReset(S2T_MemoryContext);

				S2T_Voting = NULL;
				S2T_Lifespan = NULL;

				tmpTrajectory = (Trajectory *) PG_GETARG_VARLENA_P(1);

				S2T_NrSegments = NrPoints2NrSegments(tmpTrajectory->NrPoints);

				if (S2T_NrSegments < 1)
					PG_RETURN_BOOL(false);

				if (S2T_VotingMethod != 0) {
					S2T_Voting = (float8 *) MemoryContextAllocZero(S2T_MemoryContext, S2T_NrSegments * sizeof(*S2T_Voting));
					S2T_Lifespan = (float8 *) MemoryContextAlloc(S2T_MemoryContext, S2T_NrSegments * sizeof(*S2T_Lifespan));
				}

				fcinfo->flinfo->fn_extra = MemoryContextAlloc(fcinfo->flinfo->fn_mcxt, 6 * sizeof(void *));

				if (S2T_VotingMethod == 2)
					((void **) fcinfo->flinfo->fn_extra)[0] = MemoryContextAlloc(fcinfo->flinfo->fn_mcxt, S2T_NrSegments * sizeof(SegmentST));

				((void **) fcinfo->flinfo->fn_extra)[1] = MemoryContextAlloc(fcinfo->flinfo->fn_mcxt, sizeof(BoxST));
				((void **) fcinfo->flinfo->fn_extra)[2] = MemoryContextAlloc(fcinfo->flinfo->fn_mcxt, S2T_NrSegments * sizeof(BoxST));

				tmpBoxST_2.t.i = tmpTrajectory->points[0].t;
				tmpBoxST_2.t.e = tmpTrajectory->points[0].t;
				tmpBoxST_2.sp.l = tmpTrajectory->points[0].sp;
				tmpBoxST_2.sp.h = tmpTrajectory->points[0].sp;

				for (i = 1; i <= S2T_NrSegments; i++) {
					tmpSegmentST.t.i = tmpTrajectory->points[i - 1].t;
					tmpSegmentST.sp.i = tmpTrajectory->points[i - 1].sp;
					tmpSegmentST.t.e = tmpTrajectory->points[i].t;
					tmpSegmentST.sp.e = tmpTrajectory->points[i].sp;

					if (S2T_VotingMethod != 0)
						S2T_Lifespan[i - 1] = differenceInSeconds(tmpTrajectory->points[i].t, tmpTrajectory->points[i - 1].t);

					if (S2T_VotingMethod == 2)
						((SegmentST *) (((void **) fcinfo->flinfo->fn_extra)[0]))[i - 1] = tmpSegmentST;

					SegmentST2BoxST(&tmpSegmentST, &tmpBoxST);
					bufferBoxST(&tmpBoxST, S2T_VotingMethod == 2 ? 0.0 : S2T_TemporalBufferSize, S2T_SpatialBufferSize, S2T_SpatialBufferSize, &tmpBoxST);
					((BoxST *) ((void **) fcinfo->flinfo->fn_extra)[2])[i - 1] = tmpBoxST;

					unionBoxST_BoxST(&tmpBoxST_2, &tmpBoxST, &tmpBoxST_2);
				}

				*((BoxST *) ((void **) fcinfo->flinfo->fn_extra)[1]) = tmpBoxST_2;

				((void **) fcinfo->flinfo->fn_extra)[3] = MemoryContextAlloc(fcinfo->flinfo->fn_mcxt, sizeof(int32));

				if (S2T_SimplificationMethod != 0) {
					if (S2T_SimplificationMethod == 1) {
						f = differenceInSeconds(tmpTrajectory->points[tmpTrajectory->NrPoints - 1].t, tmpTrajectory->points[0].t);
						i = (int32) ceil(f / S2T_SimplificationSamplingPeriod);

						if (i < S2T_NrSegments)
							tmpTrajectory = simplifyTrajectory(tmpTrajectory, S2T_SimplificationSamplingPeriod, false, interpolationPrecision);
					} else if (S2T_SimplificationMethod == 2)
						tmpTrajectory = simplifyDPTrajectory(tmpTrajectory, S2T_SimplificationEpsDP, false);
					else if (S2T_SimplificationMethod == 3)
						tmpTrajectory = simplifyDPTrajectory(tmpTrajectory, S2T_SimplificationEpsDP, true);

					n = NrPoints2NrSegments(tmpTrajectory->NrPoints);
					*((int32 *) ((void **) fcinfo->flinfo->fn_extra)[3]) = n;

					((void **) fcinfo->flinfo->fn_extra)[4] = MemoryContextAlloc(fcinfo->flinfo->fn_mcxt, sizeof(BoxST));
					((void **) fcinfo->flinfo->fn_extra)[5] = MemoryContextAlloc(fcinfo->flinfo->fn_mcxt, n * sizeof(BoxST));

					tmpBoxST_2.t.i = tmpTrajectory->points[0].t;
					tmpBoxST_2.t.e = tmpTrajectory->points[0].t;
					tmpBoxST_2.sp.l = tmpTrajectory->points[0].sp;
					tmpBoxST_2.sp.h = tmpTrajectory->points[0].sp;

					for (i = 1; i <= n; i++) {
						tmpSegmentST.t.i = tmpTrajectory->points[i - 1].t;
						tmpSegmentST.sp.i = tmpTrajectory->points[i - 1].sp;
						tmpSegmentST.t.e = tmpTrajectory->points[i].t;
						tmpSegmentST.sp.e = tmpTrajectory->points[i].sp;

						SegmentST2BoxST(&tmpSegmentST, &tmpBoxST);
						bufferBoxST(&tmpBoxST, S2T_VotingMethod == 2 ? 0.0 : S2T_TemporalBufferSize, S2T_SpatialBufferSize, S2T_SpatialBufferSize, &tmpBoxST);
						((BoxST *) ((void **) fcinfo->flinfo->fn_extra)[5])[i - 1] = tmpBoxST;

						unionBoxST_BoxST(&tmpBoxST_2, &tmpBoxST, &tmpBoxST_2);
					}

					*((BoxST *) ((void **) fcinfo->flinfo->fn_extra)[4]) = tmpBoxST_2;
				} else {
					*((int32 *) ((void **) fcinfo->flinfo->fn_extra)[3]) = S2T_NrSegments;

					((void **) fcinfo->flinfo->fn_extra)[4] = ((void **) fcinfo->flinfo->fn_extra)[1];
					((void **) fcinfo->flinfo->fn_extra)[5] = ((void **) fcinfo->flinfo->fn_extra)[2];
				}
			}

			if (GIST_LEAF(entry)) {
				SegmentST2BoxST((SegmentST *) DatumGetPointer(entry->key), &tmpBoxST);

				//if (intersectsSegmentST_BoxST((SegmentST *) DatumGetPointer(entry->key), (BoxST *) ((void **) fcinfo->flinfo->fn_extra)[1])) {
				if (intersectsBoxST_BoxST(&tmpBoxST, (BoxST *) ((void **) fcinfo->flinfo->fn_extra)[1])) {
					for (i = 1; i <= S2T_NrSegments; i++) {
						//if (intersectsSegmentST_BoxST((SegmentST *) DatumGetPointer(entry->key), &((BoxST *) ((void **) fcinfo->flinfo->fn_extra)[2])[i - 1])) {
						if (intersectsBoxST_BoxST(&tmpBoxST, &((BoxST *) ((void **) fcinfo->flinfo->fn_extra)[2])[i - 1])) {
							if (S2T_VotingMethod == 0) {
								retval = true;

								break;
							} else {
								if (S2T_VotingMethod == 1)
									S2T_Voting[i - 1]++;
								else if (S2T_VotingMethod == 2) {
									if (TrapezoidalSegmentST((SegmentST *) DatumGetPointer(entry->key), &((SegmentST *) ((void **) fcinfo->flinfo->fn_extra)[0])[i - 1], &f) == NULL)
										continue;

									S2T_Voting[i - 1] += exp(-(pow(f, 2.0) / S2T_sqSigma_times_two));
								}
							}
						}
					}
				}
			} else {
				if (intersectsBoxST_BoxST((BoxST *) DatumGetPointer(entry->key), (BoxST *) ((void **) fcinfo->flinfo->fn_extra)[4])) {
					n = *((int32 *) ((void **) fcinfo->flinfo->fn_extra)[3]);
					for (i = 1; i <= n; i++) {
						if (intersectsBoxST_BoxST((BoxST *) DatumGetPointer(entry->key), &((BoxST *) ((void **) fcinfo->flinfo->fn_extra)[5])[i - 1])) {
							retval = true;

							break;
						}
					}
				}

				//retval = intersectsTrajectory_BoxST(tmpTrajectory, (BoxST *) DatumGetPointer(entry->key)); //Takes too long!
			}
		}
	} else if (strategy == 22) {
		if (strcmp(typname, "boxst") == 0) {
			if (GIST_LEAF(entry))
				retval = intersectsBoxST_BoxST(SegmentST2BoxST((SegmentST *) DatumGetPointer(entry->key), &tmpBoxST), (BoxST *) PG_GETARG_POINTER(1));
			else
				retval = intersectsBoxST_BoxST((BoxST *) DatumGetPointer(entry->key), (BoxST *) PG_GETARG_POINTER(1));
		}
	}

	PG_RETURN_BOOL(retval);
}
PG_FUNCTION_INFO_V1(consistentGiSTSegmentST);

PGDLLEXPORT Datum distanceGiSTSegmentST(PG_FUNCTION_ARGS) {
	GISTENTRY *entry = (GISTENTRY *) PG_GETARG_POINTER(0);
	StrategyNumber strategy = (StrategyNumber) PG_GETARG_UINT16(2);
	Oid subtype = PG_GETARG_OID(3);
	float8 retval;
	Type typ;
	char *typname = NULL;

	if (PG_ARGISNULL(1))
		PG_RETURN_FLOAT8(DBL_MAX);

	retval = DBL_MAX;

	typ = typeidType(subtype);
	typname = typeTypeName(typ);
	ReleaseSysCache(typ);

	if (strategy == 16) {
		if (strcmp(typname, "timestamp") == 0) {
			if (GIST_LEAF(entry))
				retval = distancePeriod_Timestamp(&((SegmentST *) DatumGetPointer(entry->key) )->t, PG_GETARG_TIMESTAMP(1));
			else
				retval = distancePeriod_Timestamp(&((BoxST *) DatumGetPointer(entry->key) )->t, PG_GETARG_TIMESTAMP(1));
		}
	} else if (strategy == 17) {
		if (strcmp(typname, "period") == 0) {
			if (GIST_LEAF(entry))
				retval = distancePeriod_Period(&((SegmentST *) DatumGetPointer(entry->key) )->t, (Period *) PG_GETARG_POINTER(1));
			else
				retval = distancePeriod_Period(&((BoxST *) DatumGetPointer(entry->key) )->t, (Period *) PG_GETARG_POINTER(1));
		}
	} else if (strategy == 18) {
		if (strcmp(typname, "pointsp") == 0) {
			if (GIST_LEAF(entry))
				retval = distanceSegmentSP_PointSP(&((SegmentST *) DatumGetPointer(entry->key) )->sp, (PointSP *) PG_GETARG_POINTER(1));
			else
				retval = distanceBoxSP_PointSP(&((BoxST *) DatumGetPointer(entry->key) )->sp, (PointSP *) PG_GETARG_POINTER(1));
		}
	} else if (strategy == 19) {
		if (strcmp(typname, "segmentsp") == 0) {
			if (GIST_LEAF(entry))
				retval = distanceSegmentSP_SegmentSP(&((SegmentST *) DatumGetPointer(entry->key) )->sp, (SegmentSP *) PG_GETARG_POINTER(1));
			else
				retval = distanceBoxSP_SegmentSP(&((BoxST *) DatumGetPointer(entry->key) )->sp, (SegmentSP *) PG_GETARG_POINTER(1));
		}
	} else if (strategy == 20) {
		if (strcmp(typname, "boxsp") == 0) {
			if (GIST_LEAF(entry))
				retval = distanceBoxSP_SegmentSP((BoxSP *) PG_GETARG_POINTER(1), &((SegmentST *) DatumGetPointer(entry->key) )->sp);
			else
				retval = distanceBoxSP_BoxSP(&((BoxST *) DatumGetPointer(entry->key) )->sp, (BoxSP *) PG_GETARG_POINTER(1));
		}
	}

	PG_RETURN_FLOAT8(retval);
}
PG_FUNCTION_INFO_V1(distanceGiSTSegmentST);

PGDLLEXPORT Datum compressGiSTSegmentST(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(PG_GETARG_POINTER(0));
}
PG_FUNCTION_INFO_V1(compressGiSTSegmentST);

PGDLLEXPORT Datum decompressGiSTSegmentST(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(PG_GETARG_POINTER(0));
}
PG_FUNCTION_INFO_V1(decompressGiSTSegmentST);

PGDLLEXPORT Datum unionGiSTSegmentST(PG_FUNCTION_ARGS) {
	GistEntryVector *entryvec = (GistEntryVector *) PG_GETARG_POINTER(0);
	GISTENTRY *ent = entryvec->vector;

	int i;
	SegmentST *curSegment = NULL;

	BoxST *pageunion = NULL;
	int *sizep = (int *) PG_GETARG_POINTER(1);

	curSegment = (SegmentST *) DatumGetPointer(ent[0].key);
	pageunion = SegmentST2BoxST(curSegment, NULL);

	for (i = 1; i < entryvec->n; i++) {
		curSegment = (SegmentST *) DatumGetPointer(ent[i].key);
		unionBoxST_SegmentST(pageunion, curSegment, pageunion);
	}

	*sizep = sizeof(*pageunion);

	PG_RETURN_POINTER(pageunion);
}
PG_FUNCTION_INFO_V1(unionGiSTSegmentST);

PGDLLEXPORT Datum penaltyGiSTSegmentST(PG_FUNCTION_ARGS) {
	GISTENTRY *origentry = (GISTENTRY *) PG_GETARG_POINTER(0);
	GISTENTRY *newentry = (GISTENTRY *) PG_GETARG_POINTER(1);
	float *penalty = (float *) PG_GETARG_POINTER(2);

	BoxST *box_orig = NULL, box_new, box_union;
	float8 size_union, size_orig;

	box_orig = (BoxST *) DatumGetPointer(origentry->key);
	SegmentST2BoxST((SegmentST *) DatumGetPointer(newentry->key), &box_new);

	if (box_orig->sp.l.x > box_orig->sp.h.x || box_orig->sp.l.y > box_orig->sp.h.y)
		ereport(INFO, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("penalty: origentry not box, %s", outBoxST(box_orig))));

	size_orig = durationPeriod(&box_orig->t) * areaBoxSP(&box_orig->sp);

	unionBoxST_BoxST(box_orig, &box_new, &box_union);
	size_union = durationPeriod(&box_union.t) * areaBoxSP(&box_union.sp);

	*penalty = (float) (size_union - size_orig);

	PG_RETURN_POINTER(penalty);
}
PG_FUNCTION_INFO_V1(penaltyGiSTSegmentST);

PGDLLEXPORT Datum picksplitGiSTSegmentST(PG_FUNCTION_ARGS) {
	GistEntryVector *entryvec = (GistEntryVector *) PG_GETARG_POINTER(0);
	OffsetNumber maxoff = entryvec->n - 1, i;
	GISTENTRY *ent = entryvec->vector;
	GIST_SPLITVEC *v = (GIST_SPLITVEC *) PG_GETARG_POINTER(1);
	int nbytes;

	bool isLeafPage;
	BoxST boxUnion, tmpBox;
	OffsetNumber *listL = NULL, *listR = NULL, *listLX = NULL, *listRX = NULL, *listLY = NULL, *listRY = NULL;
	BoxST *unionL = NULL, *unionR = NULL, *unionLX = NULL, *unionRX = NULL, *unionLY = NULL, *unionRY = NULL, LRl, LRr, RLl, RLr;
	int posL, posR, posLX, posRX, posLY, posRY;
	int mposT, mposX, mposY;
	float8 sizeT, sizeX, sizeY, avgCenterT, avgCenterX, avgCenterY, center, sizeLR, sizeRL, p1, p2;
	char direction;
	bool allisequal = true, firstToLeft = true;

	nbytes = (maxoff + 2) * sizeof(OffsetNumber);

	isLeafPage = GIST_LEAF(&ent[FirstOffsetNumber]);

	if (isLeafPage)
		SegmentST2BoxST((SegmentST *) DatumGetPointer(ent[FirstOffsetNumber].key), &tmpBox);
	else
		tmpBox = *(BoxST *) DatumGetPointer(ent[FirstOffsetNumber].key);

	boxUnion = tmpBox;

	for (i = OffsetNumberNext(FirstOffsetNumber); i <= maxoff; i = OffsetNumberNext(i)) {
		if (isLeafPage)
			SegmentST2BoxST((SegmentST *) DatumGetPointer(ent[i].key), &tmpBox);
		else
			tmpBox = *(BoxST *) DatumGetPointer(ent[i].key);

		if (allisequal == true && !equalsBoxST(&boxUnion, &tmpBox))
			allisequal = false;

		unionBoxST_BoxST(&boxUnion, &tmpBox, &boxUnion);
	}

	listL = (OffsetNumber *) palloc(nbytes);
	unionL = (BoxST *) palloc(sizeof(*unionL));
	listR = (OffsetNumber *) palloc(nbytes);
	unionR = (BoxST *) palloc(sizeof(*unionR));

	if (allisequal) {
		v->spl_left = listL;
		v->spl_right = listR;
		v->spl_nleft = v->spl_nright = 0;
		*unionL = boxUnion;
		*unionR = boxUnion;

		for (i = FirstOffsetNumber; i <= maxoff; i = OffsetNumberNext(i)) {
			if (i <= (maxoff - FirstOffsetNumber + 1) / 2) {
				v->spl_left[v->spl_nleft] = i;
				v->spl_nleft++;
			} else {
				v->spl_right[v->spl_nright] = i;
				v->spl_nright++;
			}
		}

		if (v->spl_ldatum_exists)
			unionBoxST_BoxST(unionL, (BoxST *) DatumGetPointer(v->spl_ldatum), unionL);
		v->spl_ldatum = PointerGetDatum(unionL);

		if (v->spl_rdatum_exists)
			unionBoxST_BoxST(unionR, (BoxST *) DatumGetPointer(v->spl_rdatum), unionR);
		v->spl_rdatum = PointerGetDatum(unionR);

		v->spl_ldatum_exists = v->spl_rdatum_exists = false;

		PG_RETURN_POINTER(v);
	}

	listLX = (OffsetNumber *) palloc(nbytes);
	unionLX = (BoxST *) palloc(sizeof(*unionLX));
	listRX = (OffsetNumber *) palloc(nbytes);
	unionRX = (BoxST *) palloc(sizeof(*unionRX));

	listLY = (OffsetNumber *) palloc(nbytes);
	unionLY = (BoxST *) palloc(sizeof(*unionLY));
	listRY = (OffsetNumber *) palloc(nbytes);
	unionRY = (BoxST *) palloc(sizeof(*unionRY));

#define ADDLIST(list, unionD, curBox, pos, num) do { \
	if (pos != 0) { \
		unionBoxST_BoxST(unionD, curBox, unionD); \
	} else { \
		*unionD = *curBox; \
	} \
	list[pos] = num; \
	pos++; \
} while(0)

	posL = posR = posLX = posRX = posLY = posRY = 0;
	avgCenterT = avgCenterX = avgCenterY = 0.0;
	for (i = FirstOffsetNumber ; i <= maxoff; i = OffsetNumberNext(i) ) {
		if (isLeafPage)
			SegmentST2BoxST((SegmentST *) DatumGetPointer(ent[i].key), &tmpBox);
		else
			tmpBox = *(BoxST *) DatumGetPointer(ent[i].key);

		if (tmpBox.t.i - boxUnion.t.i < boxUnion.t.e - tmpBox.t.e)
			ADDLIST(listL, unionL, &tmpBox, posL, i);
		else
			ADDLIST(listR, unionR, &tmpBox, posR, i);

		if (tmpBox.sp.l.x - boxUnion.sp.l.x < boxUnion.sp.h.x - tmpBox.sp.h.x)
			ADDLIST(listLX, unionLX, &tmpBox, posLX, i);
		else
			ADDLIST(listRX, unionRX, &tmpBox, posRX, i);

		if (tmpBox.sp.l.y - boxUnion.sp.l.y < boxUnion.sp.h.y - tmpBox.sp.h.y)
			ADDLIST(listLY, unionLY, &tmpBox, posLY, i);
		else
			ADDLIST(listRY, unionRY, &tmpBox, posRY, i);
		
		avgCenterT += (tmpBox.t.e + tmpBox.t.i) / 2.0;
		avgCenterX += (tmpBox.sp.h.x + tmpBox.sp.l.x) / 2.0;
		avgCenterY += (tmpBox.sp.h.y + tmpBox.sp.l.y) / 2.0;
	}

	avgCenterT /= maxoff;
	avgCenterX /= maxoff;
	avgCenterY /= maxoff;

	if ((posL == 0 || posR == 0 || ((float8) posL / (float8) posR) < 0.1) && (posLX == 0 || posRX == 0 || ((float8) posLX / (float8) posRX) < 0.1) && (posLY == 0 || posRY == 0 || ((float8) posLY / (float8) posRY) < 0.1)) {
		posL = posR = posLX = posRX = posLY = posRY = 0;
		for (i = FirstOffsetNumber; i <= maxoff; i = OffsetNumberNext(i)) {
			if (isLeafPage)
				SegmentST2BoxST((SegmentST *) DatumGetPointer(ent[i].key), &tmpBox);
			else
				tmpBox = *(BoxST *) DatumGetPointer(ent[i].key);

			center = (tmpBox.t.e + tmpBox.t.i) / 2.0;
			if (center < avgCenterT)
				ADDLIST(listL, unionL, &tmpBox, posL, i);
			else if (center == avgCenterT) {
				if (posL > posR)
					ADDLIST(listR, unionR, &tmpBox, posR, i);
				else
					ADDLIST(listL, unionL, &tmpBox, posL, i);
			} else
				ADDLIST(listR, unionR, &tmpBox, posR, i);

			center = (tmpBox.sp.h.x + tmpBox.sp.l.x) / 2.0;
			if (center < avgCenterX)
				ADDLIST(listLX, unionLX, &tmpBox, posLX, i);
			else if (center == avgCenterX) {
				if (posLX > posRX)
					ADDLIST(listRX, unionRX, &tmpBox, posRX, i);
				else
					ADDLIST(listLX, unionLX, &tmpBox, posLX, i);
			} else
				ADDLIST(listRX, unionRX, &tmpBox, posRX, i);

			center = (tmpBox.sp.h.y + tmpBox.sp.l.y) / 2.0;
			if (center < avgCenterY)
				ADDLIST(listLY, unionLY, &tmpBox, posLY, i);
			else if (center == avgCenterY) {
				if (posLY > posRY)
					ADDLIST(listRY, unionRY, &tmpBox, posRY, i);
				else
					ADDLIST(listLY, unionLY, &tmpBox, posLY, i);
			} else
				ADDLIST(listRY, unionRY, &tmpBox, posRY, i);
		}
		
		if ((posL == 0 || posR == 0 || ((float8) posL / (float8) posR) < 0.1) && (posLX == 0 || posRX == 0 || ((float8) posLX / (float8) posRX) < 0.1) && (posLY == 0 || posRY == 0 || ((float8) posLY / (float8) posRY) < 0.1)) {
			posL = posR = 0;
			for (i = FirstOffsetNumber; i <= maxoff; i = OffsetNumberNext(i)) {
				if (isLeafPage)
					SegmentST2BoxST((SegmentST *) DatumGetPointer(ent[i].key), &tmpBox);
				else
					tmpBox = *(BoxST *) DatumGetPointer(ent[i].key);

				if (i <= (maxoff - FirstOffsetNumber + 1) / 2)
					ADDLIST(listL, unionL, &tmpBox, posL, i);
				else
					ADDLIST(listR, unionR, &tmpBox, posR, i);
			}

			v->spl_left = listL;
			v->spl_nleft = posL;
			if (v->spl_ldatum_exists)
				unionBoxST_BoxST(unionL, (BoxST *) DatumGetPointer(v->spl_ldatum), unionL);
			v->spl_ldatum = PointerGetDatum(unionL);

			v->spl_right = listR;
			v->spl_nright = posR;
			if (v->spl_rdatum_exists)
				unionBoxST_BoxST(unionR, (BoxST *) DatumGetPointer(v->spl_rdatum), unionR);
			v->spl_rdatum = PointerGetDatum(unionR);

			v->spl_ldatum_exists = v->spl_rdatum_exists = false;

			PG_RETURN_POINTER(v);
		}
	}

	mposT = Max(posL, posR);
	mposX = Max(posLX, posRX);
	mposY = Max(posLY, posRY);

	sizeT = intersectionBoxST_BoxST(unionL, unionR, &tmpBox) ? durationPeriod(&tmpBox.t) * areaBoxSP(&tmpBox.sp) : 0.0;
	sizeX = intersectionBoxST_BoxST(unionLX, unionRX, &tmpBox) ? durationPeriod(&tmpBox.t) * areaBoxSP(&tmpBox.sp) : 0.0;
	sizeY = intersectionBoxST_BoxST(unionLY, unionRY, &tmpBox) ? durationPeriod(&tmpBox.t) * areaBoxSP(&tmpBox.sp) : 0.0;

	direction = 't';
	if ((mposT == mposX) && (mposX == mposY)) {
		if (sizeX <= sizeY && sizeX < sizeT)
			direction = 'x';
		else if (sizeY < sizeX && sizeY < sizeT)
			direction = 'y';
		else
			direction = 't';
	} else if (mposX == mposY) {
		if (mposX < mposT)
			direction = sizeY < sizeX ? 'y' : 'x';
		else
			direction = 't';
	} else if (mposT == mposX) {
		if (mposT < mposY)
			direction = sizeX < sizeT ? 'x' : 't';
		else
			direction = 'y';
	} else if (mposT == mposY) {
		if (mposT < mposX)
			direction = sizeY < sizeT ? 'y' : 't';
		else
			direction = 'x';
	} else {
		if (mposX < mposY && mposX < mposT)
			direction = 'x';
		else if (mposY < mposX && mposY < mposT)
			direction = 'y';
		else
			direction = 't';
	}

	if (direction == 'x') {
		pfree(listLY);
		pfree(unionLY);
		pfree(listRY);
		pfree(unionRY);

		pfree(listL);
		pfree(unionL);
		pfree(listR);
		pfree(unionR);

		listL = listLX;
		posL = posLX;
		unionL = unionLX;

		listR = listRX;
		posR = posRX;
		unionR = unionRX;
	} else if (direction == 'y') {
		pfree(listLX);
		pfree(unionLX);
		pfree(listRX);
		pfree(unionRX);

		pfree(listL);
		pfree(unionL);
		pfree(listR);
		pfree(unionR);

		listL = listLY;
		posL = posLY;
		unionL = unionLY;

		listR = listRY;
		posR = posRY;
		unionR = unionRY;
	} else {
		pfree(listLX);
		pfree(unionLX);
		pfree(listRX);
		pfree(unionRX);

		pfree(listLY);
		pfree(unionLY);
		pfree(listRY);
		pfree(unionRY);
	}

	if (v->spl_ldatum_exists || v->spl_rdatum_exists) {
		if (v->spl_ldatum_exists && v->spl_rdatum_exists) {
			unionBoxST_BoxST(unionL, (BoxST *) DatumGetPointer(v->spl_ldatum), &LRl);
			unionBoxST_BoxST(unionR, (BoxST *) DatumGetPointer(v->spl_rdatum), &LRr);
			unionBoxST_BoxST(unionR, (BoxST *) DatumGetPointer(v->spl_ldatum), &RLl);
			unionBoxST_BoxST(unionL, (BoxST *) DatumGetPointer(v->spl_rdatum), &RLr);

			sizeLR = intersectionBoxST_BoxST(&LRl, &LRr, &tmpBox) ? durationPeriod(&tmpBox.t) * areaBoxSP(&tmpBox.sp) : 0.0;
			sizeRL = intersectionBoxST_BoxST(&RLl, &RLr, &tmpBox) ? durationPeriod(&tmpBox.t) * areaBoxSP(&tmpBox.sp) : 0.0;

			if (sizeLR > sizeRL)
				firstToLeft = false;
		} else {
			tmpBox = *(BoxST *) DatumGetPointer(v->spl_ldatum_exists ? v->spl_ldatum : v->spl_rdatum);

			p2 = p1 = durationPeriod(&tmpBox.t) * areaBoxSP(&tmpBox.sp);

			unionBoxST_BoxST(&tmpBox, unionL, &boxUnion);
			p1 = durationPeriod(&boxUnion.t) * areaBoxSP(&boxUnion.sp) - p1;

			unionBoxST_BoxST(&tmpBox, unionR, &boxUnion);
			p2 = durationPeriod(&boxUnion.t) * areaBoxSP(&boxUnion.sp) - p2;

			if ((v->spl_ldatum_exists && p1 > p2) || (v->spl_rdatum_exists && p1 < p2))
				firstToLeft = false;
		}
	}

	if (firstToLeft) {
		v->spl_left = listL;
		v->spl_nleft = posL;
		if (v->spl_ldatum_exists)
			unionBoxST_BoxST(unionL, (BoxST *) DatumGetPointer(v->spl_ldatum), unionL);
		v->spl_ldatum = PointerGetDatum(unionL);

		v->spl_right = listR;
		v->spl_nright = posR;
		if (v->spl_rdatum_exists)
			unionBoxST_BoxST(unionR, (BoxST *) DatumGetPointer(v->spl_rdatum), unionR);
		v->spl_rdatum = PointerGetDatum(unionR);
	} else {
		v->spl_left = listR;
		v->spl_nleft = posR;
		if (v->spl_ldatum_exists)
			unionBoxST_BoxST(unionR, (BoxST *) DatumGetPointer(v->spl_ldatum), unionR);
		v->spl_ldatum = PointerGetDatum(unionR);

		v->spl_right = listL;
		v->spl_nright = posL;
		if (v->spl_rdatum_exists)
			unionBoxST_BoxST(unionL, (BoxST *) DatumGetPointer(v->spl_rdatum), unionL);
		v->spl_rdatum = PointerGetDatum(unionL);
	}

	v->spl_ldatum_exists = v->spl_rdatum_exists = false;

	PG_RETURN_POINTER(v);
}
PG_FUNCTION_INFO_V1(picksplitGiSTSegmentST);

PGDLLEXPORT Datum sameGiSTSegmentST(PG_FUNCTION_ARGS) {
	BoxST *a = (BoxST *) PG_GETARG_POINTER(0);
	BoxST *b = (BoxST *) PG_GETARG_POINTER(1);
	bool *result = (bool *) PG_GETARG_POINTER(2);

	if (a && b) {
		if (a->sp.l.x > a->sp.h.x || a->sp.l.y > a->sp.h.y)
			ereport(INFO, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("same: a not box, %s", outBoxST(a))));
		if (b->sp.l.x > b->sp.h.x || b->sp.l.y > b->sp.h.y)
			ereport(INFO, (errcode(ERRCODE_RAISE_EXCEPTION), errmsg("same: b not box, %s", outBoxST(b))));

		*result = equalsBoxST(a, b);
	} else
		*result = (a == NULL && b == NULL);

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(sameGiSTSegmentST);
