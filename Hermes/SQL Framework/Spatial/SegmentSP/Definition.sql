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
CREATE TYPE SegmentSP;

CREATE FUNCTION inSegmentSP(cstring) RETURNS SegmentSP AS
	'$libdir/Hermes','inSegmentSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION outSegmentSP(SegmentSP) RETURNS cstring AS
	'$libdir/Hermes','outSegmentSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION recvSegmentSP(internal) RETURNS SegmentSP AS
	'$libdir/Hermes','recvSegmentSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION send(SegmentSP) RETURNS bytea AS
	'$libdir/Hermes','sendSegmentSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE SegmentSP (
	INPUT = inSegmentSP,
	OUTPUT = outSegmentSP,
	RECEIVE = recvSegmentSP,
	SEND = send,

	INTERNALLENGTH = 16,
	ALIGNMENT = int4,
	ELEMENT = PointSP
);

/******************************************************************************/
CREATE FUNCTION equals(segmentA SegmentSP, segmentB SegmentSP) RETURNS boolean AS
	'$libdir/Hermes','equalsSegmentSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION hash(segment SegmentSP) RETURNS integer AS
	'$libdir/Hermes','hashSegmentSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR = (
	PROCEDURE = equals,
	LEFTARG = SegmentSP,
	RIGHTARG = SegmentSP,

	COMMUTATOR = =,
	HASHES
);

CREATE OPERATOR CLASS SegmentSP_OPS DEFAULT FOR TYPE SegmentSP USING hash AS
	OPERATOR 1 =,
	FUNCTION 1 hash(SegmentSP);

/** @brief The function returns the start of the SegmentSP
 *
 *  @param[in] segment The SegmentSP
 *
 *	@return start of the SegmentSP
 *
 *	@see @ref methods_get
 *
 */
CREATE FUNCTION getI(segment SegmentSP) RETURNS PointSP AS
	'$libdir/Hermes','getISegmentSP'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the x coordinate of the 
 *	start of the SegmentSP
 *
 *  @param[in] segment The SegmentSP
 *
 *	@return the coordinate
 *
 *	@see @ref methods_get
 *
 */
CREATE FUNCTION getIx(segment SegmentSP) RETURNS integer AS
	'$libdir/Hermes','getIxSegmentSP'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the y coordinate of the 
 *	start of the SegmentSP
 *
 *  @param[in] segment The SegmentSP
 *
 *	@return the coordinate
 *
 *	@see @ref methods_get
 *
 */
CREATE FUNCTION getIy(segment SegmentSP) RETURNS integer AS
	'$libdir/Hermes','getIySegmentSP'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the end of the SegmentSP
 *
 *  @param[in] segment The SegmentSP
 *
 *	@return end of the SegmentSP
 *
 *	@see @ref methods_get
 *
 */
CREATE FUNCTION getE(segment SegmentSP) RETURNS PointSP AS
	'$libdir/Hermes','getESegmentSP'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the x coordinate of the 
 *	end of the SegmentSP
 *
 *  @param[in] segment The SegmentSP
 *
 *	@return the coordinate
 *
 *	@see @ref methods_get
 *
 */
CREATE FUNCTION getEx(segment SegmentSP) RETURNS integer AS
	'$libdir/Hermes','getExSegmentSP'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the y coordinate of the 
 *	end of the SegmentSP
 *
 *  @param[in] segment The SegmentSP
 *
 *	@return the coordinate
 *
 *	@see @ref methods_get
 *
 */
CREATE FUNCTION getEy(segment SegmentSP) RETURNS integer AS
	'$libdir/Hermes','getEySegmentSP'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function creates a SegmentSP from 4 integers
 * 
 *  @param[in] ix the x coordinate of the start point 
 *  @param[in] iy the y coordinate of the start point 
 *	@param[in] ex the x coordinate of the end point
 *	@param[in] ey the x coordinate of the end point	
 *
 *  @return SegmentSP
 */
CREATE FUNCTION SegmentSP(ix integer, iy integer, ex integer, ey integer) RETURNS SegmentSP AS
	'$libdir/Hermes','constructorSegmentSPV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function creates a SegmentSP from 2 PointSP
 * 
 *  @param[in] i the start point 
 *  @param[in] e the end point 
 *
 *  @return SegmentSP
 */
CREATE FUNCTION SegmentSP(i PointSP, e PointSP) RETURNS SegmentSP AS
	'$libdir/Hermes','constructorHighSegmentSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION clone(segment SegmentSP) RETURNS SegmentSP AS
	'$libdir/Hermes','cloneSegmentSPV1'
LANGUAGE C IMMUTABLE STRICT;
