# Visualization {#visualization}

Hermes provides a set of functions that allow to construct a KML document within a query in steps. In particular:

- function KMLPoint() returns a string that gives a KML point placemark element (with each point having the object and trajectory identifiers in its description)
- function string_agg() aggregates points;
- function KMLFolder() encloses aggregated points under one KML folder element.

By enclosing a folder element into a KML document element, a KML file is ready to be written into a system file using “COPY” command. But before someone could export the results of a query he should create a directory where PostgreSQL has access. An example of how we visualized the Timeslice query is following:

	COPY (
		WITH 
			TABULAR_RESULT AS (
				SELECT DISTINCT ON (obj_id,traj_id) obj_id,traj_id,
					atInstant(seg, '2009-01-02 00:00:00') AS position
				FROM imis_seg
				WHERE seg ~ '2009-01-02 00:00:00' :: timestamp
				)
		SELECT KMLDocument(KMLFolder('2009-01-02 00:00:00',
					string_agg(KMLPoint('MMSI : ' || obj_id,getSp(position),
						HDatasetID ('imis')), ' ')))
		FROM TABULAR_RESULT
	) TO '/home/anagno/Timeslice.kml';
	
Another more complicated example is shown below:

	COPY (
		WITH 
			TABULAR_RESULT AS (
				WITH 
					TO_METERS AS (
						SELECT 
							PointSP(PointLL(23.59, 37.91), HDatasetID('imis')) AS low,
							PointSP(PointLL(23.65, 37.96), HDatasetID('imis')) AS high
						), 
					SPT_WINDOW AS (
						 SELECT 
							BoxST(Period('2008-12-31 23:00:00', '2009-01-01 01:00:00'),
							BoxSP((SELECT low FROM TO_METERS), (SELECT high FROM TO_METERS))
						) AS box
					)
				SELECT obj_id, traj_id, (atBox(seg, (SELECT box FROM SPT_WINDOW))).s
				AS seg
				FROM imis_seg
				WHERE seg && (SELECT box FROM SPT_WINDOW)
					AND (atBox(seg, (SELECT box FROM SPT_WINDOW))).n = 2
				)
	 	SELECT KMLDocument(KMLFolder('Input area',
 			KMLPolygon('Piraeus port area',
 			BoxSP(PointSP(PointLL(23.59, 37.91), HDatasetID('imis')),
	 		PointSP(PointLL(23.65, 37.96), HDatasetID('imis'))),
 			HDatasetID('imis'))) || string_agg(tracksFolder, ''))
	 	FROM (
			SELECT obj_id, KMLFolder('MMSI: ' || obj_id,
				string_agg(trackPlacemark, '')) AS tracksFolder
			FROM (
				SELECT obj_id, traj_id, KMLTrack(
					'MMSI: ' || obj_id || '<br/><br/>' ||
					count(*) - 1 || ' points sampled between ' || min(getTi(seg)) ||
					' and ' || max(getTe(seg)) || '.<br/><br/>' ||
					'The ship covered a distance of ' ||
					trunc(metres2nm(sum(length(getSp(seg))))::numeric, 1) ||
					' NM with an average speed of ' ||
 					trunc(mps2knots(sum(length(getSp(seg))) /
					extract(epoch from max(getTe(seg)) - min(getTi(seg))))::numeric
					, 1) || ' knots.'
					, array_agg(seg ORDER BY getTi(seg) ASC), HDatasetID('imis')
				) AS trackPlacemark
				FROM TABULAR_RESULT
				GROUP BY obj_id, traj_id
			) AS tracks
			GROUP BY obj_id
		) AS folders
	) TO '/home/anagno/postgresql/Range.kml';
	
\see KML.sql
\see [PostgreSQL aggregate functions](http://www.postgresql.org/docs/9.4/static/functions-aggregate.html)