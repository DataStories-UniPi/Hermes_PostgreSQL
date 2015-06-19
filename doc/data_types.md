
# Data types {#data_types}

In this section, the data types supported by Hermes are presented in the following table.

Category        | Data types
:-----------:   | :-----------:
Temporal        | [Timestamp](http://www.postgresql.org/docs/9.4/static/datatype-datetime.html) (Datetime t) 
Temporal        | [Interval](http://www.postgresql.org/docs/9.4/static/datatype-datetime.html)  
Temporal        | Period (Timestamp initial, Timestamp ending) 
Temporal        | RangeT (Interval radius, Timestamp center)
Saptial         | PointSP (number x, number y) 
Saptial         | BoxSP (PointSP lower-left, PointSP upper-right)
Saptial         | SegmentSP (PointSP initial, PointSP ending)
Saptial         | RangeSP (number radius, PointSP center)
Saptial         | PointLL (float8 lon, float8 lat)
Saptial         | PointXY (float8 x, float8 y)
Spatio-temporal | PointST (Timestamp t, PointSP p)
Spatio-temporal | BoxST (PeriodT period, BoxSP box)
Spatio-temporal | SegmentST (PeriodT period, SegmentSP segment)
Spatio-temporal | RangeST (RangeT t-range, RangeSP sp-range)
Spatio-temporal | Trajectory (to be entered)

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
	

A timestamp is given as a string and is cast using “::” to the timestamp data type. This is also possible for the interval data type. Finally Hermes introduced the @ref Period temporal data type.

	postgres=# SELECT Period('2008-12-31 23:00:00', '2009-01-01 01:00:00');
                   period                    
	---------------------------------------------
 	'2008-12-31 23:00:00' '2009-01-01 01:00:00'
	(1 row)
	
	
# Spatial data types {#data_types_spatial}

According to @cite vodas2013hermes spatial data types are those types that model only the spatial dimension of Mobility. Hermes is designed to work with data in the Eyclidean space, which means that a position defined in Riemannian geometry (or any other geometry) can`t be used. Usually the Cartesian system is used and the coordinates are measured in meters. **To be precise due to the fact that intergers are used to represent the main spatial data type, the unit length that must be used is a factor smaller than the precision we want to achieve.**

The main spatial data types are the @ref PointSP and the @ref SegmentSP

![Spatial segment](spatial_segment.png)
@image latex spatial_segment.png "Spatial segment" 

The previous data types do not have a surface. In contrast, the @ref BoxSP data type has a surface

![Spatial box](spatial_box.png)
@image latex spatial_box.png "Spatial box" 

Some examples of using the saptial data types are shown below:

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
	
	postgres=# SELECT BoxSP(PointSP(2337709, 4163887),PointSP(3228259, 4721671)) ;
              boxsp              
	---------------------------------
 	2337709 4163887 3228259 4721671
	(1 row)
	
# Spatio-temporal data types {#data_types_spatio_temp}

According to @cite vodas2013hermes spatio-temporal data types are those types that model both the temporal and spatial dimension of Mobility in a unified manner. The main spatio-temporal data types are @ref PointST, @ref SegmentST and @ref BoxST

# Trajectory type {#data_type_trajectory}

Trajectory data type in an object containing a sequence of spatio-temporal points ordered in time. In contrast with the previous data types, it has a variable length and is compromised of s sequence of PointST objects ordered by time @cite vodas2013hermes. The different approach to this data type is that we look at the movement of an object as a whole and not as segments in smaller parts. 

# Coordinate Transformation {#data_type_transformation}

As already mention Hermes work on the Euclidean space, meaning it need meaning it needs degrees (lon, lat) to be transformed into meters (x, y). For this transformation the Geographic
to/from Topocentric conversion (EPSG 9837) was implemented. According to this specification, to do the transformation we only need a reference point (lon, lat) which in (x, y) will be regarded as (0, 0), i.e. the Cartesian center. So, the closer a position is to this reference point the more accurate the transformation will be. Thus, a dataset must have a reference point for transformations @cite vodas2013hermes. This can be achieved by using the `PointXY(point PointLL, LRP PointLL)` and  `PointLL(point PointXY, LRP PointLL)` which are implemented by @ref ll2xy and @ref xy2ll functions.

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