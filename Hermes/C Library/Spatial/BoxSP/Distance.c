/**
 *	@file
 *	@author Marios Vodas (mvodas@gmail.com).
 *	@brief The implementation of BoxSP
 *
 *	@details These functions are documented in the @ref BoxSP/Distance.h
 *
 */

#include "../../h/Spatial/BoxSP/Distance.h"
#include "../../h/Spatial/BoxSP/RelationalOperators.h"
#include "../../h/Spatial/BoxSP/Properties.h"
#include "../../h/Spatial/PointSP/Distance.h"
#include "../../h/Spatial/PointSP/Casts.h"
#include "../../h/Spatial/SegmentSP/RelationalOperators.h"
#include "../../h/Spatial/SegmentSP/Distance.h"
#include "../../h/Utilities/Utilities.h"

float8 distanceBoxSP_PointSP(const BoxSP *box, const PointSP *point) {
	BoxSP pointBox;

	PointSP2BoxSP(point, &pointBox);

	return distanceBoxSP_BoxSP(box, &pointBox);
}

PGDLLEXPORT Datum distanceBoxSP_PointSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(distanceBoxSP_PointSP((BoxSP *) PG_GETARG_POINTER(0), (PointSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(distanceBoxSP_PointSPV1);

float8 distanceBoxSP_SegmentSP(const BoxSP *box, const SegmentSP *segment) {
	SegmentSP edge;
	float8 result;

	if (intersectsSegmentSP_BoxSP(segment, box)) {
		return 0.0;
	}

	lh_hBoxSP(box, &edge);
	result = distanceSegmentSP_SegmentSP(segment, &edge);

	h_hlBoxSP(box, &edge);
	result = minFloat8(result, distanceSegmentSP_SegmentSP(segment, &edge));

	hl_lBoxSP(box, &edge);
	result = minFloat8(result, distanceSegmentSP_SegmentSP(segment, &edge));

	l_lhBoxSP(box, &edge);
	result = minFloat8(result, distanceSegmentSP_SegmentSP(segment, &edge));

	return result;
}

PGDLLEXPORT Datum distanceBoxSP_SegmentSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(distanceBoxSP_SegmentSP((BoxSP *) PG_GETARG_POINTER(0), (SegmentSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(distanceBoxSP_SegmentSPV1);

float8 distanceBoxSP_BoxSP(const BoxSP *boxA, const BoxSP *boxB) {
	bool north, south, east, west;
	PointSP tmp1, tmp2;
	SegmentSP tmp3, tmp4;
	float8 result;

	north = northBoxSP_BoxSP(boxB, boxA);
	south = southBoxSP_BoxSP(boxB, boxA);
	east = eastBoxSP_BoxSP(boxB, boxA);
	west = westBoxSP_BoxSP(boxB, boxA);

	if (north && west) {
		hlBoxSP(boxB, &tmp1);
		lhBoxSP(boxA, &tmp2);

		result = distancePointSP_PointSP(&tmp1, &tmp2);
	} else if (north && east) {
		result = distancePointSP_PointSP(&boxB->l, &boxA->h);
	} else if (south && west) {
		result = distancePointSP_PointSP(&boxB->h, &boxA->l);
	} else if (south && east) {
		lhBoxSP(boxB, &tmp1);
		hlBoxSP(boxA, &tmp2);

		result = distancePointSP_PointSP(&tmp1, &tmp2);
	} else if (north) {
		l_hlBoxSP(boxB, &tmp3);
		lh_hBoxSP(boxA, &tmp4);

		result = distanceSegmentSP_SegmentSP(&tmp3, &tmp4);
	} else if (east) {
		l_lhBoxSP(boxB, &tmp3);
		hl_hBoxSP(boxA, &tmp4);

		result = distanceSegmentSP_SegmentSP(&tmp3, &tmp4);
	} else if (south) {
		lh_hBoxSP(boxB, &tmp3);
		l_hlBoxSP(boxA, &tmp4);

		result = distanceSegmentSP_SegmentSP(&tmp3, &tmp4);
	} else if (west) {
		hl_hBoxSP(boxB, &tmp3);
		l_lhBoxSP(boxA, &tmp4);

		result = distanceSegmentSP_SegmentSP(&tmp3, &tmp4);
	} else {
		result = 0.0;
	}

	return result;
}

PGDLLEXPORT Datum distanceBoxSP_BoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(distanceBoxSP_BoxSP((BoxSP *) PG_GETARG_POINTER(0), (BoxSP *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(distanceBoxSP_BoxSPV1);
