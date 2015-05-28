/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION PointLLFromGeom(ll anyelement) RETURNS PointLL AS $$
BEGIN
    RETURN PointLL(ST_X(ll), ST_Y(ll));
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE FUNCTION PointSPFromGeom(xy anyelement) RETURNS PointSP AS $$
BEGIN
    RETURN PointSP(round(ST_X(xy))::integer, round(ST_Y(xy))::integer);
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;
