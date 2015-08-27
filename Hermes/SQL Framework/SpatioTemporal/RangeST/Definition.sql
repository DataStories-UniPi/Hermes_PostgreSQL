/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE TYPE RangeST;

CREATE FUNCTION inRangeST(cstring) RETURNS RangeST AS
	'$libdir/Hermes','inRangeSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION outRangeST(RangeST) RETURNS cstring AS
	'$libdir/Hermes','outRangeSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION recvRangeST(internal) RETURNS RangeST AS
	'$libdir/Hermes','recvRangeST'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION send(RangeST) RETURNS bytea AS
	'$libdir/Hermes','sendRangeST'
LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE RangeST (
	INPUT = inRangeST,
	OUTPUT = outRangeST,
	RECEIVE = recvRangeST,
	SEND = send,

	INTERNALLENGTH = 40,
	ALIGNMENT = double
);

/******************************************************************************/
CREATE FUNCTION equals(rangeA RangeST, rangeB RangeST) RETURNS boolean AS
	'$libdir/Hermes','equalsRangeSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION hash(range RangeST) RETURNS integer AS
	'$libdir/Hermes','hashRangeST'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR = (
	PROCEDURE = equals,
	LEFTARG = RangeST,
	RIGHTARG = RangeST,

	COMMUTATOR = =,
	HASHES
);

CREATE OPERATOR CLASS RangeST_OPS DEFAULT FOR TYPE RangeST USING hash AS
	OPERATOR 1 =,
	FUNCTION 1 hash(RangeST);

/** @brief The function returns the RangeT of the RangeST
 *
 *  @param[in] range The RangeST
 *
 *	@return the RangeT
 *
 *	@see @ref methods_get
 *
 */
CREATE FUNCTION getT(range RangeST) RETURNS RangeT AS
	'$libdir/Hermes','getTRangeST'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the interval of the RangeST
 *
 *  @param[in] range The RangeST
 *
 *	@return the interval
 *
 *	@see @ref methods_get
 *
 */
CREATE FUNCTION getTr(range RangeST) RETURNS interval AS
	'$libdir/Hermes','getTrRangeST'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the timestamp of the RangeST
 *
 *  @param[in] range The RangeST
 *
 *	@return the interval
 *
 *	@see @ref methods_get
 *
 */
CREATE FUNCTION getTc(range RangeST) RETURNS timestamp AS
	'$libdir/Hermes','getTcRangeST'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the spatial range of the RangeST
 *
 *  @param[in] range The RangeST
 *
 *	@return the RangeSP
 *
 *	@see @ref methods_get
 *
 */
CREATE FUNCTION getSp(range RangeST) RETURNS RangeSP AS
	'$libdir/Hermes','getSpRangeST'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the radius of the spatial 
 * 	range of the RangeST
 *
 *  @param[in] range The RangeST
 *
 *	@return the RangeSP
 *
 *	@see @ref methods_get
 *
 */
CREATE FUNCTION getSpr(range RangeST) RETURNS integer AS
	'$libdir/Hermes','getSprRangeST'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the center of the spatial 
 * 	range of the RangeST
 *
 *  @param[in] range The RangeST
 *
 *	@return the RangeSP
 *
 *	@see @ref methods_get
 *
 */
CREATE FUNCTION getSpc(range RangeST) RETURNS PointSP AS
	'$libdir/Hermes','getSpcRangeST'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the x coordinate of the center 
 * 	 of the spatial range of the RangeST
 *
 *  @param[in] range The RangeST
 *
 *	@return the RangeSP
 *
 *	@see @ref methods_get
 *
 */
CREATE FUNCTION getCx(range RangeST) RETURNS integer AS
	'$libdir/Hermes','getCxRangeST'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the y coordinate of the center 
 * 	 of the spatial range of the RangeST
 *
 *  @param[in] range The RangeST
 *
 *	@return the RangeSP
 *
 *	@see @ref methods_get
 *
 */
CREATE FUNCTION getCy(range RangeST) RETURNS integer AS
	'$libdir/Hermes','getCyRangeST'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns a RangeST data type
 *
 *	@param[in] tr the interval of the RangeST
 *	@param[in] tc the initial timestamp
 *	@param[in] spr the radius of the range
 *	@param[in] cx the x coordinate of the center
 *	@param[in] cy the y coordinate of the center
 *
 *  @return a RangeST data type
 */
CREATE FUNCTION RangeST(tr interval, tc timestamp, spr integer, cx integer, cy integer) RETURNS RangeST AS
	'$libdir/Hermes','constructorRangeSTV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns a RangeST data type
 *
 *	@param[in] t the RangeT of the RangeST
 *	@param[in] sp the spatial range of the RangeST
 *
 *  @return a RangeST data type
 */
CREATE FUNCTION RangeST(t RangeT, sp RangeSP) RETURNS RangeST AS
	'$libdir/Hermes','constructorHighRangeSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION clone(range RangeST) RETURNS RangeST AS
	'$libdir/Hermes','cloneRangeSTV1'
LANGUAGE C IMMUTABLE STRICT;
