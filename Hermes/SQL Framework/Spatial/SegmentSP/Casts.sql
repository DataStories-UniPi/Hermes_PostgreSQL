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
CREATE FUNCTION BoxSP(segment SegmentSP) RETURNS BoxSP AS
	'$libdir/Hermes','SegmentSP2BoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE CAST (SegmentSP AS BoxSP) WITH FUNCTION BoxSP(SegmentSP) AS ASSIGNMENT;

/******************************************************************************/
CREATE FUNCTION LineSP(segment SegmentSP) RETURNS LineSP AS
	'$libdir/Hermes','SegmentSP2LineSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE CAST (SegmentSP AS LineSP) WITH FUNCTION LineSP(SegmentSP) AS ASSIGNMENT;
