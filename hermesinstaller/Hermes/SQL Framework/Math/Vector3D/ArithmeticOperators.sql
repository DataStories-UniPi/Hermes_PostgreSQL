/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION addition(vectorA Vector3D, vectorB Vector3D) RETURNS Vector3D AS
	'$libdir/Hermes','additionVector3DV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION subtraction(vectorA Vector3D, vectorB Vector3D) RETURNS Vector3D AS
	'$libdir/Hermes','subtractionVector3DV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION multiplicationScalar(vector Vector3D, n double precision) RETURNS Vector3D AS
	'$libdir/Hermes','multiplicationScalarVector3DV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION divisionScalar(vector Vector3D, n double precision) RETURNS Vector3D AS
	'$libdir/Hermes','divisionScalarVector3DV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR + (
	PROCEDURE = addition,
	LEFTARG = Vector3D,
	RIGHTARG = Vector3D,

	COMMUTATOR = +
);

CREATE OPERATOR - (
	PROCEDURE = subtraction,
	LEFTARG = Vector3D,
	RIGHTARG = Vector3D
);

CREATE OPERATOR * (
	PROCEDURE = multiplicationScalar,
	LEFTARG = Vector3D,
	RIGHTARG = double precision,

	COMMUTATOR = *
);

CREATE OPERATOR / (
	PROCEDURE = divisionScalar,
	LEFTARG = Vector3D,
	RIGHTARG = double precision
);

/******************************************************************************/
CREATE FUNCTION dotProduct(vectorA Vector3D, vectorB Vector3D) RETURNS double precision AS
	'$libdir/Hermes','dotProductVector3DV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION crossProduct(vectorA Vector3D, vectorB Vector3D) RETURNS Vector3D AS
	'$libdir/Hermes','crossProductVector3DV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR * (
	PROCEDURE = dotProduct,
	LEFTARG = Vector3D,
	RIGHTARG = Vector3D,

	COMMUTATOR = *
);

/******************************************************************************/
CREATE FUNCTION norm(vector Vector3D) RETURNS double precision AS
	'$libdir/Hermes','normVector3DV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION sqNorm(vector Vector3D) RETURNS double precision AS
	'$libdir/Hermes','sqNormVector3DV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION normalize(vector Vector3D) RETURNS Vector3D AS
	'$libdir/Hermes','normalizeVector3DV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION distance(vectorA Vector3D, vectorB Vector3D) RETURNS double precision AS
	'$libdir/Hermes','distanceVector3DV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION sqDistance(vectorA Vector3D, vectorB Vector3D) RETURNS double precision AS
	'$libdir/Hermes','sqDistanceVector3DV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR <-> (
	PROCEDURE = distance,
	LEFTARG = Vector3D,
	RIGHTARG = Vector3D,

	COMMUTATOR = <->
);

CREATE OPERATOR <->^ (
	PROCEDURE = sqDistance,
	LEFTARG = Vector3D,
	RIGHTARG = Vector3D,

	COMMUTATOR = <->^
);
