/**
 *	@file
 *	@author Marios Vodas (mvodas@gmail.com).
 *	@brief The implementantion of BoxST
 *
 *	@details These functions are documented in the @ref BoxST/RelationalOperators.h
 *
 */

#include "../../h/SpatioTemporal/BoxST/RelationalOperators.h"
#include "../../h/Temporal/Period/RelationalOperators.h"
#include "../../h/Spatial/BoxSP/RelationalOperators.h"

bool intersectsBoxST_BoxST(const BoxST *boxA, const BoxST *boxB) {
	return intersectsPeriod_Period(&boxA->t, &boxB->t) && intersectsBoxSP_BoxSP(&boxA->sp, &boxB->sp);
}

PGDLLEXPORT Datum intersectsBoxST_BoxSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(intersectsBoxST_BoxST((BoxST *) PG_GETARG_POINTER(0), (BoxST *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(intersectsBoxST_BoxSTV1);

bool containsBoxST_PointST(const BoxST *box, const PointST *point) {
	return containsPeriod_Timestamp(&box->t, point->t) && containsBoxSP_PointSP(&box->sp, &point->sp);
}

bool containsBoxST_BoxST(const BoxST *boxA, const BoxST *boxB) {
	return containsPeriod_Period(&boxA->t, &boxB->t) && containsBoxSP_BoxSP(&boxA->sp, &boxB->sp);
}

PGDLLEXPORT Datum containsBoxST_PointSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(containsBoxST_PointST((BoxST *) PG_GETARG_POINTER(0), (PointST *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(containsBoxST_PointSTV1);

PGDLLEXPORT Datum containsBoxST_BoxSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(containsBoxST_BoxST((BoxST *) PG_GETARG_POINTER(0), (BoxST *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(containsBoxST_BoxSTV1);

bool containsProperlyBoxST_PointST(const BoxST *box, const PointST *point) {
	return containsProperlyPeriod_Timestamp(&box->t, point->t) && containsProperlyBoxSP_PointSP(&box->sp, &point->sp);
}

bool containsProperlyBoxST_BoxST(const BoxST *boxA, const BoxST *boxB) {
	return containsProperlyPeriod_Period(&boxA->t, &boxB->t) && containsProperlyBoxSP_BoxSP(&boxA->sp, &boxB->sp);
}

PGDLLEXPORT Datum containsProperlyBoxST_PointSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(containsProperlyBoxST_PointST((BoxST *) PG_GETARG_POINTER(0), (PointST *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(containsProperlyBoxST_PointSTV1);

PGDLLEXPORT Datum containsProperlyBoxST_BoxSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(containsProperlyBoxST_BoxST((BoxST *) PG_GETARG_POINTER(0), (BoxST *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(containsProperlyBoxST_BoxSTV1);
