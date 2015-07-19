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
CREATE FUNCTION distance(box BoxSP, point PointSP) RETURNS double precision AS
	'$libdir/Hermes','distanceBoxSP_PointSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION distance(box BoxSP, segment SegmentSP) RETURNS double precision AS
	'$libdir/Hermes','distanceBoxSP_SegmentSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION distance(boxA BoxSP, boxB BoxSP) RETURNS double precision AS
	'$libdir/Hermes','distanceBoxSP_BoxSPV1'
LANGUAGE C IMMUTABLE STRICT;
