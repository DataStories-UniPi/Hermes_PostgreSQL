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
CREATE FUNCTION isPunctal(box BoxSP) RETURNS boolean AS
	'$libdir/Hermes','isPunctalBoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION width(box BoxSP) RETURNS integer AS
	'$libdir/Hermes','widthBoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION height(box BoxSP) RETURNS integer AS
	'$libdir/Hermes','heightBoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION area(box BoxSP) RETURNS bigint AS
	'$libdir/Hermes','areaBoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION center(box BoxSP) RETURNS PointXY AS
	'$libdir/Hermes','centerBoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION centerX(box BoxSP) RETURNS double precision AS
	'$libdir/Hermes','centerXBoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION centerY(box BoxSP) RETURNS double precision AS
	'$libdir/Hermes','centerYBoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION lh(box BoxSP) RETURNS PointSP AS
	'$libdir/Hermes','lhBoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION hl(box BoxSP) RETURNS PointSP AS
	'$libdir/Hermes','hlBoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION l_h(box BoxSP) RETURNS SegmentSP AS
	'$libdir/Hermes','l_hBoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION h_l(box BoxSP) RETURNS SegmentSP AS
	'$libdir/Hermes','h_lBoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION lh_hl(box BoxSP) RETURNS SegmentSP AS
	'$libdir/Hermes','lh_hlBoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION hl_lh(box BoxSP) RETURNS SegmentSP AS
	'$libdir/Hermes','hl_lhBoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION lh_h(box BoxSP) RETURNS SegmentSP AS
	'$libdir/Hermes','lh_hBoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION h_lh(box BoxSP) RETURNS SegmentSP AS
	'$libdir/Hermes','h_lhBoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION h_hl(box BoxSP) RETURNS SegmentSP AS
	'$libdir/Hermes','h_hlBoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION hl_h(box BoxSP) RETURNS SegmentSP AS
	'$libdir/Hermes','hl_hBoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION hl_l(box BoxSP) RETURNS SegmentSP AS
	'$libdir/Hermes','hl_lBoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION l_hl(box BoxSP) RETURNS SegmentSP AS
	'$libdir/Hermes','l_hlBoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION l_lh(box BoxSP) RETURNS SegmentSP AS
	'$libdir/Hermes','l_lhBoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION lh_l(box BoxSP) RETURNS SegmentSP AS
	'$libdir/Hermes','lh_lBoxSPV1'
LANGUAGE C IMMUTABLE STRICT;
