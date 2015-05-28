/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION distance(box BoxST, period Period) RETURNS double precision AS
	'$libdir/Hermes','distanceBoxST_PeriodV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR <-> (
	PROCEDURE = distance,
	LEFTARG = BoxST,
	RIGHTARG = Period
);

/******************************************************************************/
CREATE FUNCTION distance(boxA BoxST, boxB BoxSP) RETURNS double precision AS
	'$libdir/Hermes','distanceBoxST_BoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR <-> (
	PROCEDURE = distance,
	LEFTARG = BoxST,
	RIGHTARG = BoxSP
);
