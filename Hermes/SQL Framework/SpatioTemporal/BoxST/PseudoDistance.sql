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
