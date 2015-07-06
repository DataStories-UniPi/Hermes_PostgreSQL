# Advanced # {#advanced}

In this section are described the more advanced functions of Hermes MOD

# Distance functions # {#similarity}

Measuring the similarity/distance between trajectories is not straightforward mainly because we need to take into account the temporal dimension. There are many proposed measures in the bibliography and Hermes implements some of the state of the art methods in its similarity measures library @cite vodas2013hermes. The trajectory similarity functions that are implemented in Hermes include: Manhattan, Euclidean, Tchebycheff, DISSIM, DTW, LCSS, EDR, ERP.


The process of calculating the similarity between two trajectories is more complicated than calculating the corresponding similarity between points. The reasons that makes the calculations more complicated for trajectories are @cite anagnostopoulos2015similarity:

- that the values may be observed in equidistant or varying distant intervals
- that the two trajectories may contain different number of measurement points
- that the trajectories may be measured in 2 or multi-dimensional spaces.

## Euclidean distance measure ## {#similarity_euclidean}

A simple solution to calculate the similarity is to use an $L_p$ norm to the sequences of points. Euclidean distance has the advantage of being easy to compute (and specifically it has a linear computation cost and is easy to implement. However, the Euclidean distance outliers have a great impact on the overall distance (due to the fact that is computed by square deviations) and it does not support local time shifting @cite anagnostopoulos2015similarity. 

To be able to calculate the measure of similarity for the trajectories, one needs to define @cite anagnostopoulos2015similarity:
- how to find the matching pairs in the two trajectories and 
- how to take into account the varying numbers of points in the trajectories



(\htmlonly /home/anagno/Documents/unipi/4ο Εξάμηνο/Συστήματα Γεωγραφικών Πληροφοριών/hermes/Hermes/SQL Framework/SpatioTemporal/Trajectory \endhtmlonly)

# Clustering  {#clustering}

According to @cite wiki:cluster_analysis clustering is the task of grouping a set of objects in such way that objects in the same group are more similar to each other than to those in other groups.

## TRACLUS algorithm ## {#traclus}

TRACLUS algorithm was introduced by @cite lee2007trajectory in an effort to cluster similar trajectories. The main difference from the rest algorithms that cluster trajectories is that it partitions a trajectory into a set of line segments, and then, groups similar line segments together into a cluster and the primary advantage of this algorithm is that is able to discover common sub-trajectories from trajectory database.

The algorithm works on two phases. First it partitions the trajectories using the minimum description length principle and then it groups them using a density-based line-segment clustering algorithm.

An example using the TRACLUS algorithm with Hermes is shown below

    SELECT traclusDistance(
    	SegmentSP(PointSP(2337709, 4163887),PointSP(3228259, 4721671)),
    	SegmentSP(PointSP(2337709, 4163887),PointSP(3228259, 4721671))
    ); 

I HAVE NO IDEA WHAT IT RETURNS. SOMEONE SHOULD EXPLAIN MORE.

\see http://hanj.cs.illinois.edu/pdf/sigmod07_jglee.pdf

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