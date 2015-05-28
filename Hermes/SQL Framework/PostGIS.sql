/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION PostGIS(seg SegmentST, dataset integer DEFAULT HCurrentDatasetID()) RETURNS geometry AS $$
DECLARE
	geom geometry;

	poiWGS_i PointLL;
	poiWGS_e PointLL;
BEGIN
	poiWGS_i := PointLL(getI(seg), dataset);
	poiWGS_e := PointLL(getE(seg), dataset);

	geom := ST_SetSRID(ST_MakeLine(ST_MakePoint(getLon(poiWGS_i), getLat(poiWGS_i), extract(epoch from getTi(seg))), ST_MakePoint(getLon(poiWGS_e), getLat(poiWGS_e), extract(epoch from getTe(seg)))), 4326);

	RETURN geom;
END;
$$ LANGUAGE plpgsql STABLE STRICT;

CREATE FUNCTION PostGIS(traj Trajectory, dataset integer DEFAULT HCurrentDatasetID()) RETURNS geometry AS $$
DECLARE
	geom geometry;

	nr_points integer;
	i integer;

	poi PointST;
	poiWGS PointLL;
BEGIN
	poi := firstPoint(traj);
	poiWGS := PointLL(getSp(poi), dataset);

	geom := ST_MakeLine(ST_MakePoint(getLon(poiWGS), getLat(poiWGS), extract(epoch from getT(poi))));

	nr_points := NrPoints(traj);
	FOR i IN 2..nr_points LOOP
		poi := pointAt(traj, i);
		poiWGS := PointLL(getSp(poi), dataset);

		geom := ST_AddPoint(geom, ST_MakePoint(getLon(poiWGS), getLat(poiWGS), extract(epoch from getT(poi))));
	END LOOP;

	RETURN ST_SetSRID(geom, 4326);
END;
$$ LANGUAGE plpgsql STABLE STRICT;
