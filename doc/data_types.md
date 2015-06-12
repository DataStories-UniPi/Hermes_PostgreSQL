
# Data types {#data_types}

In this section, the data types supported by Hermes are presented in the following table.

Category        | Data types
:-----------:   | :-----------:
Temporal        | [Timestamp](http://www.postgresql.org/docs/9.4/static/datatype-datetime.html) (Datetime t) 
Temporal        | PeriodT (Timestamp initial, Timestamp ending) 
Temporal        | RangeT (Interval radius, Timestamp center)
Saptial         | PointSP (number x, number y) 
Saptial         | BoxSP (PointSP lower-left, PointSP upper-right)
Saptial         | SegmentSP (PointSP initial, PointSP ending)
Saptial         | RangeSP (number radius, PointSP center)
Spatio-temporal | PointST (Timestamp t, PointSP p)
Spatio-temporal | BoxST (PeriodT period, BoxSP box)
Spatio-temporal | SegmentST (PeriodT period, SegmentSP segment)
Spatio-temporal | RangeST (RangeT t-range, RangeSP sp-range)

According to @cite vodas2013hermes temporal data types are those types that model only the temporal dimension of Mobility. The main type for temporal data type is introduced by PostrgreSQL and Hermes encapsulates it in its data model. An example of a timestamp is ‘2012-09-20 08:05:46’. Another encapsulated data type is the Interval which is used to store information like ‘1 second’ or ‘05:30:5’ (which means 5 hours 30 minutes and 5 seconds) hence
contains a temporal quantity. One potential limitation of interval is that on
cases where it was produced from a subtraction of two timestamps it won’t
keep the original timestamps from which it was computed. That limitation
led to the development of some custom temporal types in Hermes @cite vodas2013hermes.

![Hermes Data Types](data_types.png)
@image latex data_types.png "Hermes Data Types" 

PostgreSQL can cast a string to any datatype. In the 1 st query a timestamp is given as a string and is cast using “::” to the timestamp datatype. This is also done for another datatype (i.e. interval) in the 2 nd query @cite vodas2013hermes.
