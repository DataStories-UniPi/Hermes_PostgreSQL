/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief The implementantion in sql of RangeSP.
 *
 * @details RangeSP is a spatial data type used in Hermes
 *
 * @see @ref data_types_spatial
 *
 */

/******************************************************************************/
CREATE TYPE RangeSP;

CREATE FUNCTION inRangeSP(cstring) RETURNS RangeSP AS
	'$libdir/Hermes','inRangeSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION outRangeSP(RangeSP) RETURNS cstring AS
	'$libdir/Hermes','outRangeSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION recvRangeSP(internal) RETURNS RangeSP AS
	'$libdir/Hermes','recvRangeSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION send(RangeSP) RETURNS bytea AS
	'$libdir/Hermes','sendRangeSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE RangeSP (
	INPUT = inRangeSP,
	OUTPUT = outRangeSP,
	RECEIVE = recvRangeSP,
	SEND = send,

	INTERNALLENGTH = 12,
	ALIGNMENT = int4,
	ELEMENT = integer
);

/******************************************************************************/
CREATE FUNCTION equals(rangeA RangeSP, rangeB RangeSP) RETURNS boolean AS
	'$libdir/Hermes','equalsRangeSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION hash(range RangeSP) RETURNS integer AS
	'$libdir/Hermes','hashRangeSP'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR = (
	PROCEDURE = equals,
	LEFTARG = RangeSP,
	RIGHTARG = RangeSP,

	COMMUTATOR = =,
	HASHES
);

CREATE OPERATOR CLASS RangeSP_OPS DEFAULT FOR TYPE RangeSP USING hash AS
	OPERATOR 1 =,
	FUNCTION 1 hash(RangeSP);

/** @brief The function returns the radius of the RangeSP
 *
 *  @param[in] range The RangeSP
 *
 *	@return the radius
 *
 *	@see @ref methods_get
 *
 */
CREATE FUNCTION getR(range RangeSP) RETURNS integer AS
	'$libdir/Hermes','getRRangeSP'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the center of the RangeSP
 *
 *  @param[in] range The RangeSP
 *
 *	@return the center
 *
 *	@see @ref methods_get
 *
 */
CREATE FUNCTION getC(range RangeSP) RETURNS PointSP AS
	'$libdir/Hermes','getCRangeSP'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the x coordinate of the center of the RangeSP
 *
 *  @param[in] range The RangeSP
 *
 *	@return the coordinate
 *
 *	@see @ref methods_get
 *
 */
CREATE FUNCTION getX(range RangeSP) RETURNS integer AS
	'$libdir/Hermes','getXRangeSP'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the y coordinate of the center of the RangeSP
 *
 *  @param[in] range The RangeSP
 *
 *	@return the coordinate
 *
 *	@see @ref methods_get
 *
 */
CREATE FUNCTION getY(range RangeSP) RETURNS integer AS
	'$libdir/Hermes','getYRangeSP'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION RangeSP(r integer, cx integer, cy integer) RETURNS RangeSP AS
	'$libdir/Hermes','constructorRangeSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION RangeSP(r integer, c PointSP) RETURNS RangeSP AS
	'$libdir/Hermes','constructorHighRangeSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION clone(range RangeSP) RETURNS RangeSP AS
	'$libdir/Hermes','cloneRangeSPV1'
LANGUAGE C IMMUTABLE STRICT;
