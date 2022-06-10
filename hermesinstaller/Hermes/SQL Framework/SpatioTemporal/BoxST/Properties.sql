/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION isPunctal(box BoxST) RETURNS boolean AS
	'$libdir/Hermes','isPunctalBoxSTV1'
LANGUAGE C IMMUTABLE STRICT;
