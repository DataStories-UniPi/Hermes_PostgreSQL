/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief The implementantion in sql of LineSP.
 *
 * @details LineSP is a spatial data type used in Hermes
 *
 * @see @ref data_types_spatial
 *
 */

/*
 * General Form of the line equation: A*x + B*y + C = 0
 */

/******************************************************************************/
CREATE TYPE LineSP;

CREATE FUNCTION inLineSP(cstring) RETURNS LineSP AS
	'$libdir/Hermes','inLineSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION outLineSP(LineSP) RETURNS cstring AS
	'$libdir/Hermes','outLineSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION recvLineSP(internal) RETURNS LineSP AS
	'$libdir/Hermes','recvLineSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION send(LineSP) RETURNS bytea AS
	'$libdir/Hermes','sendLineSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE LineSP (
	INPUT = inLineSP,
	OUTPUT = outLineSP,
	RECEIVE = recvLineSP,
	SEND = send,

	INTERNALLENGTH = 24,
	ALIGNMENT = double,
	ELEMENT = double precision
);

/******************************************************************************/
CREATE FUNCTION equals(lineA LineSP, lineB LineSP) RETURNS boolean AS
	'$libdir/Hermes','equalsLineSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION hash(line LineSP) RETURNS integer AS
	'$libdir/Hermes','hashLineSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR = (
	PROCEDURE = equals,
	LEFTARG = LineSP,
	RIGHTARG = LineSP,

	COMMUTATOR = =,
	HASHES
);

CREATE OPERATOR CLASS LineSP_OPS DEFAULT FOR TYPE LineSP USING hash AS
	OPERATOR 1 =,
	FUNCTION 1 hash(LineSP);

/** @brief The function returns the A parameter of the line
 *
 *  @param[in] line The LineSP
 *
 *	@return the A parameter
 *
 *	@see @ref methods_get
 *
 */
CREATE FUNCTION getA(line LineSP) RETURNS double precision AS
	'$libdir/Hermes','getALineSP'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the B parameter of the line
 *
 *  @param[in] line The LineSP
 *
 *	@return the B parameter
 *
 *	@see @ref methods_get
 *
 */
CREATE FUNCTION getB(line LineSP) RETURNS double precision AS
	'$libdir/Hermes','getBLineSP'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the C parameter of the line
 *
 *  @param[in] line The LineSP
 *
 *	@return the C parameter
 *
 *	@see @ref methods_get
 *
 */
CREATE FUNCTION getC(line LineSP) RETURNS double precision AS
	'$libdir/Hermes','getCLineSP'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function creates a LineSP from 3 double precision numbers
 * 
 *  @param[in] A the A parameter 
 *  @param[in] B the B parameter 
 * 	@param[in] C the C parameter 
 *
 *  @return LineSP
 */
CREATE FUNCTION LineSP(A double precision, B double precision, C double precision) RETURNS LineSP AS
	'$libdir/Hermes','constructorLineSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION clone(line LineSP) RETURNS LineSP AS
	'$libdir/Hermes','cloneLineSPV1'
LANGUAGE C IMMUTABLE STRICT;
