/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION multiplicationScalar(n double precision, vector Vector3D) RETURNS Vector3D AS
	'SELECT multiplicationScalar($2, $1);'
LANGUAGE SQL IMMUTABLE STRICT;

CREATE OPERATOR * (
	PROCEDURE = multiplicationScalar,
	LEFTARG = double precision,
	RIGHTARG = Vector3D,

	COMMUTATOR = *
);
