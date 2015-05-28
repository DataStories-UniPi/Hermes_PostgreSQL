/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "../../../h/Spatial/CRS/PointLL/Distance.h"
#include "../../../h/Spatial/CRS/Ellipsoid/Definition.h"
#include "../../../h/Utilities/Utilities.h"
#include <math.h>

float8 distanceSpherePointLL_PointLL(const PointLL *pointA, const PointLL *pointB) {
	float8 rad_lat_a = degrees2radians(pointA->lat);
	float8 rad_lat_b = degrees2radians(pointB->lat);

	float8 d_lon = degrees2radians(pointB->lon) - degrees2radians(pointA->lon);
	float8 cos_d_lon = cos(d_lon);
	float8 cos_lat_b = cos(rad_lat_b);
	float8 sin_lat_b = sin(rad_lat_b);
	float8 cos_lat_a = cos(rad_lat_a);
	float8 sin_lat_a = sin(rad_lat_a);

	float8 a1 = pow(cos_lat_b * sin(d_lon), 2.0);
	float8 a2 = pow(cos_lat_a * sin_lat_b - sin_lat_a * cos_lat_b * cos_d_lon, 2.0);
	float8 a = sqrt(a1 + a2);
	float8 b = sin_lat_a * sin_lat_b + cos_lat_a * cos_lat_b * cos_d_lon;

	return WGS84.r1 * atan2(a, b);
}

PGDLLEXPORT Datum distanceSpherePointLL_PointLLV1(PG_FUNCTION_ARGS) {
	PG_RETURN_FLOAT8(distanceSpherePointLL_PointLL((PointLL *) PG_GETARG_POINTER(0), (PointLL *) PG_GETARG_POINTER(1)));
}
PG_FUNCTION_INFO_V1(distanceSpherePointLL_PointLLV1);
