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
CREATE FUNCTION intersects(boxA BoxSP, boxB BoxSP) RETURNS boolean AS
	'$libdir/Hermes','intersectsBoxSP_BoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION contains(box BoxSP, point PointSP) RETURNS boolean AS
	'$libdir/Hermes','containsBoxSP_PointSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION contains(boxA BoxSP, boxB BoxSP) RETURNS boolean AS
	'$libdir/Hermes','containsBoxSP_BoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION containsProperly(box BoxSP, point PointSP) RETURNS boolean AS
	'$libdir/Hermes','containsProperlyBoxSP_PointSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION containsProperly(boxA BoxSP, boxB BoxSP) RETURNS boolean AS
	'$libdir/Hermes','containsProperlyBoxSP_BoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION north(box BoxSP, point PointSP) RETURNS boolean AS
	'$libdir/Hermes','northBoxSP_PointSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION north(boxA BoxSP, boxB BoxSP) RETURNS boolean AS
	'$libdir/Hermes','northBoxSP_BoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION south(box BoxSP, point PointSP) RETURNS boolean AS
	'$libdir/Hermes','southBoxSP_PointSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION south(boxA BoxSP, boxB BoxSP) RETURNS boolean AS
	'$libdir/Hermes','southBoxSP_BoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION east(box BoxSP, point PointSP) RETURNS boolean AS
	'$libdir/Hermes','eastBoxSP_PointSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION east(boxA BoxSP, boxB BoxSP) RETURNS boolean AS
	'$libdir/Hermes','eastBoxSP_BoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION west(box BoxSP, point PointSP) RETURNS boolean AS
	'$libdir/Hermes','westBoxSP_PointSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION west(boxA BoxSP, boxB BoxSP) RETURNS boolean AS
	'$libdir/Hermes','westBoxSP_BoxSPV1'
LANGUAGE C IMMUTABLE STRICT;
