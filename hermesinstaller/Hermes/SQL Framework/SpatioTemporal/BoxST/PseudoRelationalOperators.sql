/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION intersects(box BoxST, period Period) RETURNS boolean AS
	'$libdir/Hermes','intersectsBoxST_PeriodV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION intersects(boxA BoxST, boxB BoxSP) RETURNS boolean AS
	'$libdir/Hermes','intersectsBoxST_BoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR && (
	PROCEDURE = intersects,
	LEFTARG = BoxST,
	RIGHTARG = Period
);

CREATE OPERATOR && (
	PROCEDURE = intersects,
	LEFTARG = BoxST,
	RIGHTARG = BoxSP
);
