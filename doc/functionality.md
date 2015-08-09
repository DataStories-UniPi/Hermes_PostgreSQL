# Query functionality # {#functionality}

In this section we elaborate on the functionality of Hermes in terms of SQL functions and the algorithms that implements.

# Object methods # {#methods}

The following methods can be used interchangeably either on Segments (e.g. SegmentST) or Trajectory objects, but in each case a different interpolation model is assumed @cite vodas2013hermes. According to @cite vodas2013hermes in the case of segments a [uniform linear motion model](https://en.wikipedia.org/wiki/Linear_motion) is assumed and in the case of a trajectory object a non-uniform linear motion with constant non-zero acceleration between two points is used. An assumption is made on the initial speed of the object: the speed of the object at the first point of the trajectory is considered equal to the speed at the second point, in other words, the acceleration at the first segment of the trajectory is zero.

In the following, there is example code segments for the segment model mainly.

@see @ref SegmentST/Interpolation.sql
@see @ref SegmentST/Properties.sql
@see @ref SegmentST/RelationalOperators.sql
@see @ref SegmentST/Definition.sql

@see @ref LineSP/Definition.sql

@see @ref RangeSP/Definition.sql

@see @ref PointST/Definition.sql

@see @ref Trajectory/Properties.sql
@see @ref Trajectory/Interpolation.sql
@see @ref Trajectory/Properties.sql
@see @ref Trajectory/RelationalOperators.sql

## Average speed ## {#methods_average}

This function takes a segment or a trajectory as a parameter and returns the
average speed.

	SELECT averageSpeed(SegmentST('1970-1-1 0:0:0',0,0,'1970-1-1 0:0:4',0,4));
 	averagespeed 
	--------------
        	    1
	(1 row)
		
	SELECT averageSpeed(Trajectory(ARRAY[PointST('2008-12-31 19:29:31' :: Timestamp, PointSP(2,2)),PointST('2008-12-31 19:30:30' :: Timestamp, PointSP(3,3))]));
	    averagespeed    
	--------------------
 	0.0239697213961542
	(1 row)
	
	
## At instant ## {#methods_instant}

This function takes a segment (or a trajectory) and a timestamp as parameters and returns the point where the object was found at the given timestamp.

	SELECT atInstant(SegmentST('1970-1-1 0:0:0',0,0,'1970-1-1 	0:0:4',0,4),'1970-1-1 0:0:2');
      atinstant         
	---------------------------
 	'1970-01-01 00:00:02' 0 2
	(1 row) 
		
	SELECT atinstant(Trajectory(ARRAY[PointST('2008-12-31 19:29:32' :: Timestamp, PointSP(2,2)),PointST('2008-12-31 19:30:30' :: Timestamp, PointSP(3,3))]),'2008-12-31 19:30:00');
    atinstant         
	---------------------------
 	'2008-12-31 19:30:00' 2 2
	(1 row)
		
		
## At point ## {#methods_atpoint}

This function takes a segment (or a trajectory) and a point as parameters and returns the timestamp at which the object was found at the given point. The point has to be on the segment, otherwise the function returns NULL.

	SELECT atPoint(SegmentST('1970-1-1 0:0:0',0,0,'1970-1-1 0:0:4',0,4),PointSP(0,0));
          atpoint          
	---------------------------
 	'1970-01-01 00:00:00' 0 0
	(1 row)
	
	SELECT atPoint(Trajectory(ARRAY[PointST('2008-12-31 19:29:32' :: Timestamp, PointSP(1,1)),PointST('2008-12-31 19:30:30' :: Timestamp, PointSP(3,3))]),PointSP(2,2));
          atpoint          
	---------------------------
 	'2008-12-31 19:30:01' 2 2
	(1 row)
	
	
## closestPoint ## {#methods_closestPoint}

This function takes as input a segment and a point and returns the closest point of the segment. 

	SELECT closestPoint(SegmentSP(0,0,0,4),PointSP(1,1)); 
		closestpoint 
	--------------
 	0 1
	(1 row)
	
## pointAt ## {#methods_pointAt}

This function returns the point at a specific position of the trajectory.

	SELECT PointAt(Trajectory(ARRAY[PointST('2008-12-31 19:29:32' :: Timestamp, PointSP(1,1)),PointST('2008-12-31 19:30:40' :: Timestamp, PointSP(4,4))]),2);
          pointat          
	---------------------------
 	'2008-12-31 19:30:40' 4 4
	(1 row)

## segmentAt ## {#methods_segmentAt}

This function returns the segment at a specific position of the trajectory.

	SELECT SegmentAt(Trajectory(ARRAY[PointST('2008-12-31 19:29:32' :: Timestamp, PointSP(1,1)),PointST('2008-12-31 19:30:40' :: Timestamp, PointSP(4,4))]),1);
                      segmentat                      
	-----------------------------------------------------
 	'2008-12-31 19:29:32' 1 1 '2008-12-31 19:30:40' 4 4
	(1 row)

## firstPoint ## {#methods_firstPoint}

This function returns the first point of a trajectory.

	SELECT firstPoint(Trajectory(ARRAY[PointST('2008-12-31 19:29:32' :: Timestamp, PointSP(1,1)),PointST('2008-12-31 19:30:40' :: Timestamp, PointSP(4,4))]));
        firstpoint         
	---------------------------
 	'2008-12-31 19:29:32' 1 1
	(1 row)

## lastPoint ## {#methods_lastPoint}

This function returns the last point of a trajectory.

	SELECT lastPoint(Trajectory(ARRAY[PointST('2008-12-31 19:29:32' :: Timestamp, PointSP(1,1)),PointST('2008-12-31 19:30:40' :: Timestamp, PointSP(4,4))]));
         lastpoint         
	---------------------------
 	'2008-12-31 19:30:40' 4 4
	(1 row)

## firstSegment ## {#methods_firstSegment}

This function returns the first segment of the trajectory.

	SELECT firstSegment(Trajectory(ARRAY[PointST('2008-12-31 19:29:32' :: Timestamp, PointSP(1,1)),PointST('2008-12-31 19:30:40' :: Timestamp, PointSP(4,4))]));
                    firstsegment                     
	-----------------------------------------------------
 	'2008-12-31 19:29:32' 1 1 '2008-12-31 19:30:40' 4 4
	(1 row)


## lastSegment ## {#methods_lastSegment}

This function returns the last segment of the trajectory.

	SELECT lastSegment(Trajectory(ARRAY[PointST('2008-12-31 19:29:32' :: Timestamp, PointSP(1,1)),PointST('2008-12-31 19:30:40' :: Timestamp, PointSP(4,4))]));
                     lastsegment                     
	-----------------------------------------------------
 	'2008-12-31 19:29:32' 1 1 '2008-12-31 19:30:40' 4 4
	(1 row)


## sub ## {#methods_sub}

This function returns the sub-trajectory from a trajectory specified by the two integers.

	SELECT sub(Trajectory(ARRAY[PointST('2008-12-31 19:29:32' :: Timestamp, PointSP(1,1)),PointST('2008-12-31 19:30:40' :: Timestamp, PointSP(4,4))]),1,2);
                         sub                         
	-----------------------------------------------------
 	'2008-12-31 19:29:32' 1 1,'2008-12-31 19:30:40' 4 4
	(1 row)


## At period ## {#methods_atperiod}

This function takes a segment (or a trajectory) and a period as parameters and returns the part of the segment that corresponds to the given period. 

	SELECT n,s,p FROM atPeriod(SegmentST('1970-1-1 0:0:0',0,0,'1970-1-1 4:0:0',4,4),Period('1970-1-1 1:0:0','1970-1-1 2:0:0'));
 	n |                          s                          | p 
	---+-----------------------------------------------------+---
 	2 | '1970-01-01 01:00:00' 1 1 '1970-01-01 02:00:00' 2 2 | 
	(1 row)
	
The segment might have only one timestamp in common with the period so in that case the function returns a point instead of a segment. This is why the function returns three columns (n, s, p) where n is the number of common points, s is the segment within the period (if n is 2) and p is the point that the segment was within the period (if n is 1).
	
	SELECT * FROM atPeriod(Trajectory(ARRAY[PointST('2008-12-31 19:29:32' :: Timestamp, PointSP(1,1)),PointST('2008-12-31 19:30:30' :: Timestamp, PointSP(3,3))]),Period('2008-12-31 19:29:32','2008-12-31 19:30:0'));
                      atperiod                       
	-----------------------------------------------------
 	'2008-12-31 19:29:32' 1 1,'2008-12-31 19:30:00' 2 2
	(1 row)

In the case of the trajectory also a trajectory is returned. 

## At box ## {#methods_atbox}
	
This function takes a segment and a box as parameters and returns the part of the segment that resides within the box. The parameters have the same meaning as in @ref methods_atperiod.

	SELECT n,s,p FROM atBox(SegmentST('1970-1-1 0:0:0',0,0,'1970-1-1 4:0:0',4,4),BoxSP(1,1,2,2));
 	n |                          s                          | p 
	---+-----------------------------------------------------+---
 	2 | '1970-01-01 01:00:00' 1 1 '1970-01-01 02:00:00' 2 2 | 
	(1 row)
	
	SELECT * FROM atBox(Trajectory(ARRAY[PointST('2008-12-31 19:29:32' :: Timestamp, PointSP(1,1)),PointST('2008-12-31 19:30:30' :: Timestamp, PointSP(3,3))]),BoxSP(1,1,2,2));
                         tr                          
	-----------------------------------------------------
 	'2008-12-31 19:29:32' 1 1,'2008-12-31 19:30:01' 2 2
	(1 row)

## Intersection ## {#methods_intersection}

This function takes a spatial segment and a spatial box as parameters and returns the intersection of the segment with the box. There is also a third optional parameter, called “solid”, that when is set to false the function returns NULL when the segment is fully contained within the box without touching the perimeter. The n, s, and p have the same meaning as in @ref methods_atperiod and @ref methods_atbox.

	SELECT n,s,p FROM intersection(SegmentSP(0,0,4,4),BoxSP(1,1,2,2));
 	n |    s    | p 
	---+---------+---
 	2 | 1 1 2 2 | 
	(1 row)
	
## Contain ## {#methods_contains}

This function checks if an object contains an another object.

	postgres=# SELECT Contains(BoxSP(2337709, 4163887,3228259, 4721671),PointSP(1,1));
 	contains 
	----------
 	f
	(1 row)
	
## Duration ## {#methods_duration}

This function returns the duration of a trajectory.

	SELECT duration(Trajectory(ARRAY[PointST('2008-12-31 19:29:32' :: Timestamp, PointSP(1,1)),PointST('2008-12-31 19:30:30' :: Timestamp, PointSP(3,3))]));
	 duration 
	----------
 	00:00:58
	(1 row)	

## Length ## {#methods_length}

This function returns the length of a trajectory.

	SELECT length(Trajectory(ARRAY[PointST('2008-12-31 19:29:32' :: Timestamp, PointSP(1,1)),PointST('2008-12-31 19:30:30' :: Timestamp, PointSP(3,3))]));
      length      
	------------------
 	2.82842712474619
	(1 row)

## Displacement ## {#methods_displacement}

This function returns the displacement of a trajectory.  -- PENDING!! --

	SELECT displacement(Trajectory(ARRAY[PointST('2008-12-31 19:29:32' :: Timestamp, PointSP(1,1)),PointST('2008-12-31 19:30:30' :: Timestamp, PointSP(3,3))]));
   	displacement   
	------------------
 	2.82842712474619
	(1 row)

## Centroid ## {#methods_centroid}

This function returns the centroid of a trajectory.

	SELECT centroid(Trajectory(ARRAY[PointST('2008-12-31 19:29:32' :: Timestamp, PointSP(1,1)),PointST('2008-12-31 19:30:30' :: Timestamp, PointSP(3,3))]));
         centroid          
	---------------------------
 	'2008-12-31 19:30:01' 2 2
	(1 row)

## Masscenter ## {#methods_masscenter}

This function returns the masscenter of a trajectory. -- PENDING!! --

	SELECT masscenter(Trajectory(ARRAY[PointST('2008-12-31 19:29:32' :: Timestamp, PointSP(1,1)),PointST('2008-12-31 19:30:30' :: Timestamp, PointSP(3,3))]));
 	masscenter 
	------------
 	2 2
	(1 row)

## Gyradius ## {#methods_gyradius}

This function returns the [radius of gyration](https://en.wikipedia.org/wiki/Radius_of_gyration) of a trajectory. -- PENDING!! --
	
	SELECT gyradius(Trajectory(ARRAY[PointST('2008-12-31 19:29:32' :: Timestamp, PointSP(1,1)),PointST('2008-12-31 19:30:30' :: Timestamp, PointSP(3,3))]));
    gyradius     
	-----------------
 	1.4142135623731
	(1 row)

## anglexxavg ## {#methods_anglexxavg}

This function returns the average direction of a trajectory.

	SELECT anglexxavg(Trajectory(ARRAY[PointST('2008-12-31 19:29:32' :: Timestamp, PointSP(1,1)),PointST('2008-12-31 19:30:30' :: Timestamp, PointSP(3,3))]));
    anglexxavg     
	-------------------
 	0.785398163397448
	(1 row)

## anglexx ## {#methods_anglexx}

-- PENDING!! --

	SELECT anglexx(Trajectory(ARRAY[PointST('2008-12-31 19:29:32' :: Timestamp, PointSP(1,1)),PointST('2008-12-31 19:30:30' :: Timestamp, PointSP(3,3))]));
      anglexx      
	-------------------
 	0.785398163397448
	(1 row)
	
## samplingPeriod ## {#methods_samplingPeriod}

-- PENDING!! --

	SELECT SamplingPeriod(Trajectory(ARRAY[PointST('2008-12-31 19:29:32' :: Timestamp, PointSP(1,1)),PointST('2008-12-31 19:30:40' :: Timestamp, PointSP(4,4))]));
 	samplingperiod 
	----------------
 	00:01:08
	(1 row)

## normalizedsamplingrate ## {#methods_normalizedsamplingrate}

-- PENDING!! --

	SELECT normalizedSamplingRate(Trajectory(ARRAY[PointST('2008-12-31 19:29:32' :: Timestamp, PointSP(1,1)),PointST('2008-12-31 19:30:30' :: Timestamp, PointSP(3,3))]));
 	normalizedsamplingrate 
	------------------------
     0.0344827586206897
	(1 row)
	
## size ## {#methods_size}

This function returns the size of the trajectory in bytes.

	SELECT size(Trajectory(ARRAY[PointST('2008-12-31 19:29:32' :: Timestamp, PointSP(1,1))]));
 	size 
	------
   	32
	(1 row)

## NrPoints ## {#methods_NrPoints}

This functions returns the number of points in a trajectory.
	
	SELECT NrPoints(Trajectory(ARRAY[PointST('2008-12-31 19:29:32' :: Timestamp, PointSP(1,1)),PointST('2008-12-31 19:30:40' :: Timestamp, PointSP(4,4))]));
 	nrpoints 
	----------
    	    2
	(1 row)

## NrSegments ## {#methods_NrSegments}

This function returns the number of segments in a trajectory.
	
	SELECT NrSegments(Trajectory(ARRAY[PointST('2008-12-31 19:29:32' :: Timestamp, PointSP(1,1)),PointST('2008-12-31 19:30:40' :: Timestamp, PointSP(4,4))]));
 	nrsegments 
	------------
          1
	(1 row)
	
## get ## {#methods_get}

This category of functions returns the attributes of an object.

Function                | Returns
:---------------------: | :-----------:
getT(segment SegmentST) | the period of the SegmentST
getTi(segment SegmentST)| the start time of the period of the SegmentST
getTe(segment SegmentST)| the end time of the period of the SegmentST
getSp(segment SegmentST)| the Spatial Segment of the SegmentST
getI(segment SegmentST) | the start of the  Spatial Segment of the SegmentST
getIx(segment SegmentST)| the x coordinate of the start of the  Spatial Segment of the SegmentST
getIy(segment SegmentST)| the y coordinate of the start of the  Spatial Segment of the SegmentST
getE(segment SegmentST) | the end of the  Spatial Segment of the SegmentST
getEx(segment SegmentST)| the x coordinate of the end of the  Spatial Segment of the SegmentST
getEy(segment SegmentST)| the y coordinate of the end of the  Spatial Segment of the SegmentST
getA(line LineSP)       | the a parameter of the LineSP
getB(line LineSP)       | the b parameter of the LineSP
getC(line LineSP)       | the c parameter of the LineSP
getR(range RangeSP)     | the radius of the RangeSP
getC(range RangeSP)     | the center of the RangeSP
getX(range RangeSP)     | the x coordinate of the center of the RangeSP
getY(range RangeSP)     | the y coordinate of the center of the RangeSP
getT(point PointST)     | the timestamp of the PointST
getSp(point PointST)    | the coordinates of the PointST
getX(point PointST)     | the x coordinate of the PointST
getY(point PointST)     | the y coordinate of the PointST

Some examples are shown below: 

	SELECT getT(SegmentST('1970-1-1 0:0:0',0,0,'1970-1-1 4:0:0',4,4));
 	gett
	---------------------------------------------
 	'1970-01-01 00:00:00' '1970-01-01 04:00:00'
	(1 row)
	
	SELECT getI(SegmentST('1970-1-1 0:0:0',0,0,'1970-1-1 4:0:0',4,4));
 	geti 
	------
 	0 0
	(1 row)
	
	SELECT getSp(PointST('2008-12-31 19:29:31' :: Timestamp, PointSP(2,2)));
 	getsp 
	-------
 	2 2
	(1 row)
	
	SELECT getT(PointST('2008-12-31 19:29:31' :: Timestamp, PointSP(2,2)));
        gett         
	---------------------
 	2008-12-31 19:29:31
	(1 row)

@see @ref SegmentST/Definition.sql
@see @ref LineSP/Definition.sql
@see @ref RangeSP/Definition.sql
@see @ref PointST/Definition.sql

# Functions & Operators # {#operators}

In the following table the basic operators are presented. The operators rely on the methods of the previous section in order to be implemented.

Symbol  Operation                 | Returns | Left Argument | Right Argument
:---------------:                 | :-----: | :-----------: | :------------:
@ref operators_overlaps           | boolean | SegmentST     | Period, BoxSP, SegmentSP, BoxST, SegmentST
@ref operators_contains           | boolean | SegmentST     | Timestamp, Period, PointSP, PointST
@ref operators_contained          | boolean | SegmentST     | Period, BoxSP, BoxST
@ref operators_contained_properly | boolean | SegmentST     | BoxST
@ref operators_within_distance    | boolean | SegmentST     | RangeSP, RangeST
@ref operators_distance           | number  | SegmentST     | Timestamp, Period, PointSP, SegmentSP, BoxSP   

## && overlaps ## {#operators_overlaps}

The && (overlaps) operator checks if the segment has any common points (or common timespan, in the case of Period) with the object in the right of the operator. When the object in the right is of spatio-temporal type interpolation is used to find if both the spatial and temporal components interact.

For example some of the questions that someone can answer with  @ref operators_overlaps operator are:

### Find the movement of ships inside the area of Piraeus port at New Year`s eve 2009 (range query) ###

This is range query base on a rectangular spatio-temporal window.The SQL script that answers this question is listed below:

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
	
To answer this specific query, we exploit on the index-supported operators @ref operators_overlaps. Technically, the operator @ref operators_overlaps filters the database in order to select only those segments that overlap the spatio-temporal window defined by interval “2009-01-01 00:00:00” ± 1 hour in temporal dimension and 2-dimensional rectangle with lower-left corner (23.59E, 37.91N) and upper-right corner (23.65E, 37.96N) in spatial dimension, bounding the area of port of Piraeus in lon/lat degrees (converted to x/y meters with PointSP() method). Then, the atbox() method finds the sub-trajectories within this range @cite pelekis2014mobility.

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
	    
@see [WITH queries](http://www.postgresql.org/docs/9.4/static/queries-with.html)

### Find the ships that started their trip at the port of Piraeus and ended their trip at the port of Heraklion (start/end) ###

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
- using these two areas and two appropriate operations, @ref operators_overlaps operator and @ref methods_contains, the trajectory dataset is pruned to find those trajectories that, on the one hand, overlap the two areas and, on the other hand, have their starting and ending points contained in the respective areas, the port of Piraeus and the port of Heraklion, respectively.

@see [Greek ports file](https://gunet2.cs.unipi.gr/modules/document/file.php/TMF103/%CE%95%CF%81%CE%B3%CE%B1%CF%83%CF%84%CE%AE%CF%81%CE%B9%CE%B1/Hermes/greek_ports.txt)

## ~ contains  ## {#operators_contains}

The \latexonly ~ \endlatexonly  \htmlonly ~ \endhtmlonly (contains) operator checks if the segment contains the object in the right argument. When the right argument is PointST then interpolation takes place in order to find the position the segment was at the timestamp that PointST contains and then if the position is the same as the position that PointST contains the operator returns true.

For example some of the questions that someone can answer with  @ref operators_contains operator are:

### Find the position of all ships on the second day of 2009, midnight (timeslice query) ### 

The SQL script that answers this question is listed below:

	SELECT DISTINCT ON ( obj_id , traj_id ) obj_id , traj_id ,
		atInstant ( seg , '2009-01-02 00:00:00') AS position
	FROM imis_seg
	WHERE seg ~ '2009-01-02 00:00:00' :: timestamp ;
	
The index-supported operator @ref operators_contains filters the database in order to select only those segments that contain the given timestamp “2009-01-01 00:00:00” in their temporal dimension. Then, the @ref atinstant() method finds the exact location of objects at the given timestamp @cite pelekis2014mobility. Some results of the query are shown below:
	
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

## \@ contained ## {#operators_contained}

The \@ (contained) operator checks whether the segment is contained within a BoxSP (or Period, when we only check time) allowing it to touch the perimeter of the box. 

## \@! contained properly ## {#operators_contained_properly}

The \@! (contained properly) operator differentiates from the @ref operators_contained in that it doesn’t allow the segment to touch the perimeter (thus fully contained).

## -< within distance  ## {#operators_within_distance}

The -<(within distance) operator checks whether the distance of the segment from the center of the RangeSP is less than the radius of the RangeSP object. In the case where the right argument is a RangeST interpolation takes place before evaluating the spatial distance. Specifically, atPeriod method is called on the segment and the Period (Period is the temporal quantity that is represented in the RangeST object).

### Find the ships that approached closer than half nautical mile to Cape Sounion, Attica (range query) ###	

This is a range query base on a circular spatio-temporal window. The SQL script that answers this question is listed below:

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

### Find the ships that approached closer than half nautical mile to Cape Sounion, Attica (range query) ###

This is a range query base on a circular spatio-temporal window. The SQL script that answers this question is listed below:

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

## <-> distance  ## {#operators_distance}

The <->(distance) operator returns a number, in contrast to the previous operators that return a boolean value, and shows the distance in seconds or meters from the SegmentST to the right argument. If the right argument is a temporal type the operator returns distance in seconds whereas if the argumment is a spatial type it returns in meters.

For example some of the questions that someone can answer with  @ref operators_distance operator are:

### Find the ships that came closer than half nautical mile from an old lighthouse in Patrai ###

The SQL script that answers this question is listed below:

	SELECT DISTINCT obj_id , traj_id
	FROM imis_seg
	WHERE seg -< RangeSP (
		round ( nm2metres (0.5) ) :: integer ,
		PointSP ( PointLL (21.72565 , 38.24513) , HDatasetID ('imis')));
		
Just as in the previous query the operator @ref operators_within_distance filters filters the database. Notice that nm2metres() function is used to transform nautical miles to meters. Since Hermes uses 1 meter accuracy the number is rounded to the nearest integer. The results are shown below:

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
	
### Find the ship that approached closest to Cape Sounion, Attica (NN) ###

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
	
## Hunion ## {#methods_hunion}

Like most other relational database products, Hermes MOD supports aggregate functions. An aggregate function computes a single result from multiple input rows. For example, there are aggregates to compute the count, sum, avg (average), max (maximum) and min (minimum) over a set of rows.


	postgres=# SELECT hunion (BoxSP(2337709, 4163887,3228259, 4721671),PointSP(1,1));
       hunion        
	---------------------
 	1 1 3228259 4721671
	(1 row)
	
For example some of the questions that someone can answer with  @ref methods_hunion function are:

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
	
## Enter-leave points ## {#methods_enter_leave_points}

The enter-leave function finds the points where the object entered or left a specific region. It takes an array of segments and a box as parameters. The function returns two columns one for the enter and one for the leave point. If one of them doesn’t exist then it returns NULL to the corresponding column.

	SELECT enterPoint , leavePoint FROM enter_leave (array_o f_segm ents [] , box_area ) ;
	
For example some of the questions that someone can answer with  @ref methods_enter_leave_points function are:
	
### Find the ships that entered Patras port area. (Irrespective of time) (enter) ###

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
	
Notice the @ref methods_enter_leave_points function which takes an array of segments of the same trajectory and returns an enter and a leave point of that trajectory in the area specified in the second argument. If there is no enter and/or leave point then the corresponding property in the result of the function will be NULL. In our example, the box corresponding to the port of Patras – and returns a structure consisting of two points: the enter and leave points of the trajectory with respect to the box (or value(s) NULL if there is no enter and/or leave point). We are only interested in the enter point of the structure denoted by (el).enterPoint (see the WHERE clause at the final line of the SQL script) @cite vodas2013hermes. The results of the query are shown below:
	
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
	
@see @ref [array_agg](http://www.postgresql.org/docs/9.4/static/functions-aggregate.html)
	
### Find the ships that crossed Evvoia - Andros narrow passage. (Irrespective of time) (cross) ###

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
	
We simulate cross operator by appropriately combining enter and leave operators. After defining a spatial box corresponding to Corinth Canal, the canal that separates the Peloponnesian peninsula from the Greek mainland, we again utilize the @ref methods_enter_leave_points function. This time, we are interested in both enter and leave points, (el).enterPoint and (el).leavePoint , respectively, of the output of @ref methods_enter_leave_points function. In other words, we are looking for trajectories with a segment that enters the area and another segment that leaves the area under consideration @cite vodas2013hermes . The results of the query are shown below:
		
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
	    
# Advanced queries # {#queries_advanecd}

Below some more advanced queries are shown.

## Grid partinioning ## {#queries_grid}

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

## Origin-destination matrix ## {#queries_od_matrix}
    
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
	
	
##  Trajectory Buffer Query (TBQ) ## {#queries_tbq}

TBQ takes as as input a trajectory and a set of trajectories and returns a voting vector. 
Each element of this vector corresponds to the voting that a specific element has received, for example the ith element of the voting vector correspond to the voting of the ith segment. 
More specifically, it takes as input a trajectory, calculates its trajectory buffer (i.e. its spatial enlargement in space), returns the segments of all other trajectories that overlap with it and calculates its voting.

	﻿SET enable_seqscan = off;--Forcing the system to use the index. We need this because TBQ is index based

	SELECT S2T_TemporalBufferSize('00:00:00'); --Setting the Temporal Buffer Size (00:00:00 since we use the trapezoidal distance function)
	SELECT S2T_SpatialBufferSize(10000); --Setting the Spatial Buffer Size
	SELECT S2T_Sigma(10000); --Setting Sigma. Sigma shows how fast the function of the "voting influence” decreases with distance.
	SELECT S2T_VotingMethod('Trapezoidal'); --Setting the distance function

	SELECT 1
	FROM ONLY imis_seg
	WHERE seg &&& 
	(
		SELECT trajectory_agg(seg ORDER BY getTi(seg) ASC) FROM imis_seg WHERE (obj_id, traj_id) = (215171000, 2)--i, j are the object_id and trajectory_id respectively of the input trajectory. 
	);

	SELECT array_agg(normalized_voting ORDER BY ordinality ASC)
	FROM S2T_VotingSignal() WITH ORDINALITY;
	
Below is shown the the execution of the queries:
	
	SET enable_seqscan = off;
	SET
	
	SELECT S2T_TemporalBufferSize('00:00:00'); 
	 s2t_temporalbuffersize 
	------------------------
	
	(1 row)
	
	SELECT S2T_SpatialBufferSize(10000);
 	s2t_spatialbuffersize 
	-----------------------
 
	(1 row)
	
	SELECT S2T_VotingMethod('Trapezoidal');
 	s2t_votingmethod 
	------------------
 
	(1 row)
	
	SELECT 1
	FROM ONLY imis_seg
	WHERE seg &&& 
	(
		SELECT trajectory_agg(seg ORDER BY getTi(seg) ASC) FROM imis_seg WHERE (obj_id, traj_id) = (215171000, 2)
	);
	
	 ?column? 
	----------
	(0 rows)
	
	
	SELECT array_agg(normalized_voting ORDER BY ordinality ASC)
	FROM S2T_VotingSignal() WITH ORDINALITY;
	 array_agg 
	-----------
 
	(1 row)
		
	
@see http://infolab.cs.unipi.gr/technical_reports/TR-2013-02.pdf
