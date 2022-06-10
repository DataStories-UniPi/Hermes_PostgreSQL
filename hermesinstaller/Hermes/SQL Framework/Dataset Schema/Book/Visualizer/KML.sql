/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

/******************************************************************************/
CREATE FUNCTION KMLDocument(content text, colors hstore DEFAULT 'polygon=>16000000, track=>ffffffff, STOP=>ff0000ff, MOVE=>ffffffff', widths hstore DEFAULT 'track=>1, STOP=>1, MOVE=>1') RETURNS text AS $$
DECLARE
	kml text;
BEGIN
	kml := '<?xml version="1.0" encoding="UTF-8"?><kml xmlns="http://www.opengis.net/kml/2.2" xmlns:gx="http://www.google.com/kml/ext/2.2"><Document>' ||
	'<Style id="polygon_style"><PolyStyle><color>' || (colors -> 'polygon') || '</color></PolyStyle></Style>' ||
	'<Style id="track_style"><IconStyle><Icon></Icon></IconStyle><LineStyle><color>' || (colors -> 'track') || '</color><width>' || (widths -> 'track') || '</width></LineStyle></Style>' ||
	'<Style id="STOP_style"><IconStyle><Icon></Icon></IconStyle><LineStyle><color>' || (colors -> 'STOP') || '</color><width>' || (widths -> 'STOP') || '</width></LineStyle></Style>' ||
	'<Style id="MOVE_style"><IconStyle><Icon></Icon></IconStyle><LineStyle><color>' || (colors -> 'MOVE') || '</color><width>' || (widths -> 'MOVE') || '</width></LineStyle></Style>';

	RETURN kml || content || '</Document></kml>';
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE FUNCTION KMLFolder(folderName text, content text) RETURNS text AS $$
BEGIN
	RETURN '<Folder><name>' || folderName || '</name>' || content || '</Folder>';
END;
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

/******************************************************************************/
CREATE FUNCTION KMLPoint(description text, point PointSP, dataset integer DEFAULT HCurrentDatasetID()) RETURNS text AS $$
DECLARE
	poiWGS PointLL;
BEGIN
	poiWGS := PointLL(point, dataset);

	RETURN '<Placemark><description>' || description || '</description><Point><coordinates>' || getLon(poiWGS) || ',' || getLat(poiWGS) || '</coordinates></Point></Placemark>';
END;
$$ LANGUAGE plpgsql STABLE STRICT;

/******************************************************************************/
CREATE FUNCTION KMLPolygon(description text, box BoxSP, dataset integer DEFAULT HCurrentDatasetID()) RETURNS text AS $$
DECLARE
	l PointLL;
	hl PointLL;
	h PointLL;
	lh PointLL;
BEGIN
	l := PointLL(getL(box), dataset);
	hl := PointLL(hl(box), dataset);
	h := PointLL(getH(box), dataset);
	lh := PointLL(lh(box), dataset);

	RETURN '<Placemark><styleUrl>#polygon_style</styleUrl><description>' || description || '</description><Polygon><outerBoundaryIs><LinearRing><coordinates>' ||
	getLon(l) || ',' || getLat(l) || ' ' || getLon(hl) || ',' || getLat(hl) || ' ' || getLon(h) || ',' || getLat(h) || ' ' || getLon(lh) || ',' || getLat(lh) || ' ' ||
	getLon(l) || ',' || getLat(l) || '</coordinates></LinearRing></outerBoundaryIs></Polygon></Placemark>';
END;
$$ LANGUAGE plpgsql STABLE STRICT;

CREATE FUNCTION KMLPolygon(description text, range RangeSP, segmentsInPolygon integer DEFAULT 360, dataset integer DEFAULT HCurrentDatasetID()) RETURNS text AS $$
DECLARE
	circlePolygon text;

	d double precision;
	cLL PointLL;
	clonRad double precision;
	clatRad double precision;

	bearing double precision;
	lon double precision;
	lat double precision;
BEGIN
	d := getR(range) / 6371000.0; --Earth mean radius is 6371 km
	cLL := PointLL(getC(range), dataset);
	clonRad := radians(getLon(cLL));
	clatRad := radians(getLat(cLL));

	circlePolygon := '<Placemark><description>' || description || '</description><Point><coordinates>' || getLon(cLL) || ',' || getLat(cLL) || '</coordinates></Point></Placemark>';
	circlePolygon := circlePolygon || '<Placemark><styleUrl>#polygon_style</styleUrl><description>' || description || '</description><Polygon><outerBoundaryIs><LinearRing><coordinates>';

	FOR i IN 0..segmentsInPolygon LOOP
		bearing := i * pi() / 180.0;

		lat := asin(sin(clatRad) * cos(d) + cos(clatRad) * sin(d) * cos(bearing));
		lon := ((clonRad + atan2(sin(bearing) * sin(d) * cos(clatRad), cos(d) - sin(clatRad) * sin(lat))) * 180.0) / pi();
		lat := (lat * 180.0) / pi();

		circlePolygon := circlePolygon || ' ' || lon || ',' || lat;
	END LOOP;

	RETURN circlePolygon || '</coordinates></LinearRing></outerBoundaryIs></Polygon></Placemark>';
END;
$$ LANGUAGE plpgsql STABLE STRICT;

/******************************************************************************/
CREATE FUNCTION KMLTrack(description text, trajSegments SegmentST[], dataset integer DEFAULT HCurrentDatasetID()) RETURNS text AS $$
DECLARE
	trackPlacemark text;
	isMultiTrack boolean;

	nr_segments integer;
	i integer;

	poi PointST;
	poiWGS PointLL;

	prev_te timestamp;
BEGIN
	poi := i(trajSegments[1]);
	poiWGS := PointLL(getSp(poi), dataset);

	trackPlacemark := '<gx:Track><when>' || replace(getT(poi)::text, ' ', 'T') || 'Z</when>' || '<gx:coord>' || getLon(poiWGS) || ' ' || getLat(poiWGS) || ' 0</gx:coord>';

	poi := e(trajSegments[1]);
	poiWGS := PointLL(getSp(poi), dataset);

	trackPlacemark := trackPlacemark || '<when>' || replace(getT(poi)::text, ' ', 'T') || 'Z</when>' || '<gx:coord>' || getLon(poiWGS) || ' ' || getLat(poiWGS) || ' 0</gx:coord>';

	prev_te := getT(poi);
	isMultiTrack := false;

	nr_segments := array_length(trajSegments, 1);
	FOR i IN 2..nr_segments LOOP
		IF prev_te <> getTi(trajSegments[i]) THEN
			trackPlacemark := trackPlacemark || '</gx:Track><gx:Track>';

			poi := i(trajSegments[i]);
			poiWGS := PointLL(getSp(poi), dataset);

			trackPlacemark := trackPlacemark || '<when>' || replace(getT(poi)::text, ' ', 'T') || 'Z</when>' || '<gx:coord>' || getLon(poiWGS) || ' ' || getLat(poiWGS) || ' 0</gx:coord>';

			poi := e(trajSegments[i]);
			poiWGS := PointLL(getSp(poi), dataset);

			trackPlacemark := trackPlacemark || '<when>' || replace(getT(poi)::text, ' ', 'T') || 'Z</when>' || '<gx:coord>' || getLon(poiWGS) || ' ' || getLat(poiWGS) || ' 0</gx:coord>';

			isMultiTrack := true;
		ELSE
			poi := e(trajSegments[i]);
			poiWGS := PointLL(getSp(poi), dataset);

			trackPlacemark := trackPlacemark || '<when>' || replace(getT(poi)::text, ' ', 'T') || 'Z</when>' || '<gx:coord>' || getLon(poiWGS) || ' ' || getLat(poiWGS) || ' 0</gx:coord>';
		END IF;

		prev_te := getT(poi);
	END LOOP;

	IF isMultiTrack THEN
		RETURN '<Placemark><styleUrl>#track_style</styleUrl><description>' || description || '</description><gx:MultiTrack>' || trackPlacemark || '</gx:Track></gx:MultiTrack></Placemark>';
	ELSE
		RETURN '<Placemark><styleUrl>#track_style</styleUrl><description>' || description || '</description>' || trackPlacemark || '</gx:Track></Placemark>';
	END IF;
END;
$$ LANGUAGE plpgsql STABLE STRICT;

CREATE FUNCTION KMLTrack(description text, traj Trajectory, isSTOP boolean DEFAULT false, isMOVE boolean DEFAULT false, dataset integer DEFAULT HCurrentDatasetID()) RETURNS text AS $$
DECLARE
	trackPlacemark text;
	result text;

	nr_points integer;
	i integer;

	poi PointST;
	poiWGS PointLL;
BEGIN
	poi := firstPoint(traj);
	poiWGS := PointLL(getSp(poi), dataset);

	trackPlacemark := '<gx:Track><when>' || replace(getT(poi)::text, ' ', 'T') || 'Z</when>' || '<gx:coord>' || getLon(poiWGS) || ' ' || getLat(poiWGS) || ' 0</gx:coord>';

	nr_points := NrPoints(traj);
	FOR i IN 2..nr_points LOOP
		poi := pointAt(traj, i);
		poiWGS := PointLL(getSp(poi), dataset);

		trackPlacemark := trackPlacemark || '<when>' || replace(getT(poi)::text, ' ', 'T') || 'Z</when>' || '<gx:coord>' || getLon(poiWGS) || ' ' || getLat(poiWGS) || ' 0</gx:coord>';
	END LOOP;

	IF isSTOP THEN
		result := '<Placemark><styleUrl>#STOP_style</styleUrl>';
	ELSIF isMOVE THEN
		result := '<Placemark><styleUrl>#MOVE_style</styleUrl>';
	ELSE
		result := '<Placemark><styleUrl>#track_style</styleUrl>';
	END IF;

	IF length(description) > 0 THEN
		result := result || '<description>' || description || '</description>';
	END IF;

	RETURN result || trackPlacemark || '</gx:Track></Placemark>';
END;
$$ LANGUAGE plpgsql STABLE STRICT;
