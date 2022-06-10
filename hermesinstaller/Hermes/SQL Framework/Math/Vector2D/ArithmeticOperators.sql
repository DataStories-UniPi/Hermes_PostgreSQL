/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION addition(vectorA Vector2D, vectorB Vector2D) RETURNS Vector2D AS
	'$libdir/Hermes','additionVector2DV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION subtraction(vectorA Vector2D, vectorB Vector2D) RETURNS Vector2D AS
	'$libdir/Hermes','subtractionVector2DV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION multiplicationScalar(vector Vector2D, n double precision) RETURNS Vector2D AS
	'$libdir/Hermes','multiplicationScalarVector2DV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION divisionScalar(vector Vector2D, n double precision) RETURNS Vector2D AS
	'$libdir/Hermes','divisionScalarVector2DV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR + (
	PROCEDURE = addition,
	LEFTARG = Vector2D,
	RIGHTARG = Vector2D,

	COMMUTATOR = +
);

CREATE OPERATOR - (
	PROCEDURE = subtraction,
	LEFTARG = Vector2D,
	RIGHTARG = Vector2D
);

CREATE OPERATOR * (
	PROCEDURE = multiplicationScalar,
	LEFTARG = Vector2D,
	RIGHTARG = double precision,

	COMMUTATOR = *
);

CREATE OPERATOR / (
	PROCEDURE = divisionScalar,
	LEFTARG = Vector2D,
	RIGHTARG = double precision
);

/******************************************************************************/
CREATE FUNCTION dotProduct(vectorA Vector2D, vectorB Vector2D) RETURNS double precision AS
	'$libdir/Hermes','dotProductVector2DV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION perpDotProduct(vectorA Vector2D, vectorB Vector2D) RETURNS double precision AS
	'$libdir/Hermes','perpDotProductVector2DV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR * (
	PROCEDURE = dotProduct,
	LEFTARG = Vector2D,
	RIGHTARG = Vector2D,

	COMMUTATOR = *
);

/******************************************************************************/
CREATE FUNCTION norm(vector Vector2D) RETURNS double precision AS
	'$libdir/Hermes','normVector2DV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION sqNorm(vector Vector2D) RETURNS double precision AS
	'$libdir/Hermes','sqNormVector2DV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION normalize(vector Vector2D) RETURNS Vector2D AS
	'$libdir/Hermes','normalizeVector2DV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION distance(vectorA Vector2D, vectorB Vector2D) RETURNS double precision AS
	'$libdir/Hermes','distanceVector2DV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION sqDistance(vectorA Vector2D, vectorB Vector2D) RETURNS double precision AS
	'$libdir/Hermes','sqDistanceVector2DV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION angle(vectorA Vector2D, vectorB Vector2D) RETURNS double precision AS
	'$libdir/Hermes','angleVector2DV1'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR <-> (
	PROCEDURE = distance,
	LEFTARG = Vector2D,
	RIGHTARG = Vector2D,

	COMMUTATOR = <->
);

CREATE OPERATOR <->^ (
	PROCEDURE = sqDistance,
	LEFTARG = Vector2D,
	RIGHTARG = Vector2D,

	COMMUTATOR = <->^
);
