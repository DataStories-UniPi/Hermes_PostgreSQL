/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../h/Spatial/CRS/Conversions.h"
#include "../../h/Spatial/CRS/Ellipsoid/Definition.h"
#include "../../h/Spatial/SessionParameters.h"
#include "../../h/Utilities/Utilities.h"
#include <math.h>

PointXYZ *plh2xyz(const PointPLH *point, PointXYZ *result) {
	float8 sin_phi, cos_phi, N;
	PointXYZ *ret = result ? result : (PointXYZ *) palloc(sizeof(*ret));

	sin_phi = sin(point->phi);
	cos_phi = cos(point->phi);

	N = WGS84.a / sqrt(1.0 - WGS84.e1stSq * pow(sin_phi, 2.0));

	ret->x = (N + point->h) * cos_phi * cos(point->lambda);
	ret->y = (N + point->h) * cos_phi * sin(point->lambda);
	ret->z = (N * (1.0 - WGS84.e1stSq) + point->h) * sin_phi;

	return ret;
}

PointPLH *xyz2plh(const PointXYZ *point, PointPLH *result) {
	float8 p, q, N;
	PointPLH *ret = result ? result : (PointPLH *) palloc(sizeof(*ret));

	p = sqrt(pow(point->x, 2.0) + pow(point->y, 2.0));
	q = atan2(point->z * WGS84.a, p * WGS84.b);

	ret->phi = atan2(point->z + WGS84.e2ndSq * WGS84.b * pow(sin(q), 3.0), p - WGS84.e1stSq * WGS84.a * pow(cos(q), 3.0));
	ret->lambda = atan2(point->y, point->x);

	N = WGS84.a / sqrt(1.0 - WGS84.e1stSq * pow(sin(ret->phi), 2.0));
	ret->h = (p / cos(ret->phi)) - N;

	return ret;
}

PointENU *xyz2enu(const PointXYZ *point, const PointPLH *lrp, PointENU *result) {
	PointXYZ r_xyz;
	PointENU *ret = result ? result : (PointENU *) palloc(sizeof(*ret));

	plh2xyz(lrp, &r_xyz);

	ret->e = -(point->x - r_xyz.x) * sin(lrp->lambda) + (point->y - r_xyz.y) * cos(lrp->lambda);
	ret->n = -(point->x - r_xyz.x) * sin(lrp->phi) * cos(lrp->lambda) - (point->y - r_xyz.y) * sin(lrp->phi) * sin(lrp->lambda) + (point->z - r_xyz.z) * cos(lrp->phi);
	ret->u = (point->x - r_xyz.x) * cos(lrp->phi) * cos(lrp->lambda) + (point->y - r_xyz.y) * cos(lrp->phi) * sin(lrp->lambda) + (point->z - r_xyz.z) * sin(lrp->phi);

	return ret;
}

PointXYZ *enu2xyz(const PointENU *point, const PointPLH *lrp, PointXYZ *result) {
	PointXYZ r_xyz;
	PointXYZ *ret = result ? result : (PointXYZ *) palloc(sizeof(*ret));

	plh2xyz(lrp, &r_xyz);

	ret->x = r_xyz.x - point->e * sin(lrp->lambda) - point->n * sin(lrp->phi) * cos(lrp->lambda) + point->u * cos(lrp->phi) * cos(lrp->lambda);
	ret->y = r_xyz.y + point->e * cos(lrp->lambda) - point->n * sin(lrp->phi) * sin(lrp->lambda) + point->u * cos(lrp->phi) * sin(lrp->lambda);
	ret->z = r_xyz.z + point->n * cos(lrp->phi) + point->u * sin(lrp->phi);

	return ret;
}

PointENU *plh2enu(const PointPLH *point, const PointPLH *lrp, PointENU *result) {
	PointXYZ xyz;

	plh2xyz(point, &xyz);

	return xyz2enu(&xyz, lrp, result);
}

PointPLH *enu2plh(const PointENU *point, const PointPLH *lrp, PointPLH *result) {
	PointXYZ xyz;

	enu2xyz(point, lrp, &xyz);

	return xyz2plh(&xyz, result);
}

PointXY *ll2xy(const PointLL *point, const PointLL *lrp, PointXY *result) {
	PointPLH e_tmp, r_tmp;
	PointENU enu;
	PointXY *ret = result ? result : (PointXY *) palloc(sizeof(*ret));

	e_tmp.phi = degrees2radians(point->lat);
	e_tmp.lambda = degrees2radians(point->lon);
	e_tmp.h = 0.0;

	r_tmp.phi = degrees2radians(lrp->lat);
	r_tmp.lambda = degrees2radians(lrp->lon);
	r_tmp.h = 0.0;

	plh2enu(&e_tmp, &r_tmp, &enu);

	ret->x = enu.e;
	ret->y = enu.n;

	return ret;
}

PointLL *xy2ll(const PointXY *point, const PointLL *lrp, PointLL *result) {
	PointENU enu;
	PointPLH plh, r_tmp;
	PointLL *ret = result ? result : (PointLL *) palloc(sizeof(*ret));

	enu.e = point->x;
	enu.n = point->y;
	enu.u = 0.0;

	r_tmp.phi = degrees2radians(lrp->lat);
	r_tmp.lambda = degrees2radians(lrp->lon);
	r_tmp.h = 0.0;

	do {
		enu2plh(&enu, &r_tmp, &plh);

		if (plh.h < spatialTolerance) {
			break;
		}

		enu.u = enu.u - plh.h;
	} while (true );

	ret->lon = radians2degrees(plh.lambda);
	ret->lat = radians2degrees(plh.phi);

	return ret;
}

PGDLLEXPORT Datum plh2xyzV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(plh2xyz((PointPLH *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(plh2xyzV1);

PGDLLEXPORT Datum xyz2plhV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(xyz2plh((PointXYZ *) PG_GETARG_POINTER(0), NULL));
}
PG_FUNCTION_INFO_V1(xyz2plhV1);

PGDLLEXPORT Datum xyz2enuV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(xyz2enu((PointXYZ *) PG_GETARG_POINTER(0), (PointPLH *) PG_GETARG_POINTER(1), NULL));
}
PG_FUNCTION_INFO_V1(xyz2enuV1);

PGDLLEXPORT Datum enu2xyzV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(enu2xyz((PointENU *) PG_GETARG_POINTER(0), (PointPLH *) PG_GETARG_POINTER(1), NULL));
}
PG_FUNCTION_INFO_V1(enu2xyzV1);

PGDLLEXPORT Datum plh2enuV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(plh2enu((PointPLH *) PG_GETARG_POINTER(0), (PointPLH *) PG_GETARG_POINTER(1), NULL));
}
PG_FUNCTION_INFO_V1(plh2enuV1);

PGDLLEXPORT Datum enu2plhV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(enu2plh((PointENU *) PG_GETARG_POINTER(0), (PointPLH *) PG_GETARG_POINTER(1), NULL));
}
PG_FUNCTION_INFO_V1(enu2plhV1);

PGDLLEXPORT Datum ll2xyV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(ll2xy((PointLL *) PG_GETARG_POINTER(0), (PointLL *) PG_GETARG_POINTER(1), NULL));
}
PG_FUNCTION_INFO_V1(ll2xyV1);

PGDLLEXPORT Datum xy2llV1(PG_FUNCTION_ARGS) {
	PG_RETURN_POINTER(xy2ll((PointXY *) PG_GETARG_POINTER(0), (PointLL *) PG_GETARG_POINTER(1), NULL));
}
PG_FUNCTION_INFO_V1(xy2llV1);
