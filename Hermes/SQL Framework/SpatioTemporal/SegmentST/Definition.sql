/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the SegmentST data type.
 *
 * @see @ref SegmentST
 * @see @ref data_types_spatio_temp
 */

/******************************************************************************/
CREATE TYPE SegmentST;

CREATE FUNCTION inSegmentST(cstring) RETURNS SegmentST AS
	'$libdir/Hermes','inSegmentSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION outSegmentST(SegmentST) RETURNS cstring AS
	'$libdir/Hermes','outSegmentSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION recvSegmentST(internal) RETURNS SegmentST AS
	'$libdir/Hermes','recvSegmentST'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION send(SegmentST) RETURNS bytea AS
	'$libdir/Hermes','sendSegmentST'
LANGUAGE C IMMUTABLE STRICT;


CREATE TYPE SegmentST (
	INPUT = inSegmentST,
	OUTPUT = outSegmentST,
	RECEIVE = recvSegmentST,
	SEND = send,

	INTERNALLENGTH = 32,
	ALIGNMENT = double
);

/******************************************************************************/
CREATE FUNCTION equals(segmentA SegmentST, segmentB SegmentST) RETURNS boolean AS
	'$libdir/Hermes','equalsSegmentSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION hash(segment SegmentST) RETURNS integer AS
	'$libdir/Hermes','hashSegmentST'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR = (
	PROCEDURE = equals,
	LEFTARG = SegmentST,
	RIGHTARG = SegmentST,

	COMMUTATOR = =,
	HASHES
);

CREATE OPERATOR CLASS SegmentST_OPS DEFAULT FOR TYPE SegmentST USING hash AS
	OPERATOR 1 =,
	FUNCTION 1 hash(SegmentST);

/** @brief The function returns the period of the SegmentST
 *
 *  @param[in] segment The SegmentST
 *
 *	@return the period
 *
 *	@see @ref methods_get
 *
 */
 CREATE FUNCTION getT(segment SegmentST) RETURNS Period AS
	'$libdir/Hermes','getTSegmentST'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the start time of the period of the SegmentST
 *
 *  @param[in] segment The SegmentST
 *
 *	@return the start time
 *
 *	@see @ref methods_get
 *
 */
CREATE FUNCTION getTi(segment SegmentST) RETURNS timestamp AS
	'$libdir/Hermes','getTiSegmentST'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the end time of the period of the SegmentST
 *
 *  @param[in] segment The SegmentST
 *
 *	@return the end time
 *
 *	@see @ref methods_get
 *
 */
CREATE FUNCTION getTe(segment SegmentST) RETURNS timestamp AS
	'$libdir/Hermes','getTeSegmentST'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the Spatial Segment of the SegmentST
 *
 *  @param[in] segment The SegmentST
 *
 *	@return SegmentSP
 *
 *	@see @ref methods_get
 *
 */
CREATE FUNCTION getSp(segment SegmentST) RETURNS SegmentSP AS
	'$libdir/Hermes','getSpSegmentST'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the start of the  Spatial Segment 
 *	of the SegmentST
 *
 *  @param[in] segment The SegmentST
 *
 *	@return start of the SegmentSP
 *
 *	@see @ref methods_get
 *
 */
CREATE FUNCTION getI(segment SegmentST) RETURNS PointSP AS
	'$libdir/Hermes','getISegmentST'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the x coordinate of the start of the  
 *	Spatial Segment of the SegmentST
 *
 *  @param[in] segment The SegmentST
 *
 *	@return start of the SegmentSP
 *
 *	@see @ref methods_get
 *
 */
CREATE FUNCTION getIx(segment SegmentST) RETURNS integer AS
	'$libdir/Hermes','getIxSegmentST'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the y coordinate of the start of the  
 *	Spatial Segment of the SegmentST
 *
 *  @param[in] segment The SegmentST
 *
 *	@return start of the SegmentSP
 *
 *	@see @ref methods_get
 *
 */
CREATE FUNCTION getIy(segment SegmentST) RETURNS integer AS
	'$libdir/Hermes','getIySegmentST'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the end of the  Spatial Segment 
 *	of the SegmentST
 *
 *  @param[in] segment The SegmentST
 *
 *	@return end of the SegmentSP
 *
 *	@see @ref methods_get
 *
 */
CREATE FUNCTION getE(segment SegmentST) RETURNS PointSP AS
	'$libdir/Hermes','getESegmentST'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the x coordinate of the end of the  
 *	Spatial Segment of the SegmentST
 *
 *  @param[in] segment The SegmentST
 *
 *	@return end of the SegmentSP
 *
 *	@see @ref methods_get
 *
 */
CREATE FUNCTION getEx(segment SegmentST) RETURNS integer AS
	'$libdir/Hermes','getExSegmentST'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the y coordinate of the end of the  
 *	Spatial Segment of the SegmentST
 *
 *  @param[in] segment The SegmentST
 *
 *	@return end of the SegmentSP
 *
 *	@see @ref methods_get
 *
 */
CREATE FUNCTION getEy(segment SegmentST) RETURNS integer AS
	'$libdir/Hermes','getEySegmentST'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns a SegmentST data type
 *
 *	@param[in] ti the initial timestamp
 *	@param[in] ix the initial x coordinate
 *	@param[in] iy the initial y coordinate
 *	@param[in] te the ending timestamp
 *	@param[in] ex the ending x coordinate
 *	@param[in] ey the ending y coordinate
 *
 *  @return a SegmentST data type
 */
CREATE FUNCTION SegmentST(ti timestamp, ix integer, iy integer, te timestamp, ex integer, ey integer) RETURNS SegmentST AS
	'$libdir/Hermes','constructorSegmentSTV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns a SegmentST data type
 *
 *	@param[in] i the initial PointST
 *	@param[in] e the initial PointST
 *
 *  @return a SegmentST data type
 */
CREATE FUNCTION SegmentST(i PointST, e PointST) RETURNS SegmentST AS
	'$libdir/Hermes','constructorHighSegmentSTV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function copy a SegmentST data type to a 
 * 	new position in the memory
 *
 *	@param[in] segment the segment to be cloned
 *
 *  @return the new segment in the memory
 */
CREATE FUNCTION clone(segment SegmentST) RETURNS SegmentST AS
	'$libdir/Hermes','cloneSegmentSTV1'
LANGUAGE C IMMUTABLE STRICT;
