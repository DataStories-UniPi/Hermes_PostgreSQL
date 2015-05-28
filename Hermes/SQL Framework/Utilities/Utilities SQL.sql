/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
--SELECT x'0a'::bigint;
CREATE FUNCTION HEX2int(HEX text) RETURNS bigint AS $$
DECLARE
	ret bigint;
BEGIN
	EXECUTE 'SELECT x''' || HEX || '''::bigint' INTO ret;
	RETURN ret;
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

/******************************************************************************/
--TODO implement the random() methods in C

CREATE FUNCTION random(fromInclusive double precision, toExclusive double precision) RETURNS double precision AS
	'SELECT CASE WHEN $1 > $2 THEN NULL ELSE random() * ($2 - $1) + $1 END;'
LANGUAGE SQL STRICT;

CREATE FUNCTION random(fromInclusive real, toExclusive real) RETURNS real AS
	'SELECT random($1::double precision, $2::double precision)::real;'
LANGUAGE SQL STRICT;

CREATE FUNCTION random(fromInclusive bigint, toExclusive bigint) RETURNS bigint AS
	'SELECT CASE WHEN $1 > $2 THEN NULL ELSE trunc(random() * ($2 - $1) + $1)::bigint END;'
LANGUAGE SQL STRICT;

CREATE FUNCTION random(fromInclusive integer, toExclusive integer) RETURNS integer AS
	'SELECT random($1::bigint, $2::bigint)::integer;'
LANGUAGE SQL STRICT;

CREATE FUNCTION random(fromInclusive smallint, toExclusive smallint) RETURNS smallint AS
	'SELECT random($1::bigint, $2::bigint)::smallint;'
LANGUAGE SQL STRICT;

CREATE FUNCTION random(fromInclusive timestamp, toExclusive timestamp) RETURNS timestamp AS
	'SELECT CASE WHEN $1 > $2 THEN NULL ELSE random() * ($2 - $1) + $1 END;'
LANGUAGE SQL STRICT;
