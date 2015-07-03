# Loading datasets {#datasets}

Hermes is possible to host diverse datasets of moving vehicles. For example, it is
possible to host two diverse datasets one of moving vehicles and another one
of traveling vessels in the same database.

# Database Schema {#database_schema}

This section describes the structure of the database that was designed to be
able to host multiple datasets. That metadata infrastructure takes the form of a table named hdataset. The structure of the hdataset table is as follows @cite vodas2013hermes :

- **id** is an auto incremented integer column that is the primary key of the table. Each dataset hosted is given an id.
- **name** is a text column that contains a unique short name of the dataset.
- **name_long** is a text column that contains a human friendly name of the dataset.
- **lrp** is a PointLL column that is the reference point for coordinate transformation.
- **srid** is an integer column that contains the EPSG code of the projected reference system in which the dataset is stored in Hermes. Note that if we give a value for local ref poi then SRID will have to be NULL and vice versa.
- **segment_storage** a boolean value ???
- **trajectory_storage** a boolean value ???
- **subtrajectory_storage** a boolean value indicating a parent-child relationship between the datasets. ???
- **semantics_enabled** a boolean value ???
- **partitioning_t_range** an iterval ??? 
- **partitioning_x_range** an integer ???
- **partitioning_y_range** an integer ???
- **notes** is a text column that contains arbitrary notes on the dataset.
 
Each dataset in this table has a unique identifier and a unique short name e.g. [1, ‘imis’] or [2, ‘milan’]. Most spatio-temporal datasets are in the form of [objectID, trajectoryID, t, lon, lat] where “objectID” is the identifier of the object, “trajectoryID” is the identifier of the trajectory for that object, “t” is the UTC (Coordinated Universal Time) timestamp at which it was recorded and “lon” and “lat” are degrees of longitude and latitude in WGS 84 Geographic Coordinate System. “objectID” and “trajectoryID”, normally, are combined to form the unique identifier of a trajectory in a specific dataset @cite vodas2013hermes

Each dataset consists of three tables. Each table’s name begins with the
dataset’s name followed by a suffix @cite vodas2013hermes :

- _obj: this table hosts the objects that exist in the dataset and contains
one column:
  + **obj_id** the unique identifier of the object.
- _traj: this table hosts the trajectories of the objects and contains three
columns:
  + **obj_id** is a foreign key to obj table.
  + **traj_id** is an identifier for the particular trajectory of the object.
- _seg: this table will host the segments of the trajectories and contains four
columns:
  + **obj_id** is a foreign key to obj table.
  + **traj_id** is a foreign key to traj table.
  + **seg_id** is an identifier for the particular segment of the trajectory.
  + **seg** contains a segment of a trajectory, an object on type SegmentST.
- _seg_lobby: ???
  + **obj_id** contains an integer ??? 
  + **traj_id** contains an integer ???
  + **t** contains a timestamp without time zone ???
  + **x** contains an integer ???
  + **y** contains an integer ???
 
According to @cite vodas2013hermes the objects table and the trajectories table must have data in contrast with segments table. The “traj” column in trajectories table could be empty if
trajectories are stored in the segments table. It is always possible to build a trajectory object from its corresponding segments that we can find in the segments table on the fly using aggregate functions. That allows us to use advanced methods that Hermes provides for its “Trajectory” type.

Finally there is an hparameters table, with extra parameters for ??? and the structure of the table is as follows:

- **key** contains a text ???
- **value** contains a text ???

The statistics for each dataset are stored in the table hdatasets_statistics. The statistics about the dataset that can be kept in this table are:
- bounds of the dataset (tmin, tmax, lx, ly, hx, hy, llon, llat, hlon, hlat).
- centroid of the dataset (centroid x, centroid y, centroid lon, centroid lat).
- number of objects / trajectories / points / segments.
- minimum /average / maximum number of points per trajectory / trajectory duration / trajectory length.

The structure of the table is described below:

- **dataset** is a foreign key to hdatasets table storing the primary key of the dataset.
- **nr_objects** is a bigint storing the total number of objects stored in the table _obj of the dataset. 
- **nr_trajectories** is a bigint storing the total number of Trajectories in the table _traj of the dataset. 
- **nr_segments** is a bigint storing the total number of Segments in the table _seg of the dataset.
- **nr_points** is a bigint storing the total number of Points in the Trajectories. 
- **tmin** is a timestamp showing the minimum timestamp of the dataset
- **tmax** is a timestamp showing the maximum timestamp of the dataset
- **lx** is a integer
- **ly** is a integer
- **hx** is a integer
- **hy** is a integer
- **llon** is a double precision number
- **llat** is a double precision number
- **hlon** is a double precision number
- **hlat** is a double precision number
- **points_centroid_t** is a timestamp showing the centroid of the dataset
- **points_centroid_x** is a integer showing the centroid of the x coordinate the dataset
- **points_centroid_y** is a integer showing the centroid of the y coordinate the dataset
- **points_centroid_lon** is a double precision number
- **points_centroid_lat** is a double precision number
- **trajectories_centroid_t** is a timestamp showing the centroid of the trajectories of the dataset
- **trajectories_centroid_x** is a integer showing the centroid of x coordinates of the trajectories of the dataset
- **trajectories_centroid_y** is a integer showing the centroid of y coordinates of the trajectories of the dataset
- **trajectories_centroid_lon** is a double precision number
- **trajectories_centroid_lat** is a double precision number
- **duration** is an interval showing the duration of the dataset
- **area** is a bigint showing the area of the dataset
- **normalized_sampling_rate** is a double precision number showing the normalized sampling rate
- **min_object_trajectories** is a bigint 
- **max_object_trajectories** is a bigint
- **avg_object_trajectories** is a double precision number
- **stddev_object_trajectories** is a double precision number
- **median_object_trajectories** is a double precision number
- **min_object_normalized_sampling_rate** is a bigint
- **max_object_normalized_sampling_rate** is a bigint
- **avg_object_normalized_sampling_rate** is a double precision number
- **stddev_object_normalized_sampling_rate** is a double precision number
- **median_object_normalized_sampling_rate** is a double precision number
- **min_trajectory_points** is a bigint
- **max_trajectory_points** is a bigint
- **avg_trajectory_points** is a double precision number
- **stddev_trajectory_points** is a double precision number
- **median_trajectory_points** is a double precision number
- **min_trajectory_duration** is an interval
- **max_trajectory_duration** is an interval
- **avg_trajectory_duration** is an interval
- **stddev_trajectory_duration** is an interval
- **median_trajectory_duration** is an interval
- **min_trajectory_length** is a double precision number
- **max_trajectory_length** is a double precision number
- **avg_trajectory_length** is a double precision number
- **stddev_trajectory_length** is a double precision number
- **median_trajectory_length** is a double precision number
- **min_trajectory_displacement** is a double precision number
- **max_trajectory_displacement** is a double precision number
- **avg_trajectory_displacement** is a double precision number
- **stddev_trajectory_displacement** is a double precision number
- **median_trajectory_displacement** is a double precision number
- **min_trajectory_area** is a bigint
- **max_trajectory_area** is a bigint
- **avg_trajectory_area** is a double precision number
- **sttdev_trajectory_area** is a double precision number
- **median_trajectory_area** is a double precision number
- **min_trajectory_gyradius** is a double precision number
- **max_trajectory_gyradius** is a double precision number
- **avg_trajectory_gyradius** is a double precision number
- **stddev_trajectory_gyradius** is a double precision number
- **median_trajectory_gyradius** is a double precision number
- **min_trajectory_angle_xx_avg** is a double precision number
- **max_trajectory_angle_xx_avg** is a double precision number
- **avg_trajectory_angle_xx_avg** is a double precision number
- **stddev_trajectory_angle_xx_avg** is a double precision number
- **median_trajectory_angle_xx_avg** is a double precision number
- **min_trajectory_average_speed** is a double precision number
- **max_trajectory_average_speed** is a double precision number
- **avg_trajectory_average_speed** is a double precision number
- **stddev_trajectory_average_speed** is a double precision number
- **median_trajectory_average_speed** is a double precision number
- **min_trajectory_normalized_sampling_rate** is a double precision number
- **max_trajectory_normalized_sampling_rate** is a double precision number
- **avg_trajectory_normalized_sampling_rate** is a double precision number
- **stddev_trajectory_normalized_sampling_rate** is a double precision number
- **median_trajectory_normalized_sampling_rate** is a double precision number
- **min_segment_duration** is an interval
- **max_segment_duration** is an interval
- **avg_segment_duration** is an interval
- **stddev_segment_duration** is an interval
- **median_segment_duration** is an interval
- **min_segment_length** is a double precision number
- **max_segment_length** is a double precision number
- **avg_segment_length** is a double precision number
- **stddev_segment_length** is a double precision number
- **median_segment_length** is a double precision number
- **min_segment_area** is a double precision number
- **max_segment_area** is a double precision number
- **avg_segment_area** is a double precision number
- **stddev_segment_area** is a double precision number
- **median_segment_area** is a double precision number
- **min_segment_angle_xx** is a double precision number
- **max_segment_angle_xx** is a double precision number
- **avg_segment_angle_xx** is a double precision number
- **stddev_segment_angle_xx** is a double precision number
- **median_segment_angle_xx** is a double precision number
- **min_segment_average_speed** is a double precision number
- **max_segment_average_speed** is a double precision number
- **avg_segment_average_speed** is a double precision number
- **stddev_segment_average_speed** is a double precision number
- **median_segment_average_speed** is a double precision number

In the figure below @cite vodas2013hermes is shown an older schema of the database:

![Database schema](database_schema.png)
@image latex database_schema.png "Database schema" 

# AIS Dataset {#dataset_ais}

In these pages you will find some examples of the Hermes MOD. These examples use the [IMIS3days Dataset](http://chorochronos.datastories.org/?q=node/8), which has been kindly provided by the IMIS HELLAS. According to \cite pelekis2014mobility the dataset covers almost 3 days of informations about ships sailing at Greek Seas. The “IMIS 3 Days” dataset spawns from “2008-12-31 19:29:30” to “2009-01-02 17:10:06” and contains positions reports for 933 ships. It is spatially constrained in the Aegean Sea and covers an area of 496736 km 2 , from (21,35)-lowest to (29,39)-highest longitude-latitude point @cite vodas2013hermes .

![Overview of the IMIS 3 Days dataset](imis_overview.png)
@image latex imis_overview.png "Overview of the IMIS 3 Days dataset" 

Loading the datatset into Hermes MOD is simple, due to a module called "Hermes-Loader", which automates the creation and feeding of the respective database table as well as the transformation of coordinates from degrees (lon/lat to meters (x/y) @cite pelekis2014mobility.

Before loading the AIS data in PostgreSql, one needs to save the data files in the PostgreSQL directory in a file type of csv with a header for column names with the following structure: <obj_id, traj_id, t, lon, lat>. You can find the directory by executing the below command in the database:

	postgres=# SHOW data_directory;
        data_directory        
	------------------------------
 	/var/lib/postgresql/9.4/main
	(1 row)

Afterwards copying the imis3days.txt file in the database directory the below commands must be executed:

	SELECT HLoader('imis', 'IMIS 3 Days');
	SELECT HLoaderCSV_II('imis', 'imis3days.txt');
	SELECT HDatasetsOfflineStatistics('imis');
	CREATE INDEX ON imis_seg USING gist (seg) WITH (FILLFACTOR = 100);
	
	
In the above script the first and second lines create the tables that will host the dataset and fill these tables, respectively, with the data that are found in file “imis3days.txt”, according to the formatting discussed above. The function HLoader loads the dataset by taking into account the information / parameters that we pass to the function but also the ones that are present in the table. Because loader can be extended to support more formats beyond CSV this is why “HLoader” table exists to hold the specific parameters for that extension. Every loader though must have the parameters that are passed in the function since they are common to any dataset and loader combination @cite vodas2013hermes.

	postgres=# SELECT HLoader('imis', 'imis3days');
 	hloader 
	---------
 
	(1 row)
	
	postgres=# \dt
                List of relations
 	Schema |         Name         | Type  |  Owner   
	--------+----------------------+-------+----------
 	public | hdatasets            | table | postgres
 	public | hdatasets_statistics | table | postgres
 	public | hparameters          | table | postgres
 	public | imis_obj             | table | postgres
 	public | imis_seg             | table | postgres
 	public | imis_seg_lobby       | table | postgres
 	public | imis_traj            | table | postgres
	(7 rows)
	
In the execution of the second line there might be some warning but there is no reason for worrying (ask marios for more informations)

	postgres=# SELECT HLoaderCSV_II('imis', 'imis3days.txt');
	NOTICE:  table "imis_input_data" does not exist, skipping
	CONTEXT:  SQL statement "DROP TABLE IF EXISTS imis_input_data CASCADE;"
	PL/pgSQL function hdatasetsload(text,text,boolean) line 57 at EXECUTE statement
	SQL statement "SELECT HDatasetsLoad(dataset_name, csv_file)"
	PL/pgSQL function hloadercsv_ii(text,text) line 3 at PERFORM
 	hloadercsv_ii 
	---------------
 
	(1 row)
	

Line 3 calculates some statistics for the dataset, such as average number of points per trajectory, average duration of trajectories, and average length of trajectories (see also @ref database_statistics). 

	postgres=# SELECT HDatasetsOfflineStatistics('imis');
 	hdatasetsofflinestatistics 
	----------------------------
 
	(1 row)
	
  
After the calculation of the statistics we can see the statistics with simple queries such as:
  
	SELECT * 
	FROM hdatasets_statistics
	WHERE dataset = HDatasetID('imis');	
	
Line 4 creates an index of type 3D R-tree on the dataset.

	postgres=# CREATE INDEX ON imis_seg USING gist (seg) WITH (FILLFACTOR = 100);
	CREATE INDEX
	
By default, the dataset is hosted in "imis_seg" table, according to a segment-oriente storage model. The list of attributes of “imis_seg” is as follows: <obj_id, traj_id, seg_id, seg>, where obj_id corresponds to object’s identifier (in our case, the MMSI of the ship), traj_id corresponds to a unique identifier of object’s trajectory, seg_id corresponds to a unique identifier object’s trajectory segment, and seg is the geometry of the trajectory segment, of type SegmentST @cite pelekis2014mobility.

	
# Deleting the dataset {#dataset_deleting}

If someone wants to delete the dataset, he should execute the below command:

	SELECT HDatasetsDrop('imis');
	
@see Catalog.sql
@see Compatibility.sql
@see Loader.sql
@see Statistics.sql