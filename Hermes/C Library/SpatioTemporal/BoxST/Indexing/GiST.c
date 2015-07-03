/**
 *	@file
 *	@author Marios Vodas (mvodas@gmail.com).
 *	@brief The implementantion of BoxST
 *
 */

#include "../../../h/SpatioTemporal/BoxST/Interactions.h"
#include "../../../h/SpatioTemporal/BoxST/RelationalOperators.h"
#include "../../../h/Temporal/Period/Properties.h"
#include "../../../h/Temporal/Period/RelationalOperators.h"
#include "../../../h/Temporal/Period/Distance.h"
#include "../../../h/Spatial/BoxSP/Properties.h"
#include "../../../h/Spatial/BoxSP/RelationalOperators.h"
#include "../../../h/Spatial/BoxSP/Distance.h"
#include <access/gist.h>
#include <access/skey.h>
#include <parser/parse_type.h>
#include <utils/syscache.h>

PGDLLEXPORT Datum consistentGiSTBoxST(PG_FUNCTION_ARGS) {
	GISTENTRY *entry = (GISTENTRY *) PG_GETARG_POINTER(0);
	StrategyNumber strategy = (StrategyNumber) PG_GETARG_UINT16(2);
	Oid subtype = PG_GETARG_OID(3);
	bool *recheck = (bool *) PG_GETARG_POINTER(4);
	bool retval;
	Type typ;
	char *typname = NULL;

	*recheck = false;

	if (PG_ARGISNULL(1))
		PG_RETURN_BOOL(false);

	retval = false;

	typ = typeidType(subtype);
	typname = typeTypeName(typ);
	ReleaseSysCache(typ);

	if (strategy == 1) {
		if (strcmp(typname, "boxst") == 0)
			retval = intersectsBoxST_BoxST((BoxST *) DatumGetPointer(entry->key), (BoxST *) PG_GETARG_POINTER(1));
	} else if (strategy == 2) {
		if (strcmp(typname, "period") == 0)
			retval = intersectsPeriod_Period(&((BoxST *) DatumGetPointer(entry->key))->t, (Period *) PG_GETARG_POINTER(1));
	} else if (strategy == 3) {
		if (strcmp(typname, "boxsp") == 0)
			retval = intersectsBoxSP_BoxSP(&((BoxST *) DatumGetPointer(entry->key))->sp, (BoxSP *) PG_GETARG_POINTER(1));
	}

	PG_RETURN_BOOL(retval);
}
PG_FUNCTION_INFO_V1(consistentGiSTBoxST);

PGDLLEXPORT Datum distanceGiSTBoxST(PG_FUNCTION_ARGS) {
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

	if (strategy == 4) {
		if (strcmp(typname, "period") == 0)
			retval = distancePeriod_Period(&((BoxST *) DatumGetPointer(entry->key))->t, (Period *) PG_GETARG_POINTER(1));
	} else if (strategy == 5) {
		if (strcmp(typname, "boxsp") == 0)
			retval = distanceBoxSP_BoxSP(&((BoxST *) DatumGetPointer(entry->key))->sp, (BoxSP *) PG_GETARG_POINTER(1));
	}

	PG_RETURN_FLOAT8(retval);
}
PG_FUNCTION_INFO_V1(distanceGiSTBoxST);

PGDLLEXPORT Datum compressGiSTBoxST(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(PG_GETARG_POINTER(0));
}
PG_FUNCTION_INFO_V1(compressGiSTBoxST);

PGDLLEXPORT Datum decompressGiSTBoxST(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(PG_GETARG_POINTER(0));
}
PG_FUNCTION_INFO_V1(decompressGiSTBoxST);

PGDLLEXPORT Datum unionGiSTBoxST(PG_FUNCTION_ARGS) {
	GistEntryVector *entryvec = (GistEntryVector *) PG_GETARG_POINTER(0);
	GISTENTRY *ent = entryvec->vector;

	int i;
	BoxST *curBox = NULL, *pageunion = NULL;

	int *sizep = (int *) PG_GETARG_POINTER(1);

	curBox = (BoxST *) DatumGetPointer(ent[0].key);
	pageunion = cloneBoxST(curBox, NULL);

	for (i = 1; i < entryvec->n; i++) {
		curBox = (BoxST *) DatumGetPointer(ent[i].key);
		unionBoxST_BoxST(pageunion, curBox, pageunion);
	}

	*sizep = sizeof(*pageunion);

	PG_RETURN_POINTER(pageunion);
}
PG_FUNCTION_INFO_V1(unionGiSTBoxST);

PGDLLEXPORT Datum penaltyGiSTBoxST(PG_FUNCTION_ARGS) {
	GISTENTRY *origentry = (GISTENTRY *) PG_GETARG_POINTER(0);
	GISTENTRY *newentry = (GISTENTRY *) PG_GETARG_POINTER(1);
	float *penalty = (float *) PG_GETARG_POINTER(2);

	BoxST *box_orig = NULL, *box_new = NULL, box_union;
	float8 size_union, size_orig;

	box_orig = (BoxST *) DatumGetPointer(origentry->key);
	box_new = (BoxST *) DatumGetPointer(newentry->key);

	size_orig = durationPeriod(&box_orig->t) * areaBoxSP(&box_orig->sp);

	unionBoxST_BoxST(box_orig, box_new, &box_union);
	size_union = durationPeriod(&box_union.t) * areaBoxSP(&box_union.sp);

	*penalty = (float) (size_union - size_orig);

	PG_RETURN_POINTER(penalty);
}
PG_FUNCTION_INFO_V1(penaltyGiSTBoxST);

PGDLLEXPORT Datum picksplitGiSTBoxST(PG_FUNCTION_ARGS) {
	GistEntryVector *entryvec = (GistEntryVector *) PG_GETARG_POINTER(0);
	OffsetNumber maxoff = entryvec->n - 1, i;
	GISTENTRY *ent = entryvec->vector;
	GIST_SPLITVEC *v = (GIST_SPLITVEC *) PG_GETARG_POINTER(1);
	int nbytes;

	BoxST boxUnion, tmpBox;
	OffsetNumber *listL = NULL, *listR = NULL, *listLX = NULL, *listRX = NULL, *listLY = NULL, *listRY = NULL;
	BoxST *unionL = NULL, *unionR = NULL, *unionLX = NULL, *unionRX = NULL, *unionLY = NULL, *unionRY = NULL, LRl, LRr, RLl, RLr;
	int posL, posR, posLX, posRX, posLY, posRY;
	int mposT, mposX, mposY;
	float8 sizeT, sizeX, sizeY, avgCenterT, avgCenterX, avgCenterY, center, sizeLR, sizeRL, p1, p2;
	char direction;
	bool allisequal = true, firstToLeft = true;

	nbytes = (maxoff + 2) * sizeof(OffsetNumber);

	tmpBox = *(BoxST *) DatumGetPointer(ent[FirstOffsetNumber].key);

	boxUnion = tmpBox;

	for (i = OffsetNumberNext(FirstOffsetNumber); i <= maxoff; i = OffsetNumberNext(i)) {
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
PG_FUNCTION_INFO_V1(picksplitGiSTBoxST);

PGDLLEXPORT Datum sameGiSTBoxST(PG_FUNCTION_ARGS) {
	BoxST *a = (BoxST *) PG_GETARG_POINTER(0);
	BoxST *b = (BoxST *) PG_GETARG_POINTER(1);
	bool *result = (bool *) PG_GETARG_POINTER(2);

	if (a && b)
		*result = equalsBoxST(a, b);
	else
		*result = (a == NULL && b == NULL);

	PG_RETURN_POINTER(result);
}
PG_FUNCTION_INFO_V1(sameGiSTBoxST);
