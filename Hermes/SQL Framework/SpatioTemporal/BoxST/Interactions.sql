/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief The implementantion in sql of BoxST.
 *
 * @details BoxST is a spatial data type used in Hermes
 *
 * @see @ref data_types_spatio_temp
 *
 */

/******************************************************************************/
CREATE FUNCTION intersection(boxA BoxST, boxB BoxST) RETURNS BoxST AS
	'$libdir/Hermes','intersectionBoxST_BoxSTV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief This function returns the union of a BoxST and PointST
 * 
 *  @param[in] box an BoxST
 * 	@param[in] point an PointST
 *
 *  @return the union
 * 
 * 	@see @ref methods_hunion
 */
CREATE FUNCTION HUnion(box BoxST, point PointST) RETURNS BoxST AS
	'$libdir/Hermes','unionBoxST_PointSTV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief This function returns the union of a BoxST and SegmentST
 * 
 *  @param[in] box an BoxST
 * 	@param[in] segment an SegmentST
 *
 *  @return the union
 * 
 * 	@see @ref methods_hunion
 */
CREATE FUNCTION HUnion(box BoxST, segment SegmentST) RETURNS BoxST AS
	'$libdir/Hermes','unionBoxST_SegmentSTV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief This function returns the union of two BoxST
 * 
 *  @param[in] boxA an BoxST
 * 	@param[in] boxB an BoxST
 *
 *  @return the union
 * 
 * 	@see @ref methods_hunion
 */
CREATE FUNCTION HUnion(boxA BoxST, boxB BoxST) RETURNS BoxST AS
	'$libdir/Hermes','unionBoxST_BoxSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE AGGREGATE HUnion(BoxST) (
	SFUNC = HUnion(BoxST, BoxST),
	STYPE = BoxST
);
