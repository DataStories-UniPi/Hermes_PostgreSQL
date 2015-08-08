
# Data types {#data_types}

In this section, the data types supported by Hermes are presented in the following table.

Category        | Data types
:-----------:   | :-----------:
Temporal        | [Timestamp](http://www.postgresql.org/docs/9.4/static/datatype-datetime.html) (Datetime t) 
Temporal        | [Interval](http://www.postgresql.org/docs/9.4/static/datatype-datetime.html)  
Temporal        | Period (Timestamp initial, Timestamp ending) 
Temporal        | RangeT (Interval radius, Timestamp center)
Spatial         | PointSP (number x, number y) 
Spatial         | LineSP (double A, double B, double C)
Spatial         | BoxSP (PointSP lower-left, PointSP upper-right)
Spatial         | SegmentSP (PointSP initial, PointSP ending)
Spatial         | RangeSP (number radius, PointSP center)
Spatial         | PointLL (float8 lon, float8 lat)
Spatial         | PointXY (float8 x, float8 y)
Spatio-temporal | PointST (Timestamp t, PointSP p)
Spatio-temporal | BoxST (PeriodT period, BoxSP box)
Spatio-temporal | SegmentST (PeriodT period, SegmentSP segment)
Spatio-temporal | RangeST (RangeT t-range, RangeSP sp-range)
Spatio-temporal | Trajectory (PointST[]) 

![Hermes Data Types](data_types.png)
@image latex data_types.png "Hermes Data Types" 

# Temporal data types {#data_types_temporal}

According to @cite vodas2013hermes temporal data types are those types that model only the temporal dimension of Mobility. The main type for temporal data type is introduced by PostrgreSQL and Hermes encapsulates it in its data model. An example of a timestamp is ‘2012-09-20 08:05:46’. Another encapsulated data type is the Interval which is used to store information like ‘1 second’ or ‘05:30:5’ (which means 5 hours 30 minutes and 5 seconds) hence contains a temporal quantity. One potential limitation of interval is that on cases where it was produced from a subtraction of two timestamps it won’t keep the original timestamps from which it was computed. That limitation led to the development of some custom temporal types in Hermes @cite vodas2013hermes.

The user must have in his mind for using the timestamp data type that PostgreSQL can cast a string to any datatype. For example:

	postgres=# SELECT '2008-12-31 19:29:30' :: Timestamp ;
      timestamp      
	---------------------
 	2008-12-31 19:29:30
	(1 row)
	
	postgres=# SELECT '1 month'::interval;
 	interval 
	----------
 	1 mon
	(1 row)
	
A timestamp is given as a string and is cast using “::” to the timestamp data type. This is also possible for the interval data type. Finally Hermes introduced the @ref Period and @ref RangeT temporal data type.

	postgres=# SELECT Period('2008-12-31 23:00:00', '2009-01-01 01:00:00');
                   period                    
	---------------------------------------------
 	'2008-12-31 23:00:00' '2009-01-01 01:00:00'
	(1 row)
	
	postgres=# SELECT RangeT('1 month'::interval, '2008-12-31'::Timestamp) ;
	            ranget             
	-------------------------------
 	'1 mon' '2008-12-31 00:00:00'
	(1 row)
	
@see @ref Timestamp/RelationalOperators.sql

@see @ref Period/Casts.sql
@see @ref Period/Definition.sql
@see @ref Period/Distance.sql
@see @ref Period/Interactions.sql
@see @ref Period/Properties.sql
@see @ref Period/RelationalOperators.sql

@see @ref RangeT/Casts.sql
@see @ref RangeT/Definition.sql

@see @ref Temporal/SessionParameters.sql
	
# Spatial data types {#data_types_spatial}

According to @cite vodas2013hermes spatial data types are those types that model only the spatial dimension of Mobility. Hermes is designed to work with data in the Eyclidean space, which means that a position defined in Riemannian geometry (or any other geometry) can`t be used. Usually the Cartesian system is used and the coordinates are measured in meters. **To be precise due to the fact that integers are used to represent the main spatial data type, the unit length that must be used is the same with the precision we want to achieve.** So if we want to have a precision of 1 meters we should store in the coordinates in the database in meters, but if we want to achieve a centimeter accuracy, we should store the coordinates in the database as centimeters. 

The main spatial data types are the @ref PointSP and the @ref SegmentSP

![Spatial segment](spatial_segment.png)
@image latex spatial_segment.png "Spatial segment" 

The previous data types do not have a surface. In contrast, the @ref BoxSP data type has a surface

![Spatial box](spatial_box.png)
@image latex spatial_box.png "Spatial box" 

Some examples of using the saptial data types are shown below:

	postgres=# SELECT PointSP(2247569,4792246);
     		pointsp     
	-----------------
 	2247569 4792246
	(1 row)
	
	postgres=# SELECT '2247569 4792246'::pointsp;
     		pointsp     
	-----------------
 	2247569 4792246
	(1 row)

	postgres=# SELECT BoxSP(2337709, 4163887,3228259, 4721671) ;
			boxsp              
	---------------------------------
 	2337709 4163887 3228259 4721671
	(1 row)
	
	postgres=# SELECT SegmentSP(PointSP(2337709, 4163887),PointSP(3228259, 4721671)) ;
            segmentsp            
	---------------------------------
 	2337709 4163887 3228259 4721671
	(1 row)
	
	postgres=# SELECT '2247569 4792246 2246943 4782504'::segmentsp;
            segmentsp            
	---------------------------------
 	2247569 4792246 2246943 4782504
	(1 row)

	postgres=# SELECT BoxSP(PointSP(2337709, 4163887),PointSP(3228259, 4721671)) ;
              boxsp              
	---------------------------------
 	2337709 4163887 3228259 4721671
	(1 row)
	
	postgres=# SELECT RangeSP(1,1,1);
 		rangesp 
	---------
 	1 1 1
	(1 row)
	
	postgres=# SELECT LineSP(0.1,0.1,1);
    	       linesp           
	----------------------------
 	0.100000 0.100000 1.000000
	(1 row)
	
@see @ref PointSP/Definition.sql
@see @ref PointSP/Distance.sql
@see @ref PointSP/RelationalOperators.sql
@see @ref PointSP/Casts.sql

@see @ref SegmentSP/ArithmeticOperators.sql
@see @ref SegmentSP/Casts.sql
@see @ref SegmentSP/Definition.sql
@see @ref SegmentSP/Distance.sql
@see @ref SegmentSP/Interactions.sql
@see @ref SegmentSP/Properties.sql
@see @ref SegmentSP/RelationalOperators.sql

@see @ref BoxSP/Casts.sql
@see @ref BoxSP/Definition.sql
@see @ref BoxSP/Distance.sql
@see @ref BoxSP/Interactions.sql
@see @ref BoxSP/Properties.sql
@see @ref BoxSP/RelationalOperators.sql

@see @ref RangeSP/Casts.sql
@see @ref RangeSP/Definition.sql
@see @ref RangeSP/Properties.sql

@see @ref LineSP/ArithmeticOperators.sql
@see @ref LineSP/Definition.sql
@see @ref LineSP/Properties.sql
@see @ref LineSP/RelationalOperators.sql

@see @ref Spatial/SessionParameters.sql
	
# Spatio-temporal data types {#data_types_spatio_temp}

According to @cite vodas2013hermes spatio-temporal data types are those types that model both the temporal and spatial dimension of Mobility in a unified manner. The main spatio-temporal data types are @ref PointST, @ref SegmentST and @ref BoxST

Some examples of using the spatio-temporal data types are shown below:

	postgres=# SELECT PointST('2008-12-31 19:29:30' :: Timestamp, PointSP(1,1));
    	      pointst          
	---------------------------
 	'2008-12-31 19:29:30' 1 1
	(1 row)
	
	postgres=# SELECT BoxST(Period('2008-12-31 23:00:00', '2009-01-01 01:00:00'), 
		BoxSP(PointSP(2337709, 4163887),PointSP(3228259, 4721671)));                                    
			boxst		                                    
	-----------------------------------------------------------------------------
 	'2008-12-31 23:00:00' '2009-01-01 01:00:00' 2337709 4163887 3228259 4721671
	(1 row)
	
	postgres=# SELECT SegmentST('2008-12-31 19:29:30' :: Timestamp,1,1,'2008-12-31 19:29:31' :: Timestamp,2,2);
                      segmentst                      
	-----------------------------------------------------
 	'2008-12-31 19:29:30' 1 1 '2008-12-31 19:29:31' 2 2
	(1 row)

	postgres=# SELECT RangeST(RangeT('1 month'::interval, '2008-12-31'::Timestamp),RangeSP(1,1,1));
               rangest               
	-------------------------------------
 	'1 mon' '2008-12-31 00:00:00' 1 1 1
	(1 row)
		
@see @ref PointST/Aggregates.sql
@see @ref PointST/Casts.sql
@see @ref PointST/Definition.sql

@see @ref BoxST/Casts.sql
@see @ref BoxST/Definition.sql
@see @ref BoxST/Editors.sql
@see @ref BoxST/Interactions.sql
@see @ref BoxST/Properties.sql
@see @ref BoxST/PseudoDistance.sql
@see @ref BoxST/PseudoRelationalOperators.sql
@see @ref BoxST/RelationalOperators.sql
@see @ref BoxST/Indexing/GiST.sql

@see @ref SegmentST/Accessors.sql
@see @ref SegmentST/Casts.sql
@see @ref SegmentST/Definition.sql
@see @ref SegmentST/Interpolation.sql
@see @ref SegmentST/Properties.sql
@see @ref SegmentST/PseudoDistance.sql
@see @ref SegmentST/PseudoRelationalOperators.sql
@see @ref SegmentST/RelationalOperators.sql
@see @ref SegmentST/Similarity.sql
@see @ref SegmentST/Indexing/GiST.sql
@see @ref SegmentST/Indexing/S2T.sql
@see @ref EnterLeave.sql

@see @ref RangeST/Casts.sql
@see @ref RangeST/Definition.sql


# Trajectory type {#data_type_trajectory}

Trajectory data type is an object containing a sequence of spatio-temporal points ordered in time. In contrast with the previous data types, it has a variable length and is compromised of sequence of PointST objects ordered by time @cite vodas2013hermes. The different approach to this data type is that we look at the movement of an object as a whole and not as segments in smaller parts. 

Trajectory is a dynamic data type consisted of several PointST. For this reason constructing  trajectory is not a straightforward procedure.

	postgres=# SELECT TrajCache_Allocate(3);
 	trajcache_allocate 
	--------------------
    	              0
	(1 row)

Initially the appropriate space in memory is allocated by using the TrajCache_Allocate() function which takes as input the number of PointSTs that the trajectory is consisted of and gives as output an integer which is an identifier of the current allocation. Then we have to reset the index of the current allocation and set it at the beginning of the TrajCache allocation by using the TrajCache_ResetIndex() funcion which takes as input the identifier of the TrajCache allocation(which is the output of the TrajCache_Allocate() function).

	postgres=# SELECT TrajCache_ResetIndex(0);
 	trajcache_resetindex 
	----------------------
 	
	(1 row)
	
Subsequently, by employing the TrajCache_Append() function we append the PointSTs to the trajectory one by one. This function takes as input the identifier of the current allocation and the PointST to be added.

	postgres=# SELECT TrajCache_Append(0, PointST('2008-12-31 19:29:30' :: Timestamp, PointSP(1,1)));
	 trajcache_append 
	------------------
 	
	(1 row)
	
	postgres=# SELECT TrajCache_Append(0, PointST('2008-12-31 19:29:34' :: Timestamp, PointSP(1,2)));
 	trajcache_append 
	------------------
 	
	(1 row)
	
Finally with the trajcache2trajectory() function we materialize the trajectory. This function takes as input the identifier of the current allocation and returns the actual trajectory.
	
	postgres=# SELECT trajcache2trajectory(0);
                             trajcache2trajectory                              
	-------------------------------------------------------------------------------
 	'2008-12-31 19:29:30' 1 1,'2008-12-31 19:29:32' 1 3,'2008-12-31 19:29:34' 1 2
	(1 row)

Alternatively someone can use the constructor with an array of PointST. An example is shown below:
	
	postgres=# SELECT Trajectory(ARRAY[PointST('2008-12-31 19:29:31' :: Timestamp, PointSP(2,2)),PointST('2008-12-31 19:30:30' :: Timestamp, PointSP(3,3))]);
                     trajectory                      
	-----------------------------------------------------
 	'2008-12-31 19:29:31' 2 2,'2008-12-31 19:30:30' 3 3
	(1 row)
	
For inserting the trajectory in a database an INSERT query must be used. An example is shown below:

	INSERT INTO imis_traj(obj_id, traj_id, traj) VALUES (210671000, 1000, (SELECT trajcache2trajectory(0)))

	
@see @ref Trajectory/Accessors.sql
@see @ref Trajectory/Casts.sql
@see @ref Trajectory/Constructors.sql
@see @ref Trajectory/Definition.sql
@see @ref Trajectory/Editors.sql
@see @ref Trajectory/Interpolation.sql
@see @ref Trajectory/Properties.sql
@see @ref Trajectory/RelationalOperators.sql
@see @ref Trajectory/SessionParameters.sql
@see @ref Trajectory/Similarity.sql
@see @ref Trajectory/Simplification.sql

# Coordinate Transformation {#data_type_transformation}

As already mentioned Hermes works on the Euclidean space, meaning it needs degrees (lon, lat) to be transformed into meters (x, y). If you have installed the PostGIS you could use the [ST_Transform](http://postgis.org/docs/ST_Transform.html) function to do the transformations.

	postgres=# SELECT  ST_AsText( ST_Transform(ST_GeomFromText('POINT(23.65298 37.94176)', 4326), 2100));
                st_astext                 
	------------------------------------------
 	POINT(469358.735448916 4199122.03221326)
	(1 row)

Alternatively the transformation of Geographic to/from Topocentric conversion (EPSG 9837) has been implemented. According to this specification, to do the transformation we only need a reference point (lon, lat) which in (x, y) will be regarded as (0, 0), i.e. the Cartesian center. So, the closer a position is to this reference point the more accurate the transformation will be. Thus, a dataset must have a reference point for transformations @cite vodas2013hermes. This can be achieved by using the `PointXY(point PointLL, LRP PointLL)` and  `PointLL(point PointXY, LRP PointLL)` which are implemented by @ref ll2xy and @ref xy2ll functions.

	postgres=# SELECT PointLL(PointXY(-240909.991094767,-323271.482666732), PointLL(23.63994,37.9453));
       pointll       
	---------------------
 	20.999999 35.000044
	(1 row)
	
	postgres=# SELECT PointXY(PointLL(20.999999 , 35.000044), PointLL(23.63994,37.9453));
            pointxy            
	-------------------------------
 	-240909.991095 -323271.482667
	(1 row)