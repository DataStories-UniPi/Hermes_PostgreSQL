/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION PointXYZ(point PointPLH) RETURNS PointXYZ AS
	'$libdir/Hermes','plh2xyzV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION PointPLH(point PointXYZ) RETURNS PointPLH AS
	'$libdir/Hermes','xyz2plhV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION PointENU(point PointXYZ, LRP PointPLH) RETURNS PointENU AS
	'$libdir/Hermes','xyz2enuV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION PointXYZ(point PointENU, LRP PointPLH) RETURNS PointXYZ AS
	'$libdir/Hermes','enu2xyzV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION PointENU(point PointPLH, LRP PointPLH) RETURNS PointENU AS
	'$libdir/Hermes','plh2enuV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION PointPLH(point PointENU, LRP PointPLH) RETURNS PointPLH AS
	'$libdir/Hermes','enu2plhV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION PointXY(point PointLL, LRP PointLL) RETURNS PointXY AS
	'$libdir/Hermes','ll2xyV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION PointLL(point PointXY, LRP PointLL) RETURNS PointLL AS
	'$libdir/Hermes','xy2llV1'
LANGUAGE C STABLE STRICT;
