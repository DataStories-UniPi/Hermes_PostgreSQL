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
CREATE TYPE BoxSP;

/** @brief The function transforms a string to 
 * 	a BoxSP type
 * 
 *  @param[in] cstring the box in string format
 *
 *  @return BoxSP
 */
CREATE FUNCTION inBoxSP(cstring) RETURNS BoxSP AS
	'$libdir/Hermes','inBoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function transforms a BoxSP to 
 * 	a string
 * 
 *  @param[in] BoxSP the input box
 *
 *  @return the string of the box
 */
CREATE FUNCTION outBoxSP(BoxSP) RETURNS cstring AS
	'$libdir/Hermes','outBoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION recvBoxSP(internal) RETURNS BoxSP AS
	'$libdir/Hermes','recvBoxSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION send(BoxSP) RETURNS bytea AS
	'$libdir/Hermes','sendBoxSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE BoxSP (
	INPUT = inBoxSP,
	OUTPUT = outBoxSP,
	RECEIVE = recvBoxSP,
	SEND = send,

	INTERNALLENGTH = 16,
	ALIGNMENT = int4,
	ELEMENT = PointSP
);

/** @brief The function checks if two boxs are equal
 * 
 *  @param[in] boxA the first input box
 * 	@param[in] boxB the second input box
 *
 *  @return truth if the boxs are equal else false
 */
CREATE FUNCTION equals(boxA BoxSP, boxB BoxSP) RETURNS boolean AS
	'$libdir/Hermes','equalsBoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION hash(box BoxSP) RETURNS integer AS
	'$libdir/Hermes','hashBoxSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR = (
	PROCEDURE = equals,
	LEFTARG = BoxSP,
	RIGHTARG = BoxSP,

	COMMUTATOR = =,
	HASHES
);

CREATE OPERATOR CLASS BoxSP_OPS DEFAULT FOR TYPE BoxSP USING hash AS
	OPERATOR 1 =,
	FUNCTION 1 hash(BoxSP);

/** @brief The function returns the low left point of the BoxSP
 *
 *  @param[in] box The BoxSP
 *
 *	@return the point
 *
 *	@see @ref methods_get
 *
 */
CREATE FUNCTION getL(box BoxSP) RETURNS PointSP AS
	'$libdir/Hermes','getLBoxSP'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the x coordinate of the
 *	low left point of the BoxSP
 *
 *  @param[in] box The BoxSP
 *
 *	@return the coordinate
 *
 *	@see @ref methods_get
 *
 */
CREATE FUNCTION getLx(box BoxSP) RETURNS integer AS
	'$libdir/Hermes','getLxBoxSP'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the y coordinate of the
 *	low left point of the BoxSP
 *
 *  @param[in] box The BoxSP
 *
 *	@return the coordinate
 *
 *	@see @ref methods_get
 *
 */
CREATE FUNCTION getLy(box BoxSP) RETURNS integer AS
	'$libdir/Hermes','getLyBoxSP'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the high right point of the BoxSP
 *
 *  @param[in] box The BoxSP
 *
 *	@return the point
 *
 *	@see @ref methods_get
 *
 */
CREATE FUNCTION getH(box BoxSP) RETURNS PointSP AS
	'$libdir/Hermes','getHBoxSP'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the x coordinate of the
 *	high right point of the BoxSP
 *
 *  @param[in] box The BoxSP
 *
 *	@return the coordinate
 *
 *	@see @ref methods_get
 *
 */
CREATE FUNCTION getHx(box BoxSP) RETURNS integer AS
	'$libdir/Hermes','getHxBoxSP'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the y coordinate of the
 *	high right point of the BoxSP
 *
 *  @param[in] box The BoxSP
 *
 *	@return the coordinate
 *
 *	@see @ref methods_get
 *
 */
CREATE FUNCTION getHy(box BoxSP) RETURNS integer AS
	'$libdir/Hermes','getHyBoxSP'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function creates a PointSP from 4 integers
 * 
 *  @param[in] lx the x coordinate of the low left point 
 *  @param[in] ly the y coordinate of the low left point 
 *	@param[in] hx the x coordinate of the high right point
 *	@param[in] hy the x coordinate of the high right point	
 *
 *  @return BoxSP
 */
CREATE FUNCTION BoxSP(lx integer, ly integer, hx integer, hy integer) RETURNS BoxSP AS
	'$libdir/Hermes','constructorBoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function creates a BoxSP from two PointSP
 * 
 *  @param[in] l the low left point 
 *  @param[in] h the high right point 
 *
 *  @return BoxSP
 */
CREATE FUNCTION BoxSP(l PointSP, h PointSP) RETURNS BoxSP AS
	'$libdir/Hermes','constructorHighBoxSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION clone(box BoxSP) RETURNS BoxSP AS
	'$libdir/Hermes','cloneBoxSPV1'
LANGUAGE C IMMUTABLE STRICT;
