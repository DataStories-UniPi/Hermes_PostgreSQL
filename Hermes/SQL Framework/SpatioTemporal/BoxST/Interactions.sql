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
CREATE FUNCTION intersection(boxA BoxST, boxB BoxST) RETURNS BoxST AS
	'$libdir/Hermes','intersectionBoxST_BoxSTV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION HUnion(box BoxST, point PointST) RETURNS BoxST AS
	'$libdir/Hermes','unionBoxST_PointSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION HUnion(box BoxST, segment SegmentST) RETURNS BoxST AS
	'$libdir/Hermes','unionBoxST_SegmentSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION HUnion(boxA BoxST, boxB BoxST) RETURNS BoxST AS
	'$libdir/Hermes','unionBoxST_BoxSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE AGGREGATE HUnion(BoxST) (
	SFUNC = HUnion(BoxST, BoxST),
	STYPE = BoxST
);
