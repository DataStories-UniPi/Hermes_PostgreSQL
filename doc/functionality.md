# Functionality # {#functionality}

In this section we elaborate on the functionality of Hermes in terms of SQL functions and the algorithms that implements.

# Methods # {#methods}

The following methods can be used interchangeably either on Segments (e.g. SegmentST) or Trajectory objects, but in each case a different interpolation model is assumed (to be ckecked !!!) @cite vodas2013hermes. According to @ cite vodas2013hermes in the case of segments a uniform linear motion model is assumed and in the case of a trajectory object a non-uniform linear motion with constant non-zero acceleration between two points is used. An assumption is made on the initial speed of the object: the speed of the object at the first point of the trajectory is considered equal to the speed at the second point, in other words, the acceleration at the first segment of the trajectory is zero.

In the following, there is example code segments for the segment model mainly (kai oles oi perigrafes einai antigrafi apo tin diplomatiki tou Mariou)

## Average speed ## {#methods_average}

This function takes a segment or a trajectory as a parameter and returns the
average speed.

		SELECT averageSpeed(SegmentST('1970-1-1 0:0:0',0,0,'1970-1-1 0:0:4',0,4));
 		averagespeed 
		--------------
        		    1
		(1 row)

## At instant ## {#methods_instant}

This function takes a segment and a timestamp as parameters and returns the point where the object was found at the given timestamp.

		SELECT atInstant(SegmentST('1970-1-1 0:0:0',0,0,'1970-1-1 	0:0:4',0,4),'1970-1-1 0:0:2');
       atinstant         
		---------------------------
 		'1970-01-01 00:00:02' 0 2
		(1 row) 
		
## At point ## {#methods_atpoint}

This function takes a segment and a point as parameters and returns the timestamp at which the object was found at the given point. The point has to be on the segment, otherwise the function returns NULL.

	SELECT atPoint(SegmentST('1970-1-1 0:0:0',0,0,'1970-1-1 0:0:4',0,4),PointSP(0,0));
          atpoint          
	---------------------------
 	'1970-01-01 00:00:00' 0 0
	(1 row)
	
## At period ## {#methods_atperiod}

This function takes a segment and a period as parameters and returns the part of the segment that corresponds to the given period. The segment might have only one timestamp in common with the period so in that case the function returns a point instead of a segment. This is why the function returns three columns (n, s, p) where n is the number of common points, s is the segment within the period (if n is 2) and p is the point that the segment was within the period (if n is 1).

	SELECT n,s,p FROM atPeriod(SegmentST('1970-1-1 0:0:0',0,0,'1970-1-1 4:0:0',4,4),Period('1970-1-1 1:0:0','1970-1-1 2:0:0'));
 	n |                          s                          | p 
	---+-----------------------------------------------------+---
 	2 | '1970-01-01 01:00:00' 1 1 '1970-01-01 02:00:00' 2 2 | 
	(1 row)

## At box ## {#methods_atbox}
	
This function takes a segment and a box as parameters and returns the part of the segment that resides within the box. The n, s, and p have the same meaning as in @ref methods_atperiod.

	SELECT n,s,p FROM atBox(SegmentST('1970-1-1 0:0:0',0,0,'1970-1-1 4:0:0',4,4),BoxSP(1,1,2,2));
 	n |                          s                          | p 
	---+-----------------------------------------------------+---
 	2 | '1970-01-01 01:00:00' 1 1 '1970-01-01 02:00:00' 2 2 | 
	(1 row)

## Intersection ## {#methods_intersection}

This function takes a spatial segment and a spatial box as parameters and returns the intersection of the segment with the box. There is also a third optional parameter, called “solid”, that when is set to false the function returns NULL when the segment is fully contained within the box without touching the perimeter. The n, s, and p have the same meaning as in @ref methods_atperiod and @ref methods_atbox..

	SELECT n,s,p FROM intersection(SegmentSP(0,0,4,4),BoxSP(1,1,2,2));
 	n |    s    | p 
	---+---------+---
 	2 | 1 1 2 2 | 
	(1 row)
	
## Enter-leave points ## {#methods_enter_leave_points}

The enter leave function finds the points where the object entered or left a specific region. It takes an array of segments and a box as parameters. The function returns two columns one for the enter and one for the leave point. If one of them doesn’t exist then it returns NULL to the corresponding column.

	SELECT enterPoint , leavePoint FROM enter_leave (array_o f_segm ents [] , box_area ) ;
	