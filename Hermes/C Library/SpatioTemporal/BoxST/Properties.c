/**
 *	@file
 *	@author Marios Vodas (mvodas@gmail.com).
 *	@brief The implementantion of BoxST
 *
 *	@details These functions are documented in the @ref BoxST/Properties.h
 *
 */

#include "../../h/SpatioTemporal/BoxST/Properties.h"
#include "../../h/Temporal/Period/Properties.h"
#include "../../h/Spatial/BoxSP/Properties.h"

bool isPunctalBoxST(const BoxST *box) {
	return isPunctalPeriod(&box->t) && isPunctalBoxSP(&box->sp);
}

PGDLLEXPORT Datum isPunctalBoxSTV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(isPunctalBoxST((BoxST *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(isPunctalBoxSTV1);
