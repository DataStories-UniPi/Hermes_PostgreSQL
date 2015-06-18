/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief The file implements the interpolations functions of the SegmentST object
 *
 * @see @ref methods
 *
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
 *
 *  @return the period
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
 *
 *  @return the period
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
 *
 *	@param[in] segment the segment
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
 *
 *	@param[in] segment the segment
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
 *
 *	@param[in] segment the segment
 *	@param[in] box the box
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
 *
 *	@param[in] segment the segment
 *	@param[in] box the box
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
 *
 *	@param[in] segment the segment
 *	@param[in] box the box
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
