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
CREATE FUNCTION buffer(box BoxST, tSize interval, xSize integer, ySize integer) RETURNS BoxST AS
	'$libdir/Hermes','bufferBoxSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION enlarge(box BoxST, tSize interval, spSize integer) RETURNS BoxST AS
	'$libdir/Hermes','enlargeBoxSTV1'
LANGUAGE C IMMUTABLE STRICT;
