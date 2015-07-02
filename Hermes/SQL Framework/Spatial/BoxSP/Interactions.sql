/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief The implementantion in sql of BoxSP.
 *
 * @details BoxSP is a spatial data type used in Hermes
 *
 * @see @ref data_types_spatial
 *
 */

/******************************************************************************/
CREATE FUNCTION intersection(boxA BoxSP, boxB BoxSP) RETURNS BoxSP AS
	'$libdir/Hermes','intersectionBoxSP_BoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION HUnion(box BoxSP, point PointSP) RETURNS BoxSP AS
	'$libdir/Hermes','unionBoxSP_PointSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION HUnion(box BoxSP, segment SegmentSP) RETURNS BoxSP AS
	'$libdir/Hermes','unionBoxSP_SegmentSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION HUnion(boxA BoxSP, boxB BoxSP) RETURNS BoxSP AS
	'$libdir/Hermes','unionBoxSP_BoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE AGGREGATE HUnion(BoxSP) (
	SFUNC = HUnion(BoxSP, BoxSP),
	STYPE = BoxSP
);
