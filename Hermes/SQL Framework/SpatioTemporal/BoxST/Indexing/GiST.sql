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
CREATE FUNCTION consistentGiSTBoxST(internal, internal, smallint, oid, internal) RETURNS bool AS
	'$libdir/Hermes','consistentGiSTBoxST'
LANGUAGE C STRICT;

CREATE FUNCTION unionGiSTBoxST(internal, internal) RETURNS internal AS
	'$libdir/Hermes','unionGiSTBoxST'
LANGUAGE C STRICT;

CREATE FUNCTION compressGiSTBoxST(internal) RETURNS internal AS
	'$libdir/Hermes','compressGiSTBoxST'
LANGUAGE C STRICT;

CREATE FUNCTION decompressGiSTBoxST(internal) RETURNS internal AS
	'$libdir/Hermes','decompressGiSTBoxST'
LANGUAGE C STRICT;

CREATE FUNCTION penaltyGiSTBoxST(internal, internal, internal) RETURNS internal AS
	'$libdir/Hermes','penaltyGiSTBoxST'
LANGUAGE C STRICT;

CREATE FUNCTION picksplitGiSTBoxST(internal, internal) RETURNS internal AS
	'$libdir/Hermes','picksplitGiSTBoxST'
LANGUAGE C STRICT;

CREATE FUNCTION sameGiSTBoxST(internal, internal, internal) RETURNS internal AS
	'$libdir/Hermes','sameGiSTBoxST'
LANGUAGE C STRICT;

CREATE FUNCTION distanceGiSTBoxST(internal, internal, smallint, oid) RETURNS float8 AS
	'$libdir/Hermes','distanceGiSTBoxST'
LANGUAGE C STRICT;

/******************************************************************************/
CREATE OPERATOR CLASS BoxST_OPS DEFAULT FOR TYPE BoxST USING gist AS
	OPERATOR 1 && (BoxST, BoxST),
	OPERATOR 2 && (BoxST, Period),
	OPERATOR 3 && (BoxST, BoxSP),

	OPERATOR 4 <-> (BoxST, Period) FOR ORDER BY pg_catalog.float_ops,
	OPERATOR 5 <-> (BoxST, BoxSP) FOR ORDER BY pg_catalog.float_ops,

	FUNCTION 1 consistentGiSTBoxST(internal, internal, smallint, oid, internal),
	FUNCTION 2 unionGiSTBoxST(internal, internal),
	FUNCTION 3 compressGiSTBoxST(internal),
	FUNCTION 4 decompressGiSTBoxST(internal),
	FUNCTION 5 penaltyGiSTBoxST(internal, internal, internal),
	FUNCTION 6 picksplitGiSTBoxST(internal, internal),
	FUNCTION 7 sameGiSTBoxST(internal, internal, internal),
	FUNCTION 8 distanceGiSTBoxST(internal, internal, smallint, oid);
