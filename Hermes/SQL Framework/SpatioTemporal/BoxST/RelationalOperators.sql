/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief The implementantion in sql of BoxST.
 *
 * @details BoxST is a spatial data type used in Hermes
 *
 * @see @ref data_types_spatio_temp
 *
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
