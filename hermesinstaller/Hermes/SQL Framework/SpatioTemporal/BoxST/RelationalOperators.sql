/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION intersects(boxA BoxST, boxB BoxST) RETURNS boolean AS
	'$libdir/Hermes','intersectsBoxST_BoxSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR && (
	PROCEDURE = intersects,
	LEFTARG = BoxST,
	RIGHTARG = BoxST
);

/******************************************************************************/
CREATE FUNCTION contains(box BoxST, point PointST) RETURNS boolean AS
	'$libdir/Hermes','containsBoxST_PointSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION contains(boxA BoxST, boxB BoxST) RETURNS boolean AS
	'$libdir/Hermes','containsBoxST_BoxSTV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION containsProperly(box BoxST, point PointST) RETURNS boolean AS
	'$libdir/Hermes','containsProperlyBoxST_PointSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION containsProperly(boxA BoxST, boxB BoxST) RETURNS boolean AS
	'$libdir/Hermes','containsProperlyBoxST_BoxSTV1'
LANGUAGE C IMMUTABLE STRICT;
