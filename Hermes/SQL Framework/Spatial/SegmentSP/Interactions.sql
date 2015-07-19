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

/** @brief This function takes a spatial segment and a spatial box as parameters 
 * and returns the intersection of the segment with the box. There is also a third 
 * optional parameter, called “solid”, that when is set to false the function 
 * returns NULL when the segment is fully contained within the box without touching 
 * the perimeter.
 *
 *	@param[in] segment the segment
 *	@param[in] box the box
 *	@param[in] solid solid parameter of the function
 *
 *  @return the intersection of the segment with the box.
 * 
 * 	@see @ref methods_intersection
 */
CREATE FUNCTION intersection(segment SegmentSP, box BoxSP, solid boolean DEFAULT true,
	OUT n integer, OUT s SegmentSP, OUT p PointSP
) AS
	'$libdir/Hermes','intersectionSegmentSP_BoxSPV1'
LANGUAGE C IMMUTABLE STRICT;


/** @brief This function takes two spatial segments as parameters 
 * and returns the intersection of the segments. 
 *
 *	@param[in] segmentA the first segment
 *	@param[in] segmentB the second segment
 *
 *  @return the intersection of the two segments.
 * 
 * 	@see @ref methods_intersection
 */
CREATE FUNCTION intersection(segmentA SegmentSP, segmentB SegmentSP,
	OUT n integer, OUT s SegmentSP, OUT p PointSP
) AS
	'$libdir/Hermes','intersectionSegmentSP_SegmentSPV1'
LANGUAGE C STABLE STRICT;

/** @brief This function takes as input a segment and a point 
 * and returns the closest point of the segment. 
 *
 *	@param[in] segment the segment
 *	@param[in] point the point
 *
 *  @return the closest point of the segment.
 */
CREATE FUNCTION closestPoint(segment SegmentSP, point PointSP) RETURNS PointSP AS
	'$libdir/Hermes','closestPointSegmentSP_PointSPV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION closestPoints(segmentA SegmentSP, segmentB SegmentSP,
	OUT areEqual boolean, OUT cpA PointSP, OUT cpB PointSP
) AS
	'$libdir/Hermes','closestPointsSegmentSP_SegmentSPV1'
LANGUAGE C STABLE STRICT;
