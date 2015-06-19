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
	
# Basic operators # {#operators}

In the following table the basic operators are presented. The operators rely on the methods of the previous section in order to be implemented.(kai oles oi perigrafes einai antigrafi apo tin diplomatiki tou Mariou)

Symbol  Operation                 | Returns | Left Argument | Right Argument
:---------------:                 | :-----: | :-----------: | :------------:
@ref operators_overlaps           | boolean | SegmentST     | Period, BoxSP, SegmentSP, BoxST, SegmentST
@ref operators_contains           | boolean | SegmentST     | Timestamp, Period, PointSP, PointST
@ref operators_contained          | boolean | SegmentST     | Period, BoxSP, BoxST
@ref operators_contained_properly | boolean | SegmentST     | BoxST
@ref operators_within_distance    | boolean | SegmentST     | RangeSP, RangeST
@ref operators_distance           | number  | SegmentST     | Timestamp, Period, PointSP, SegmentSP, BoxSP   

## && overlaps ## {#operators_overlaps}

The && (overlaps) operator checks if the segment has any common points (or common timespan, in the case of Period) with the object in the right of the operator. When the object in the right is of spatio-temporal type interpolate is used to find if both the spatial and temporal components interact.

## ~ contains  ## {#operators_contains}

The ∼ (contains) operator checks if the segment contains the object in the right argument. When the right argument is PointST then interpolation takes place in order to find the position the segment was at the timestamp that PointST contains and then if the position is the same as the position that PointST contains the operator returns true.

## @ contained   ## {#operators_contained}

The @ (contained) operator checks whether the segment is contained within a BoxSP (or Period, when we only check time) allowing it to touch the perimeter of the box. 

## @! contained properly ## {#operators_contained_properly}

The @! (contained properly) operator differentiates from the @ref operators_contained in that it doesn’t allow the segment to touch the perimeter (thus fully contained).

## -< within distance  ## {#operators_within_distance}

The -<(within distance) operator checks whether the distance of the segment from the center of the RangeSP is less than the radius of the RangeSP object. In the case where the right argument is a RangeST interpolation takes place before evaluating the spatial distance. Specifically, atPeriod method is called on the segment and the Period (Period is the temporal quantity that is represented in the RangeST object).

## <-> distance  ## {#operators_distance}

The <->(distance) operator returns a number, in contrast to the previous operators that return a boolean value, and shows the distance in seconds or meters from the SegmentST to the right argument. If the right argument is a temporal type the operator returns distance in seconds whereas if the argumment is a spatial type it returns in meters.
	