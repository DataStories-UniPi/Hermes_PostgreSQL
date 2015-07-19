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
CREATE FUNCTION isPunctal(box BoxST) RETURNS boolean AS
	'$libdir/Hermes','isPunctalBoxSTV1'
LANGUAGE C IMMUTABLE STRICT;
