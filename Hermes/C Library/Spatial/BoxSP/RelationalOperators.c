/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/Spatial/BoxSP/RelationalOperators.h"

bool intersectsBoxSP_BoxSP(const BoxSP *boxA, const BoxSP *boxB) {
	return ((boxA->l.x <= boxB->h.x && boxB->h.x <= boxA->h.x) || (boxB->l.x <= boxA->h.x && boxA->h.x <= boxB->h.x)) && ((boxA->l.y <= boxB->h.y && boxB->h.y <= boxA->h.y) || (boxB->l.y <= boxA->h.y && boxA->h.y <= boxB->h.y));
}

PGDLLEXPORT Datum intersectsBoxSP_BoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(intersectsBoxSP_BoxSP((BoxSP *) PG_GETARG_POINTER(0), (BoxSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(intersectsBoxSP_BoxSPV1);

bool containsBoxSP_PointSP(const BoxSP *box, const PointSP *point) {
	return box->l.x <= point->x && point->x <= box->h.x && box->l.y <= point->y && point->y <= box->h.y;
}

bool containsBoxSP_BoxSP(const BoxSP *boxA, const BoxSP *boxB) {
	return boxA->l.x <= boxB->l.x && boxB->h.x <= boxA->h.x && boxA->l.y <= boxB->l.y && boxB->h.y <= boxA->h.y;
}

PGDLLEXPORT Datum containsBoxSP_PointSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(containsBoxSP_PointSP((BoxSP *) PG_GETARG_POINTER(0), (PointSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(containsBoxSP_PointSPV1);

PGDLLEXPORT Datum containsBoxSP_BoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(containsBoxSP_BoxSP((BoxSP *) PG_GETARG_POINTER(0), (BoxSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(containsBoxSP_BoxSPV1);

bool containsProperlyBoxSP_PointSP(const BoxSP *box, const PointSP *point) {
	return box->l.x < point->x && point->x < box->h.x && box->l.y < point->y && point->y < box->h.y;
}

bool containsProperlyBoxSP_BoxSP(const BoxSP *boxA, const BoxSP *boxB) {
	return boxA->l.x < boxB->l.x && boxB->h.x < boxA->h.x && boxA->l.y < boxB->l.y && boxB->h.y < boxA->h.y;
}

PGDLLEXPORT Datum containsProperlyBoxSP_PointSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(containsProperlyBoxSP_PointSP((BoxSP *) PG_GETARG_POINTER(0), (PointSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(containsProperlyBoxSP_PointSPV1);

PGDLLEXPORT Datum containsProperlyBoxSP_BoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(containsProperlyBoxSP_BoxSP((BoxSP *) PG_GETARG_POINTER(0), (BoxSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(containsProperlyBoxSP_BoxSPV1);

bool northBoxSP_PointSP(const BoxSP *box, const PointSP *point) {
	return box->l.y > point->y;
}

bool northBoxSP_BoxSP(const BoxSP *boxA, const BoxSP *boxB) {
	return boxA->l.y > boxB->h.y;
}

PGDLLEXPORT Datum northBoxSP_PointSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(northBoxSP_PointSP((BoxSP *) PG_GETARG_POINTER(0), (PointSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(northBoxSP_PointSPV1);

PGDLLEXPORT Datum northBoxSP_BoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(northBoxSP_BoxSP((BoxSP *) PG_GETARG_POINTER(0), (BoxSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(northBoxSP_BoxSPV1);

bool southBoxSP_PointSP(const BoxSP *box, const PointSP *point) {
	return box->h.y < point->y;
}

bool southBoxSP_BoxSP(const BoxSP *boxA, const BoxSP *boxB) {
	return boxA->h.y < boxB->l.y;
}

PGDLLEXPORT Datum southBoxSP_PointSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(southBoxSP_PointSP((BoxSP *) PG_GETARG_POINTER(0), (PointSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(southBoxSP_PointSPV1);

PGDLLEXPORT Datum southBoxSP_BoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(southBoxSP_BoxSP((BoxSP *) PG_GETARG_POINTER(0), (BoxSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(southBoxSP_BoxSPV1);

bool eastBoxSP_PointSP(const BoxSP *box, const PointSP *point) {
	return box->l.x > point->x;
}

bool eastBoxSP_BoxSP(const BoxSP *boxA, const BoxSP *boxB) {
	return boxA->l.x > boxB->h.x;
}

PGDLLEXPORT Datum eastBoxSP_PointSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(eastBoxSP_PointSP((BoxSP *) PG_GETARG_POINTER(0), (PointSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(eastBoxSP_PointSPV1);

PGDLLEXPORT Datum eastBoxSP_BoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(eastBoxSP_BoxSP((BoxSP *) PG_GETARG_POINTER(0), (BoxSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(eastBoxSP_BoxSPV1);

bool westBoxSP_PointSP(const BoxSP *box, const PointSP *point) {
	return box->h.x < point->x;
}

bool westBoxSP_BoxSP(const BoxSP *boxA, const BoxSP *boxB) {
	return boxA->h.x < boxB->l.x;
}

PGDLLEXPORT Datum westBoxSP_PointSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(westBoxSP_PointSP((BoxSP *) PG_GETARG_POINTER(0), (PointSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(westBoxSP_PointSPV1);

PGDLLEXPORT Datum westBoxSP_BoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(westBoxSP_BoxSP((BoxSP *) PG_GETARG_POINTER(0), (BoxSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(westBoxSP_BoxSPV1);
