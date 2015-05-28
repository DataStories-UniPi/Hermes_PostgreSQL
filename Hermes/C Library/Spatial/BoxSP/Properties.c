/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/Spatial/BoxSP/Properties.h"
#include "../../h/Utilities/MSVCWorkarounds.h"

bool isPunctalBoxSP(const BoxSP *box) {
	return box->h.x == box->l.x && box->h.y == box->l.y;
}

PGDLLEXPORT Datum isPunctalBoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_BOOL(isPunctalBoxSP((BoxSP *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(isPunctalBoxSPV1);

int32 widthBoxSP(const BoxSP *box) {
	return box->h.x - box->l.x;
}

int32 heightBoxSP(const BoxSP *box) {
	return box->h.y - box->l.y;
}

int64 areaBoxSP(const BoxSP *box) {
	return (int64) (box->h.x - box->l.x) * (box->h.y - box->l.y);
}

PGDLLEXPORT Datum widthBoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(widthBoxSP((BoxSP *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(widthBoxSPV1);

PGDLLEXPORT Datum heightBoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(heightBoxSP((BoxSP *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(heightBoxSPV1);

PGDLLEXPORT Datum areaBoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_INT64(areaBoxSP((BoxSP *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(areaBoxSPV1);

PointXY *centerBoxSP(const BoxSP *box, PointXY *result) {
	PointXY *ret = result ? result : (PointXY *) palloc(sizeof(*ret));

	ret->x = (box->l.x + box->h.x) / 2.0;
	ret->y = (box->l.y + box->h.y) / 2.0;

	return ret;
}

float8 centerXBoxSP(const BoxSP *box) {
	return (box->l.x + box->h.x) / 2.0;
}

float8 centerYBoxSP(const BoxSP *e) {
	return (e->l.y + e->h.y) / 2.0;
}

PGDLLEXPORT Datum centerBoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(centerBoxSP((BoxSP *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(centerBoxSPV1);

PGDLLEXPORT Datum centerXBoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(centerXBoxSP((BoxSP *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(centerXBoxSPV1);

PGDLLEXPORT Datum centerYBoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(centerYBoxSP((BoxSP *) PG_GETARG_POINTER(0)));
}
PG_FUNCTION_INFO_V1(centerYBoxSPV1);

PointSP *lhBoxSP(const BoxSP *box, PointSP *result) {
	PointSP *ret = result ? result : (PointSP *) palloc(sizeof(*ret));

	ret->x = box->l.x;
	ret->y = box->h.y;

	return ret;
}

PointSP *hlBoxSP(const BoxSP *box, PointSP *result) {
	PointSP *ret = result ? result : (PointSP *) palloc(sizeof(*ret));

	ret->x = box->h.x;
	ret->y = box->l.y;

	return ret;
}

PGDLLEXPORT Datum lhBoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(lhBoxSP((BoxSP *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(lhBoxSPV1);

PGDLLEXPORT Datum hlBoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(hlBoxSP((BoxSP *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(hlBoxSPV1);

SegmentSP *l_hBoxSP(const BoxSP *box, SegmentSP *result) {
	SegmentSP *ret = result ? result : (SegmentSP *) palloc(sizeof(*ret));

	ret->i = box->l;
	ret->e = box->h;

	return ret;
}

SegmentSP *h_lBoxSP(const BoxSP *box, SegmentSP *result) {
	SegmentSP *ret = result ? result : (SegmentSP *) palloc(sizeof(*ret));

	ret->i = box->h;
	ret->e = box->l;

	return ret;
}

PGDLLEXPORT Datum l_hBoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(l_hBoxSP((BoxSP *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(l_hBoxSPV1);

PGDLLEXPORT Datum h_lBoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(h_lBoxSP((BoxSP *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(h_lBoxSPV1);

SegmentSP *lh_hlBoxSP(const BoxSP *box, SegmentSP *result) {
	SegmentSP *ret = result ? result : (SegmentSP *) palloc(sizeof(*ret));

	ret->i.x = box->l.x;
	ret->i.y = box->h.y;
	ret->e.x = box->h.x;
	ret->e.y = box->l.y;

	return ret;
}

SegmentSP *hl_lhBoxSP(const BoxSP *box, SegmentSP *result) {
	SegmentSP *ret = result ? result : (SegmentSP *) palloc(sizeof(*ret));

	ret->i.x = box->h.x;
	ret->i.y = box->l.y;
	ret->e.x = box->l.x;
	ret->e.y = box->h.y;

	return ret;
}

PGDLLEXPORT Datum lh_hlBoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(lh_hlBoxSP((BoxSP *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(lh_hlBoxSPV1);

PGDLLEXPORT Datum hl_lhBoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(hl_lhBoxSP((BoxSP *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(hl_lhBoxSPV1);

SegmentSP *lh_hBoxSP(const BoxSP *box, SegmentSP *result) {
	SegmentSP *ret = result ? result : (SegmentSP *) palloc(sizeof(*ret));

	ret->i.x = box->l.x;
	ret->i.y = box->h.y;
	ret->e = box->h;

	return ret;
}

SegmentSP *h_lhBoxSP(const BoxSP *box, SegmentSP *result) {
	SegmentSP *ret = result ? result : (SegmentSP *) palloc(sizeof(*ret));

	ret->i = box->h;
	ret->e.x = box->l.x;
	ret->e.y = box->h.y;

	return ret;
}

PGDLLEXPORT Datum lh_hBoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(lh_hBoxSP((BoxSP *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(lh_hBoxSPV1);

PGDLLEXPORT Datum h_lhBoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(h_lhBoxSP((BoxSP *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(h_lhBoxSPV1);

SegmentSP *h_hlBoxSP(const BoxSP *box, SegmentSP *result) {
	SegmentSP *ret = result ? result : (SegmentSP *) palloc(sizeof(*ret));

	ret->i = box->h;
	ret->e.x = box->h.x;
	ret->e.y = box->l.y;

	return ret;
}

SegmentSP *hl_hBoxSP(const BoxSP *box, SegmentSP *result) {
	SegmentSP *ret = result ? result : (SegmentSP *) palloc(sizeof(*ret));

	ret->i.x = box->h.x;
	ret->i.y = box->l.y;
	ret->e = box->h;

	return ret;
}

PGDLLEXPORT Datum h_hlBoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(h_hlBoxSP((BoxSP *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(h_hlBoxSPV1);

PGDLLEXPORT Datum hl_hBoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(hl_hBoxSP((BoxSP *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(hl_hBoxSPV1);

SegmentSP *hl_lBoxSP(const BoxSP *box, SegmentSP *result) {
	SegmentSP *ret = result ? result : (SegmentSP *) palloc(sizeof(*ret));

	ret->i.x = box->h.x;
	ret->i.y = box->l.y;
	ret->e = box->l;

	return ret;
}

SegmentSP *l_hlBoxSP(const BoxSP *box, SegmentSP *result) {
	SegmentSP *ret = result ? result : (SegmentSP *) palloc(sizeof(*ret));

	ret->i = box->l;
	ret->e.x = box->h.x;
	ret->e.y = box->l.y;

	return ret;
}

PGDLLEXPORT Datum hl_lBoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(hl_lBoxSP((BoxSP *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(hl_lBoxSPV1);

PGDLLEXPORT Datum l_hlBoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(l_hlBoxSP((BoxSP *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(l_hlBoxSPV1);

SegmentSP *l_lhBoxSP(const BoxSP *box, SegmentSP *result) {
	SegmentSP *ret = result ? result : (SegmentSP *) palloc(sizeof(*ret));

	ret->i = box->l;
	ret->e.x = box->l.x;
	ret->e.y = box->h.y;

	return ret;
}

SegmentSP *lh_lBoxSP(const BoxSP *box, SegmentSP *result) {
	SegmentSP *ret = result ? result : (SegmentSP *) palloc(sizeof(*ret));

	ret->i.x = box->l.x;
	ret->i.y = box->h.y;
	ret->e = box->l;

	return ret;
}

PGDLLEXPORT Datum l_lhBoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(l_lhBoxSP((BoxSP *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(l_lhBoxSPV1);

PGDLLEXPORT Datum lh_lBoxSPV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(lh_lBoxSP((BoxSP *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(lh_lBoxSPV1);
