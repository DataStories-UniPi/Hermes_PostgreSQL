/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION buffer(box BoxST, tSize interval, xSize integer, ySize integer) RETURNS BoxST AS
	'$libdir/Hermes','bufferBoxSTV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION enlarge(box BoxST, tSize interval, spSize integer) RETURNS BoxST AS
	'$libdir/Hermes','enlargeBoxSTV1'
LANGUAGE C IMMUTABLE STRICT;
