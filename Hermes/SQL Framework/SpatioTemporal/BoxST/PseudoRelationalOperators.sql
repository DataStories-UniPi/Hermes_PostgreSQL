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
