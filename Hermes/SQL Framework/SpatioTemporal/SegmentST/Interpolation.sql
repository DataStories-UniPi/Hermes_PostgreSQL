/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the SegmentST data type.
 *
 * @see @ref SegmentST
 * @see @ref data_types_spatio_temp
 */

/** @brief This function takes a segment and a timestamp as parameters and returns
 * 	the point where the object was found at the given timestamp
 *
 *	@param[in] segment the segment
 *
 *  @return a PointSP with the position of the interpolation
 * 
 * 	@see @ref methods_instant
 */
CREATE FUNCTION atInstantSP(segment SegmentST, ts timestamp, checkContainment boolean DEFAULT true) RETURNS PointSP AS
	'$libdir/Hermes','atInstantSPSegmentSTV1'
LANGUAGE C STABLE STRICT;

/** @brief This function takes a segment and a timestamp as parameters and returns
 * 	the point where the object was found at the given timestamp
 *
 *	@param[in] segment the segment
 *
 *  @return a PointST with the position of the interpolation
 * 
 * 	@see @ref methods_instant
 */
CREATE FUNCTION atInstant(segment SegmentST, ts timestamp, checkContainment boolean DEFAULT true) RETURNS PointST AS
	'$libdir/Hermes','atInstantSegmentSTV1'
LANGUAGE C STABLE STRICT;

/** @brief This function takes a segment and a period as parameters and 
 * returns the part of the segment that corresponds to the given period. 
 * The segment might have only one timestamp in common with the period so 
 * in that case the function returns a point instead of a segment. This is 
 * why the function returns three columns (n, s, p) where n is the number 
 * of common points, s is the segment within the period (if n is 2) and p 
 * is the point that the segment was within the period (if n is 1).
 *
 *	@param[in] segment the segment
 *	@param[in] ts the period
 *
 *  @return the part of the segment that corresponds to the given period
 * 
 * 	@see @ref methods_atperiod
 */
CREATE FUNCTION atPeriodSP(segment SegmentST, period Period,
	OUT n integer, OUT s SegmentSP, OUT p PointSP
) AS
	'$libdir/Hermes','atPeriodSPSegmentSTV1'
LANGUAGE C STABLE STRICT;

/** @brief This function takes a segment and a period as parameters and 
 * returns the part of the segment that corresponds to the given period. 
 * The segment might have only one timestamp in common with the period so 
 * in that case the function returns a point instead of a segment. This is 
 * why the function returns three columns (n, s, p) where n is the number 
 * of common points, s is the segment within the period (if n is 2) and p 
 * is the point that the segment was within the period (if n is 1).
 *
 *	@param[in] segment the segment
 *	@param[in] period the period
 *	@param[out] n the number of common points
 *	@param[out] s the segment within the period
 *	@param[out] p the point that the segment was within the period
 *
 *  @return the part of the segment that corresponds to the given period
 * 
 * 	@see @ref methods_atperiod
 */
CREATE FUNCTION atPeriod(segment SegmentST, period Period,
	OUT n integer, OUT s SegmentST, OUT p PointST
) AS
	'$libdir/Hermes','atPeriodSegmentSTV1'
LANGUAGE C STABLE STRICT;

/** @brief This function takes a segment and a point as parameters and 
 * 	returns the timestamp at which the object was found at the given point. 
 * 	The point has to be on the segment, otherwise the function returns NULL.
 * 	The value “false” on the last parameter enforces it to avoid checking 
 * 	for containment. In contrast, if the value of the third parameter is 
 * 	set to “yes” then the cost of the calculation gets higher.
 *
 *	@param[in] segment the segment
 *	@param[in] point the point
 *	@param[in] checkContainment check if the point is contained on the segment
 *
 *  @return the timestamp of the point
 * 
 * 	@see @ref methods_atpoint
 */
CREATE FUNCTION atPointT(segment SegmentST, point PointSP, checkContainment boolean DEFAULT true) RETURNS timestamp AS
	'$libdir/Hermes','atPointTSegmentSTV1'
LANGUAGE C STABLE STRICT;

/** @brief This function takes a segment and a point as parameters and 
 * 	returns the timestamp at which the object was found at the given point. 
 * 	The point has to be on the segment, otherwise the function returns NULL.
 * 	The value “false” on the last parameter enforces it to avoid checking 
 * 	for containment. In contrast, if the value of the third parameter is 
 * 	set to “yes” then the cost of the calculation gets higher.
 *
 *	@param[in] segment the segment
 *	@param[in] point the point
 *	@param[in] checkContainment check if the point is contained on the segment
 *
 *  @return a PointST with the timestamp of the interpolation
 * 
 *  @see @ref methods_atpoint
 */
CREATE FUNCTION atPoint(segment SegmentST, point PointSP, checkContainment boolean DEFAULT true) RETURNS PointST AS
	'$libdir/Hermes','atPointSegmentSTV1'
LANGUAGE C STABLE STRICT;

/** @brief This function takes a segment and a box as 
 * parameters and returns the part of the segment that 
 * resides within the box.
 * Just as atinstant() method the function returns three 
 * columns (n, s, p) where n informs whether the result is 
 * a point (value 1) or a segment (value 2) or there is no 
 * intersection between the segment and the box (value 0).
 * Especially for values 1 and 2 of property “n”, “p” gets 
 * the point and “s” gets the segment, respectively 
 *
 *	@param[in] segment the segment
 *	@param[in] box the box
 * 	@param[out] n informs whether the result is a point
 *  @param[out] p the point
 *	@param[out] s the segment
 *
 *  @return the part of the segment
 * 
 * 	@see @ref methods_atbox
 */
CREATE FUNCTION atBoxT(segment SegmentST, box BoxSP,
	OUT n integer, OUT p Period, OUT ts timestamp
) AS
	'$libdir/Hermes','atBoxTSegmentSTV1'
LANGUAGE C STABLE STRICT;

/** @brief This function takes a segment and a box as 
 * parameters and returns the part of the segment that 
 * resides within the box. 
 * Just as atinstant() method the function returns three 
 * columns (n, s, p) where n informs whether the result is 
 * a point (value 1) or a segment (value 2) or there is no 
 * intersection between the segment and the box (value 0).
 * Especially for values 1 and 2 of property “n”, “p” gets 
 * the point and “s” gets the segment, respectively 
 *
 *	@param[in] segment the segment
 *	@param[in] box the box
 * 	@param[out] n informs whether the result is a point
 *  @param[out] p the point
 *	@param[out] s the segment
 *
 *  @return the part of the segment
 * 
 * 	@see @ref methods_atbox
 */
CREATE FUNCTION atBox(segment SegmentST, box BoxSP,
	OUT n integer, OUT s SegmentST, OUT p PointST
) AS
	'$libdir/Hermes','atBoxSegmentSTV1'
LANGUAGE C STABLE STRICT;

/** @brief This function takes a segment and a box as 
 * parameters and returns the part of the segment that 
 * resides within the box.
 * Just as atinstant() method the function returns three 
 * columns (n, s, p) where n informs whether the result is 
 * a point (value 1) or a segment (value 2) or there is no 
 * intersection between the segment and the box (value 0).
 * Especially for values 1 and 2 of property “n”, “p” gets 
 * the point and “s” gets the segment, respectively 
 *
 *	@param[in] segment the segment
 *	@param[in] box the box
 * 	@param[out] n informs whether the result is a point
 *  @param[out] p the point
 *	@param[out] s the segment
 *
 *  @return the part of the segment
 * 
 * 	@see @ref methods_atbox
 */
CREATE FUNCTION atBox(segment SegmentST, box BoxST,
	OUT n integer, OUT s SegmentST, OUT p PointST
) AS
	'$libdir/Hermes','atBoxSTSegmentSTV1'
LANGUAGE C STABLE STRICT;

/******************************************************************************/
CREATE FUNCTION intersectionPoint(segmentA SegmentST, segmentB SegmentST) RETURNS PointST AS
	'$libdir/Hermes','intersectionPointSegmentST_SegmentSTV1'
LANGUAGE C STABLE STRICT;
