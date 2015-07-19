/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief The implementantion in sql of SegmentSP.
 *
 * @details SegmentSP is a spatial data type used in Hermes
 *
 * @see @ref data_types_spatial
 *
 */

/******************************************************************************/
CREATE FUNCTION resize(segment SegmentSP, nlength double precision) RETURNS SegmentSP AS
	'$libdir/Hermes','resizeSegmentSPV1'
LANGUAGE C IMMUTABLE STRICT;
