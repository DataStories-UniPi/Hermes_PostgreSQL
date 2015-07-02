/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief The implementantion in sql of PointSP.
 *
 * @details PointSP is a spatial data type used in Hermes
 *
 * @see @ref data_types_spatial
 *
 */

/******************************************************************************/
CREATE TYPE PointSP;

/** @brief The function transforms a string to 
 * 	a PointSP type
 * 
 *  @param[in] cstring the point in string format
 *
 *  @return PointSP
 */
CREATE FUNCTION inPointSP(cstring) RETURNS PointSP AS
	'$libdir/Hermes','inPointSPV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function transforms a PointSP to 
 * 	a string
 * 
 *  @param[in] PointSP the input point
 *
 *  @return the string of the point
 */
CREATE FUNCTION outPointSP(PointSP) RETURNS cstring AS
	'$libdir/Hermes','outPointSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION recvPointSP(internal) RETURNS PointSP AS
	'$libdir/Hermes','recvPointSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION send(PointSP) RETURNS bytea AS
	'$libdir/Hermes','sendPointSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE PointSP (
	INPUT = inPointSP,
	OUTPUT = outPointSP,
	RECEIVE = recvPointSP,
	SEND = send,

	INTERNALLENGTH = 8,
	ALIGNMENT = int4,
	ELEMENT = integer
);

/** @brief The function checks if two points are equal
 * 
 *  @param[in] pointA the first input point
 * 	@param[in] pointB the second input point
 *
 *  @return truth if the points are equal else false
 */
CREATE FUNCTION equals(pointA PointSP, pointB PointSP) RETURNS boolean AS
	'$libdir/Hermes','equalsPointSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION hash(point PointSP) RETURNS integer AS
	'$libdir/Hermes','hashPointSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR = (
	PROCEDURE = equals,
	LEFTARG = PointSP,
	RIGHTARG = PointSP,

	COMMUTATOR = =,
	HASHES
);

CREATE OPERATOR CLASS PointSP_OPS DEFAULT FOR TYPE PointSP USING hash AS
	OPERATOR 1 =,
	FUNCTION 1 hash(PointSP);

/******************************************************************************/
CREATE FUNCTION getX(point PointSP) RETURNS integer AS
	'$libdir/Hermes','getXPointSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION getY(point PointSP) RETURNS integer AS
	'$libdir/Hermes','getYPointSP'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function creates a PointSP from two integers
 * 
 *  @param[in] x the x coordinate of the point 
 *  @param[in] y the y coordinate of the point 
 *
 *  @return PointSP
 */
CREATE FUNCTION PointSP(x integer, y integer) RETURNS PointSP AS
	'$libdir/Hermes','constructorPointSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION clone(point PointSP) RETURNS PointSP AS
	'$libdir/Hermes','clonePointSPV1'
LANGUAGE C IMMUTABLE STRICT;
