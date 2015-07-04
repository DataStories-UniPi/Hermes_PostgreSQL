# Querying the database # {#querying}

Once the dataset has been stored (and efficiently indexed) in Hermes, one can perform a number of queries exploiting on the strength of Hermes data type system (with a number of methods and operators on trajectory data) @cite pelekis2014mobility .

# Timeslice, Range and Nearest-Neighbor Queries # {#queries_spatial}

Timeslice, range and nearest-neighbor (NN) queries are essential for trajectory data management @cite pelekis2014mobility. Below are given some examples of such queries and are explained in detail.

## Find the position of all ships on the second day of 2009, midnight (timeslice querie) ## 

The SQL script that answers this question is listed below:

	SELECT DISTINCT ON ( obj_id , traj_id ) obj_id , traj_id ,
		atInstant ( seg , '2009-01-02 00:00:00') AS position
	FROM imis_seg
	WHERE seg ~ '2009-01-02 00:00:00' :: timestamp ;
	
The index-supported operator @ref operators_contains filters the database in order to select only those segments that contain the given timestamp “2009-01-01 00:00:00” in their temporal dimension. Then, the @rer atinstant() method finds the exact location of objects at the given timestamp @cite pelekis2014mobility. Some results of the query are shown below:
	
	 obj_id | traj_id |               position                
	--------+---------+---------------------------------------
	      6 |       1 | '2009-01-02 00:00:00' 2010473 3895001
	      8 |       1 | '2009-01-02 00:00:00' 1857235 3927390
	      9 |       1 | '2009-01-02 00:00:00' 2012939 3898405
	     11 |       1 | '2009-01-02 00:00:00' 2016087 3901072
	     12 |       1 | '2009-01-02 00:00:00' 2015512 3901574
	     13 |       1 | '2009-01-02 00:00:00' 2289075 3876879
	     14 |       1 | '2009-01-02 00:00:00' 2130213 3680774
	     17 |       1 | '2009-01-02 00:00:00' 2289575 3876028
	     18 |       1 | '2009-01-02 00:00:00' 2247820 3972149
	     21 |       1 | '2009-01-02 00:00:00' 2182760 3924308
	     23 |       1 | '2009-01-02 00:00:00' 2180014 3944883
	     26 |       1 | '2009-01-02 00:00:00' 2012918 3902112
	     28 |       1 | '2009-01-02 00:00:00' 2123797 3934290
	     29 |       1 | '2009-01-02 00:00:00' 2061812 3856800
	     30 |       1 | '2009-01-02 00:00:00' 2013435 3898403
	     32 |       1 | '2009-01-02 00:00:00' 1909819 3925662
	     33 |       1 | '2009-01-02 00:00:00' 2012838 3900850
	     36 |       1 | '2009-01-02 00:00:00' 2067413 3865059

## Find the movement of ships inside the area of Piraeus port at New Year`s eve 2009 (range querie)

This is range querie base on a rectangular spatio-temporal window.The SQL script that answers this question is listed below:

	WITH 
		TO_METERS AS (
			SELECT
				PointSP(PointLL(23.59, 37.91), HDatasetID('imis')) AS low,
				PointSP(PointLL(23.65, 37.96), HDatasetID('imis')) AS high
			),
		SPT_WINDOW AS (
			SELECT BoxST(
				Period('2008-12-31 23:00:00', '2009-01-01 01:00:00'),
				BoxSP((SELECT low FROM TO_METERS),(SELECT high FROM TO_METERS))
				) AS box
		)
	SELECT obj_id, traj_id, (atBox(seg,	(SELECT box FROM SPT_WINDOW))).s AS seg
	FROM imis_seg
	WHERE seg && (SELECT box FROM SPT_WINDOW)
	AND (atBox(seg, (SELECT box FROM SPT_WINDOW))).n = 2;
	
To answer this specific querie, we exploit on the index-supported operators @ref operators_overlaps and @ref operators_within_distance, respectively. Technically, the operator @ref operators_overlaps filters the database in order to select only those segments that overlap the spatio-temporal window defined by interval “2009-01-01 00:00:00” ± 1 hour in temporal dimension and 2-dimensional rectangle with lower-left corner (23.59E, 37.91N) and upper-right corner (23.65E, 37.96N) in spatial dimension, bounding the area of port of Piraeus in lon/lat degrees (converted to x/y meters with PointSP() method). Then, the atbox() method finds the sub-trajectories within this range @cite pelekis2014mobility.

Please note that atbox() may under certain circumstances return a (3-dimensional) point instead of a (3-dimensional) segment, such as when the intersection between the segment and the box is a point or when the segment and the period have only one timestamp in common. This explains why the method returns three properties: “n”, “p”, and “s”. In particular, “n” informs whether the result is a point (value 1) or a segment (value 2) or there is no intersection between the segment and the box (value 0). Especially for values 1 and 2 of property “n”, “p” gets the point and “s” gets the segment, respectively @cite pelekis2014mobility. Some results of the query are shown below:

	obj_id | traj_id |                                     seg                                     
	--------+---------+-----------------------------------------------------------------------------
	    251 |       1 | '2008-12-31 23:00:00' 2017310 3900853 '2008-12-31 23:00:12' 2017310 3900853
	    251 |       1 | '2008-12-31 23:00:12' 2017310 3900853 '2008-12-31 23:00:22' 2017312 3900853
	    251 |       1 | '2008-12-31 23:00:22' 2017312 3900853 '2008-12-31 23:00:32' 2017312 3900853
	    251 |       1 | '2008-12-31 23:00:32' 2017312 3900853 '2008-12-31 23:00:40' 2017312 3900853
	    251 |       1 | '2008-12-31 23:00:40' 2017312 3900853 '2008-12-31 23:00:42' 2017312 3900853
	    251 |       1 | '2008-12-31 23:00:42' 2017312 3900853 '2008-12-31 23:01:00' 2017312 3900852
	    251 |       1 | '2008-12-31 23:01:00' 2017312 3900852 '2008-12-31 23:01:22' 2017312 3900852
	    251 |       1 | '2008-12-31 23:01:22' 2017312 3900852 '2008-12-31 23:01:31' 2017314 3900852
	    251 |       1 | '2008-12-31 23:01:31' 2017314 3900852 '2008-12-31 23:01:32' 2017314 3900852
	    251 |       1 | '2008-12-31 23:01:32' 2017314 3900852 '2008-12-31 23:01:40' 2017314 3900852
	    251 |       1 | '2008-12-31 23:01:40' 2017314 3900852 '2008-12-31 23:02:22' 2017314 3900852
	    251 |       1 | '2008-12-31 23:02:22' 2017314 3900852 '2008-12-31 23:02:31' 2017314 3900852
	    251 |       1 | '2008-12-31 23:02:31' 2017314 3900852 '2008-12-31 23:02:32' 2017314 3900852
	    251 |       1 | '2008-12-31 23:02:32' 2017314 3900852 '2008-12-31 23:02:42' 2017314 3900852
	    251 |       1 | '2008-12-31 23:02:42' 2017314 3900852 '2008-12-31 23:03:20' 2017314 3900852
	    251 |       1 | '2008-12-31 23:03:20' 2017314 3900852 '2008-12-31 23:03:22' 2017314 3900852
	    251 |       1 | '2008-12-31 23:03:22' 2017314 3900852 '2008-12-31 23:03:40' 2017314 3900852
	    251 |       1 | '2008-12-31 23:03:40' 2017314 3900852 '2008-12-31 23:03:42' 2017314 3900852
	    251 |       1 | '2008-12-31 23:03:42' 2017314 3900852 '2008-12-31 23:04:22' 2017314 3900852
	    
\see [WITH queries](http://www.postgresql.org/docs/9.4/static/queries-with.html)
		
## Find the ships that approached closer than half nautical mile to Cape Sounion, Attica (range querie) ##	

This is a range querie base on a circular spatio-temporal window. The SQL script that answers this question is listed below:

	SELECT DISTINCT obj_id, traj_id
	FROM imis_seg
	WHERE seg -< RangeSP(round(nm2meters(0.5))::integer,
		PointSP(PointLL(24.025, 37.649), HDatasetID('imis'))
	);
	
The operator @ref operators_within_distance filters the database in order to select only those segments that overlap the spatial circular window defined by radius corresponding to 0.5 n.m. and center at (24.025E, 37.649N) corresponding to Cape Sounion, at the southernmost tip of the Attica peninsula. The nm2meters() function is used to transform nautical miles to meters, which is the adopted measurement unit for length in Hermes @cite pelekis2014mobility. The results of the query are shown below:

	 obj_id | traj_id 
	--------+---------
	    303 |       1
	(1 row)
	
\see @ref data_type_transformation

## Find the ships that came closer than half nautical mile from an old lighthouse in Patrai ##

The SQL script that answers this question is listed below:

	SELECT DISTINCT obj_id , traj_id
	FROM imis_seg
	WHERE seg -< RangeSP (
		round ( nm2metres (0.5) ) :: integer ,
		PointSP ( PointLL (21.72565 , 38.24513) , HDatasetID ('imis')));
		
Just as in the previous querie the operator @ref operators_within_distance filters filters the database. Notice that nm2metres() function is used to transform nautical miles to meters. Since Hermes uses 1 meter accuracy the number is rounded to the nearest integer. The results are shown below:

	 obj_id | traj_id 
	--------+---------
	    224 |       1
	    217 |       1
	    331 |       1
	    339 |       1	
	    227 |       1
	    171 |       1
	    264 |       1
	      8 |       1
	    365 |       1
	    265 |       1
	(10 rows)

	
## Find the ship that approached closest to Cape Sounion, Attica (NN) ##

The SQL script that answers this question is listed below:

	WITH 
		TO_METERS AS (
			SELECT PointSP(PointLL(24.025, 37.649), HDatasetID('imis')) AS lighthouse
			)
	SELECT obj_id, traj_id, atPoint(seg, cp, false) cp,
		distance(cp, (SELECT lighthouse FROM TO_METERS)) AS dist
	FROM (
		SELECT obj_id, traj_id, seg,
			closestPoint(getSp(seg), (SELECT lighthouse FROM TO_METERS)) AS cp
		FROM imis_seg
		ORDER BY seg <-> (SELECT lighthouse FROM TO_METERS)
		LIMIT 1
	) AS tmp;
	

The operator @ref operators_distance selects the top-1 segment with respect to its distance from a reference point (Cape Sounion: 24.025E, 37.649N). Notice that closestpoint() function finds the exact point within the trajectory segment that is closest to the reference point. Then, the atpoint() function finds the timestamp corresponding to that point. The value “false” on the last parameter of atPoint () enforces it to avoid checking for containment since we already know that the point under examination is contained on the segment. (In contrast, if the value of the third parameter is set to “yes” then the cost of the calculation gets higher.) @cite pelekis2014mobility. The results of the qurie are shown below:

	 obj_id | traj_id |                  cp                   |       dist       
	--------+---------+---------------------------------------+------------------
	    303 |       1 | '2008-12-31 21:43:29' 2058871 3874121 | 610.512080142564
	(1 row)

# Join queries # {#queries_join}

Join queries are also of great importance in trajectory databases since they join information from two datasets. Below are shown some examples mainly with self-join.


## Find pairs of ships that were located closer than 1 n.m. at 2009-01-02 11:00:00 ± 5 minutes; for each pair, find their minimum distance of one ship from the other ##

The SQL script that answers this question is listed below:

	SELECT r.obj_id AS obj_id_1, db.obj_id AS obj_id_2,
		trunc(avg(metres2nm(distance(getSp(db.seg), getSpc(r.range))))::numeric, 5)
			AS avg_dist,
		intersection(HUnion(getT(db.seg)),
		Period('2009-01-02 10:55:00', '2009-01-02 11:05:00')) AS common_period
	FROM imis_seg AS db INNER JOIN (
		SELECT obj_id, RangeST('00:05:00', getT(position),
			round(nm2metres(1))::integer, getX(position), getY(position)
				) AS range
		FROM (
			SELECT DISTINCT ON (obj_id) obj_id,
				atInstant(seg, '2009-01-02 11:00:00') AS position
			FROM imis_seg
			WHERE seg ~ '2009-01-02 11:00:00'::timestamp
			) AS timeslice
		) AS r ON db.seg -< r.range
	WHERE r.obj_id <> db.obj_id
	GROUP BY r.obj_id, db.obj_id
	ORDER BY avg_dist ASC;
	
SHOULD DOCUMENT BETTER THE FUNCTIONS IN THIS QUERY

A timeslice query is executed first and its result is used by a (spatio-temporal circular) range query that follows. The former uses '2009-01-02 11:00:00' as the reference timestamp while the latter uses each ship’s timestamp ± 5 minutes as temporal range and each ship’s location ± 1 n.m. as spatial range. Moreover, the aggregate function hunion() along with “GROUP BY” clause on the period component of the segments of a trajectory finds their union. Below are shown some results:

	obj_id_1 | obj_id_2 | avg_dist |                common_period               
	----------+----------+----------+---------------------------------------------
	      642 |      622 |  0.00535 | '2009-01-02 10:55:00' '2009-01-02 11:05:00'
	      622 |      642 |  0.00591 | '2009-01-02 10:55:00' '2009-01-02 11:05:00'
	      734 |      344 |  0.00634 | '2009-01-02 10:55:00' '2009-01-02 11:05:00'
	      129 |      235 |  0.00667 | '2009-01-02 10:55:00' '2009-01-02 11:05:00'
	      622 |      639 |  0.00763 | '2009-01-02 10:55:00' '2009-01-02 11:05:00'
	      639 |      622 |  0.00763 | '2009-01-02 10:55:00' '2009-01-02 11:05:00'
	      686 |      847 |  0.00862 | '2009-01-02 10:55:00' '2009-01-02 11:05:00'
	       78 |      177 |  0.00865 | '2009-01-02 10:55:00' '2009-01-02 11:05:00'	
	       55 |      661 |  0.00944 | '2009-01-02 10:55:00' '2009-01-02 11:05:00'
	      661 |       55 |  0.00993 | '2009-01-02 10:55:00' '2009-01-02 11:05:00'
	      656 |      847 |  0.01009 | '2009-01-02 10:55:00' '2009-01-02 11:05:00'
	      177 |       78 |  0.01053 | '2009-01-02 10:55:00' '2009-01-02 11:05:00'
	      120 |      235 |  0.01111 | '2009-01-02 10:55:00' '2009-01-02 11:05:00'
	      355 |      695 |  0.01121 | '2009-01-02 10:55:00' '2009-01-02 11:05:00'	
	      847 |      686 |  0.01133 | '2009-01-02 10:55:00' '2009-01-02 11:05:00'
	      671 |      665 |  0.01151 | '2009-01-02 10:55:00' '2009-01-02 11:05:00'
	      102 |      288 |  0.01195 | '2009-01-02 10:55:00' '2009-01-02 11:05:00'
	      
\see [PostgreSQL math functions](http://www.postgresql.org/docs/9.4/static/functions-math.html)
\see [PostgreSQL aggregate functions](http://www.postgresql.org/docs/9.4/static/functions-aggregate.html)
	

# Topological queries # {#queries_topological}

Efficient processing of topological queries makes the difference for MOD engines with respect to spatial DBMS in the sense that the former are aware of the “trajectory” concept while the latter are only able to consider trajectories as polylines in 3-dimensional space @cite pelekis2014mobility


## Find the ships that entered Patras port area. (Irrespective of time) (enter) ##

The SQL script that answers this question is listed below:

	WITH 
		TO_METERS AS (
			SELECT
				PointSP(PointLL(21.7223,38.2448), HDatasetID('imis')) AS low ,
				PointSP(PointLL(21.7394,38.2630), HDatasetID('imis')) AS high
			),
		PORT_AREA AS (
			SELECT BoxSP(
				(SELECT low FROM TO_METERS), (SELECT high FROM TO_METERS)
			) AS box
		)
	SELECT obj_id, (el).enterPoint
	FROM (
		SELECT obj_id, enter_leave(array_agg(seg), 
			(SELECT box FROM PORT_AREA )) AS el
		FROM imis_seg
		WHERE seg && ( SELECT box FROM PORT_AREA )
		GROUP BY obj_id
	) AS tmp
	WHERE (el).enterPoint IS NOT NULL ;
	
Notice the enter_leave() function which takes an array of segments of the same trajectory and returns an enter and a leave points of that trajectory in the area specified in the second argument. If there is no enter and/or leave point then the corresponding property in the result of the function will be NULL. In our example, the box corresponding to the port of Patras – and returns a structure consisting of two points: the enter and leave points of the trajectory with respect to the box (or value(s) NULL if there is no enter and/or leave point). We are only interested in the enter point of the structure denoted by (el).enterPoint (see the WHERE clause at the final line of the SQL script) @cite vodas2013hermes. The results of the query are shown below:
	
	 obj_id |              enterpoint               
	--------+---------------------------------------
	    154 | '2009-01-01 11:07:59' 1856336 3928205
	    217 | '2009-01-02 09:17:48' 1856336 3926822
	    224 | '2009-01-01 11:57:15' 1856819 3928407
	    225 | '2009-01-01 17:26:40' 1856336 3928356
	    237 | '2009-01-01 03:47:41' 1856832 3928407
	    244 | '2009-01-01 08:47:27' 1856336 3928349
	    255 | '2009-01-02 11:32:05' 1856336 3928354
	    264 | '2009-01-02 13:41:44' 1856336 3928352
	    308 | '2009-01-02 03:05:41' 1856733 3928407
	    331 | '2009-01-02 06:41:41' 1856761 3928407
	    339 | '2009-01-01 05:22:41' 1856732 3928407	
	(11 rows)
	
## Find the ships that crossed Evvoia - Andros narrow passage. (Irrespective of time) (cross) ##

The SQL script that answers this question is listed below:
	
	WITH 
		TO_METERS AS (
			SELECT
				PointSP(PointLL(24.528,37.920), HDatasetID('imis')) AS low,
				PointSP(PointLL(24.810,38.010), HDatasetID('imis')) AS high
			),
		PORT_AREA AS (
			SELECT BoxSP(
				(SELECT low FROM TO_METERS),
				(SELECT high FROM TO_METERS)
			) AS box
		)
	SELECT obj_id, (el).enterPoint, (el).leavePoint
	FROM (
		SELECT obj_id, enter_leave(array_agg(seg), 
				(SELECT box FROM PORT_AREA )) AS el
		FROM imis_seg
		WHERE seg && ( SELECT box FROM PORT_AREA )
		GROUP BY obj_id
	) AS tmp
	WHERE (el).enterPoint IS NOT NULL AND (el).leavePoint IS	NOT NULL ;	
	
We simulate cross operator by appropriately combining enter and leave operators. After defining a spatial box corresponding to Corinth Canal, the canal that separates the Peloponnesian peninsula from the Greek mainland, we again utilize the enter_leave() function. This time, we are interested in both enter and leave points, (el).enterPoint and (el).leavePoint , respectively, of the output of enter_leave(). In other words, we are looking for trajectories with a segment that enters the area and another segment that leaves the area under consideration @cite vodas2013hermes . The results of the query are shown below:
		
	 obj_id |              enterpoint               |              leavepoint               
	--------+---------------------------------------+---------------------------------------
	      1 | '2009-01-02 08:55:35' 2095263 3906319 | '2009-01-02 09:42:29' 2091984 3898443
	      5 | '2009-01-02 01:36:30' 2097977 3906319 | '2009-01-02 02:10:49' 2093733 3898443
	     19 | '2009-01-01 06:58:32' 2096760 3898443 | '2009-01-01 08:02:34' 2102495 3906319
	     28 | '2009-01-01 17:43:30' 2099663 3898443 | '2009-01-01 18:33:04' 2102225 3906319
	     29 | '2009-01-01 17:22:08' 2095028 3906319 | '2009-01-01 18:17:54' 2092426 3898443
	     30 | '2009-01-01 04:17:02' 2096315 3906319 | '2009-01-01 05:03:14' 2094294 3898443
	     32 | '2009-01-01 03:41:33' 2095657 3906319 | '2009-01-01 04:39:59' 2092536 3898443
	     36 | '2009-01-01 21:17:46' 2097907 3906319 | '2009-01-01 21:45:32' 2093978 3898443
	     42 | '2008-12-31 20:14:20' 2095266 3906319 | '2008-12-31 20:48:04' 2092560 3898443
	     48 | '2009-01-01 13:17:09' 2095253 3898443 | '2009-01-01 14:00:04' 2100845 3906319
	     51 | '2008-12-31 20:52:31' 2096592 3906319 | '2008-12-31 21:18:49' 2093695 3898443
	     54 | '2009-01-01 09:58:18' 2094850 3898443 | '2009-01-01 10:29:00' 2100331 3906319
	     56 | '2009-01-01 22:56:16' 2096889 3898443 | '2009-01-01 23:30:28' 2101104 3906319
	     62 | '2009-01-02 10:14:36' 2093185 3898443 | '2009-01-02 10:49:27' 2096634 3906319
	     69 | '2009-01-01 04:20:45' 2096876 3906319 | '2009-01-01 04:37:21' 2097197 3898443
	     77 | '2009-01-02 09:36:34' 2099694 3898443 | '2009-01-02 10:08:02' 2101671 3906319
	     78 | '2009-01-01 23:23:15' 2098476 3906319 | '2009-01-02 00:00:25' 2093416 3898443
	     79 | '2009-01-01 02:02:43' 2097129 3906319 | '2009-01-01 02:34:09' 2095059 3898443
	    146 | '2009-01-02 02:37:03' 2091776 3898443 | '2009-01-02 13:48:29' 2091505 3898443
	    159 | '2009-01-01 17:38:43' 2094036 3898443 | '2009-01-01 18:19:15' 2096228 3906319




## Find the ships that started their trip at the port of Piraeus and ended their trip at the port of Heraklion (start/end) ##

The SQL script that answers this question is listed below:

	CREATE TABLE greek_ports (
		name text NOT NULL,
		lon double precision NOT NULL,
		lat double precision NOT NULL,
		PRIMARY KEY (name)
	);

	COPY greek_ports(name, lon, lat) FROM 'greek_ports.txt' WITH CSV HEADER;

	WITH 
		TO_METERS AS (
			SELECT name, PointSP(PointLL(lon,lat), HDatasetID('imis'))
				AS port_location
			FROM greek_ports
			WHERE name IN ('PIRAEUS', 'HERAKLION')
			), 
		PORTS AS (
			SELECT name, 
				BoxSP(
					getX(port_location) - 1000,getY(port_location) - 1000, 
					getX(port_location) + 1000,getY(port_location) + 1000
				) 
				AS port_area
			FROM TO_METERS
			), 
		SE AS (
			SELECT S.name AS s_name, S.port_area AS s_area, 
				E.name AS e_name, E.port_area AS e_area
			FROM (
					SELECT name, port_area 
					FROM PORTS WHERE name = 'PIRAEUS'
				) AS S, 
				(
					SELECT name, port_area 
					FROM PORTS WHERE name = 'HERAKLION'
				) AS E
				), 
		START_END AS (
			SELECT obj_id, minT(i(seg)) AS start, maxT(e(seg)) AS end
			FROM imis_seg
			WHERE obj_id IN (
				SELECT DISTINCT obj_id
				FROM imis_seg, SE
				WHERE seg && s_area OR seg && e_area
			)
			GROUP BY obj_id
		)
	SELECT DISTINCT obj_id
	FROM SE INNER JOIN START_END ON
		contains(SE.s_area, getSp(START_END.start)) AND
		contains(SE.e_area, getSp(START_END.end));

The process for addressing the query is:

- spatial information about the locations of 100+ Greek ports is loaded (in table greek_ports);
- the two ports of interest, Piraeus and Heraklion, are selected and, foreach, a surrounding area of ± 1 km distance is defined around the point locations, called s_area and e_area, respectively;
- using these two areas and two appropriate operations, @ref operators_overlaps operator and method contains(), the trajectory dataset is pruned to find those trajectories that, on the one hand, overlap the two areas and, on the other hand, have their starting and ending points contained in the respective areas, the port of Piraeus and the port of Heraklion, respectively.

@see [Greek ports file](https://gunet2.cs.unipi.gr/modules/document/file.php/TMF103/%CE%95%CF%81%CE%B3%CE%B1%CF%83%CF%84%CE%AE%CF%81%CE%B9%CE%B1/Hermes/greek_ports.txt)


# Cross-tab queries # {#queries_cross_tab}

Cross-tab queries are, in general, more complex and more expensive than the previous ones. Nevertheless, they are very useful for analysis purposes since they provide deeper insight into the dataset under examination.

## Perform equi-sized homogeneous partitioning in space and in time (e.g. a 10x10 grid in space and a 1-day interval in time) and count the number of ships per cell ##

The SQL script that answers this question is listed below:

	SELECT t_id, x_id, y_id, count(DISTINCT obj_id)
	FROM (
		SELECT t_id, x_id, y_id, BoxST(ti, te, lx, ly, hx, hy) AS region_box
		FROM (
			SELECT row_number() OVER (ORDER BY ti) AS t_id,
				ti, ti + '24:00:00'::interval AS te
			FROM (
				SELECT generate_series(tmin, tmax, '24:00:00') AS ti
				FROM (
					SELECT date_trunc('day', tmin) AS tmin,
						date_trunc('day', tmax) AS tmax
					FROM HDatasets_Online_Statistics
					WHERE dataset = HDatasetID('imis')
				) AS tmp
			) AS tmp
		) AS t CROSS JOIN (
			SELECT row_number() OVER (ORDER BY lx) AS x_id, lx,
				lx + length AS hx
			FROM (
				SELECT generate_series(lx, hx, (hx - lx) / 10) AS lx,
					(hx - lx) / 10 AS length
				FROM HDatasets_Online_Statistics
				WHERE dataset = HDatasetID('imis')
			) AS tmp
		) AS x CROSS JOIN (
			SELECT row_number() OVER (ORDER BY ly) AS y_id, ly,
				ly + length AS hy
			FROM (
				SELECT generate_series(ly, hy, (hy - ly) / 10) AS ly,
					(hy - ly) / 10 AS length
				FROM HDatasets_Online_Statistics
				WHERE dataset = HDatasetID('imis')
			) AS tmp
		) AS y
	) AS regions LEFT JOIN imis_seg ON seg && region_box
	GROUP BY t_id, x_id, y_id;
	

In detail, we first create all 300 spatio-temporal cells, according to the following rule: triple \f$ (t_{id} , x_{id} , y_{id} )\f$ , \f$ 1 \leq t_{id} \leq 3 \f$ ,
\f$ 1 \leq x_{id} \f$,\f$ y_{id} \leq 10 \f$, corresponds to one of the 3 days and one of the 100 spatial cells, e.g. triple (2, 5, 8) corresponds to values t in the 2 nd day, x in interval \f$ (x_{min} + 4 \cdot (x_{max} - x_{min}), x_{min} + 5 \cdot (x_{max} - x_{min}))\f$, and y in interval \f$ (y_{min}+ 7 \cdot (y_{max} - y_{min}), y_{min} + 8 \cdot (y_{max} - y_{min})) \f$. Having set the partitioning, for each spatio-temporal cell, we execute a range query utilizing the operator @ref operators_overlaps in order to find the ships that were located in the specific spatial area during the specific temporal interval @cite vodas2013hermes. Some of the results of the query are shown below:

	 t_id | x_id | y_id | count 
		------+------+------+-------
	    1 |    1 |    1 |     0
	    1 |    1 |    2 |     0
	    1 |    1 |    3 |     0
	    1 |    1 |    4 |     0
	    1 |    1 |    5 |     0
	    1 |    1 |    6 |     1
	    1 |    1 |    7 |     0	
	    1 |    1 |    8 |     5
	    1 |    1 |    9 |     2
	    1 |    1 |   10 |     1
	    1 |    1 |   11 |     0
	    1 |    2 |    1 |     0
	    1 |    2 |    2 |     0
	    1 |    2 |    3 |     0
	    1 |    2 |    4 |     4
	    1 |    2 |    5 |     2
	    1 |    2 |    6 |     2
	    1 |    2 |    7 |     1
	    1 |    2 |    8 |     3	
	    1 |    2 |    9 |    15

@see [PostgreSQL date/time functions](http://www.postgresql.org/docs/9.4/static/functions-datetime.html)
@see [PostgreSQL set returning functions](http://www.postgresql.org/docs/9.4/static/functions-srf.html)
@see [PostgreSQL window functions](http://www.postgresql.org/docs/9.4/static/functions-window.html)
    
## Find the Origin-Destination Matrix between 4 large areas of the Greek territory. ##

	WITH 
		AREAS AS (
			SELECT 'North Aegean' AS name , 
				BoxSP(PointSP(PointLL(24.84,37.43), HDatasetID('imis')),
				PointSP(PointLL(27.10,40.06), HDatasetID('imis')))
			AS	area
			UNION 
			SELECT 'Piraeus', 
				BoxSP(PointSP(PointLL(23.19,37.50), HDatasetID('imis')),
				PointSP(PointLL(23.90,38.10), HDatasetID('imis')))
			UNION 
			SELECT 'Ionian - Cretan',
				BoxSP(PointSP(PointLL(21.55,	35.28), HDatasetID('imis')),
				PointSP(PointLL(23.65,36.68), HDatasetID('imis')))
			UNION 
			SELECT 'Dodecanese',
				BoxSP(PointSP(PointLL(26.39,	35.05), HDatasetID('imis')),
				PointSP(PointLL(28.57,37.32), HDatasetID('imis')))
			), 
		OD AS (
			SELECT origin.name AS o_name, origin.area AS o_area,
				destination.name AS d_name, destination.area AS d_area
			FROM AREAS AS origin 
			INNER JOIN AREAS AS destination ON origin.name <> destination.name
			), 
		START_END AS (
			SELECT obj_id,minT(i(seg)) AS start,maxT(e(seg)) AS end
			FROM imis_seg
			GROUP BY obj_id
			)
	SELECT OD.o_name, OD.d_name, count(DISTINCT START_END.obj_id ) AS nof_ships
	FROM OD LEFT JOIN START_END
		ON contains (OD.o_area, getSp(START_END.start))
			AND contains (OD.d_area, getSp(START_END.end))
	GROUP BY OD.o_name, OD.d_name
	HAVING count ( DISTINCT START_END.obj_id) > 0
	ORDER BY OD.o_name ASC , OD.d_name ASC ;
	
In this query we proceed as follows: 
- we calculate all combinations of the OD-Matrix (considering a box from 4 large areas of the Greek territory)
- for each trajectory in the database, we find its starting and ending point
- we join the two results according to the condition that the starting point of the trajectory is contained in the origin port and the ending point of the trajectory is contained in
the destination port.

The results of the query are shown below:
	
     o_name      |     d_name      | nof_ships 
	-----------------+-----------------+-----------
 	Dodecanese      | North Aegean    |        19
 	Dodecanese      | Piraeus         |         1
 	Ionian - Cretan | North Aegean    |         2
 	Ionian - Cretan | Piraeus         |         6
 	North Aegean    | Dodecanese      |        12
 	North Aegean    | Ionian - Cretan |         4
 	North Aegean    | Piraeus         |         1
 	Piraeus         | Dodecanese      |         1
 	Piraeus         | Ionian - Cretan |         1
 	Piraeus         | North Aegean    |         3
	(10 rows)
	

	