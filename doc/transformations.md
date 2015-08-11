# Transformations {#transformations}

Hermes provides a way to perform a series of transformations upon a dataset
stored in the engine.

With the functions provided, the user can easily get new datasets, based on the
initial one. The user has the options of save the generated datasets to the
engine, export them to a csv file, or both.

Each one of the six different transformation functions is further described
below, followed by a hands on example.

## Decreased Sampling Rate Transformation {#decreased_sr}
**trajectory\_transformation\_dec\_sr (dataset, rate\[, save \[, new\_dataset\_name\[, csv_file\]\]\])**

###Returns###
The integer 1 to indicate that everything went right.
###Parameters Description###
- **dataset** Text, the name with which the dataset is stored in Hermes.
- **rate** Float between 0 and 1, number of points to be deleted
(rate \* |points|).
- **save** \(Optional\) Boolean, indicates whether the new dataset will be
saved in Hermes or not. If left blank, the new dataset will be saved.
- **new\_dataset\_name** \(Optional\) Text, If save is True, the name of the
new dataset inside Hermes. If left blank, the name of the new_dataset will be
*transformed\_dec\_sr*
- **csv\_file** \(Optional\) Boolean, Indicates whether the new dataset will be
exported to a csv file, with the default format of Hermes. If left blank, the
dataset will be exported to a csv file.

###Example###
For example purposes, suppose we have a dataset, saved in Hermes, containing one
trajectory.
The name of the dataset is gmaps. The segments of the dataset are shown below.
![Segments of gmaps dataset](gmaps.png)
@image latex gmaps.png "Segments of gmaps dataset" width=\textwidth

It is clear that the trajectory consists of 7 ST Points. If we wanted to perform
a *decreased sampling rate* transformation by keeping 60% of the initial
points, we could run the following querie:

      SELECT trajectory_transformation_dec_sr('gmaps', 0.4, True, 'new_d', True);

As a result, we get a new dataset named new\_d\_dec\_sr, which consists of random
60% of the initial points. If we run the following querie we get the new
trajectory segments:

      SELECT * FROM new_d_dec_sr_seg;

![Segments of decreased sr gmaps dataset](gmaps_dec.png)
@image latex gmaps_dec.png "Segments of decreased sr gmaps dataset" width=\textwidth

Additionally, due to csv_file parameter being True, we get a cvs file
in the data folder. The name of the file would be *gmaps\_transformed\_decsr\_0.6.txt*. Generally, the name of the csv file will
be *dataset + \_transformed + \_kind of transformation + \_parameters*

If we load the initial and the transformed datasets in Google Maps \(from the
csv files\), we get the following result:

![Initial (blue) and transformed (red) dataset](dec_04.png)
@image latex dec_04.png "Initial \(blue\) and transformed \(red\) dataset" width=\textwidth

## Increased Sampling Rate Transformation {#increased_sr}
**trajectory\_transformation\_inc\_sr (dataset, rate\[, save \[, new\_dataset\_name\[, csv_file\]\]\])**

###Returns###
The integer 1 to indicate that everything went right.
###Parameters Description###
- **dataset** Text, the name with which the dataset is stored in Hermes.
- **rate** Float between 0 and 1, number of points to be added
(rate \* |points|).
- **save** \(Optional\) Boolean, indicates whether the new dataset will be
saved in Hermes or not. If left blank, the new dataset will be saved.
- **new\_dataset\_name** \(Optional\) Text, If save is True, the name of the
new dataset inside Hermes. If left blank, the name of the new_dataset will be
*transformed\_inc\_sr*
- **csv\_file** \(Optional\) Boolean, Indicates whether the new dataset will be
exported to a csv file, with the default format of Hermes. If left blank, the
dataset will be exported to a csv file.

###Example###
For example purposes, suppose we have a dataset, saved in Hermes, containing one
trajectory.
The name of the dataset is gmaps. The segments of the dataset are shown below.
![Segments of gmaps dataset](gmaps.png)
@image latex gmaps.png "Segments of gmaps dataset" width=\textwidth

It is clear that the trajectory consists of 7 ST Points. If we wanted to perform
an *increased sampling rate* transformation by adding 60% more
points, we could run the following querie:

      SELECT trajectory_transformation_inc_sr('gmaps', 0.6, True, 'new_i', True);

Because we perform increased sampling rate transformation all the new points
will be on the initial trajectory. In order to achive this, *atInstant* function
is used internally.
As a result, we get a new dataset named new\_i\_dec\_sr, which consists of random
60% more points. If we run the following querie we get the new
trajectory segments:

      SELECT * FROM new_i_inc_sr_seg;

![Segments of increased sr gmaps dataset](gmaps_inc.png)
@image latex gmaps_inc.png "Segments of increased sr gmaps dataset" width=\textwidth

Additionally, due to csv_file parameter being True, we get a cvs file
in the data folder. The name of the file would be *gmaps\_transformed\_incsr\_0.6.txt*. Generally, the name of the csv file will
be *dataset + \_transformed + \_kind of transformation + \_parameters*

If we load the initial and the transformed datasets in Google Maps \(from the
csv files\), we get the following result:

![Initial \(blue\) and transformed dataset](inc_04.png)
@image latex inc_04.png "Initial \(blue\) and transformed dataset" width=\textwidth

## Time Interval With Step Transformation {#time_sr}
**trajectory\_transformation\_inc\_sr (dataset, start\_date, end\_date, step\[, save \[, new\_dataset\_name\[, csv_file\]\]\])**

###Returns###
The integer 1 to indicate that everything went right.
###Parameters Description###
- **dataset** Text, the name with which the dataset is stored in Hermes.
- **start\_date** Text, a timestamp with the following format:
*yyyy-mm-dd hh:mm:ss*. Indicates the start of the time interval
- **end\_date** Text, a timestamp with the following format:
*yyyy-mm-dd hh:mm:ss*. Indicates the end of the time interval
- **step** Integer, indicates the step inside time interval, measured in
seconds.
- **save** \(Optional\) Boolean, indicates whether the new dataset will be
saved in Hermes or not. If left blank, the new dataset will be saved.
- **new\_dataset\_name** \(Optional\) Text, If save is True, the name of the
new dataset inside Hermes. If left blank, the name of the new_dataset will be
*transformed\_time\_sr*
- **csv\_file** \(Optional\) Boolean, Indicates whether the new dataset will be
exported to a csv file, with the default format of Hermes. If left blank, the
dataset will be exported to a csv file.

###Example###
For example purposes, suppose we have a dataset, saved in Hermes, containing one
trajectory.
The name of the dataset is gmaps. The segments of the dataset are shown below.
![Segments of gmaps dataset](gmaps.png)
@image latex gmaps.png "Segments of gmaps dataset" width=\textwidth

It is clear that the trajectory consists of 7 ST Points. If we wanted to keep
just the points between 2008-12-31 19:30:30 and 2008-12-31 19:50:30, every
10 minutes we could run the following querie:

      SELECT trajectory_transformation_time_sr('gmaps', '2008-12-31 19:30:30', '2008-12-31 19:50:30', 600, True, 'new_t', True);

As a result, we get a new dataset named new\_t\_time\_sr, which consists of
all the points (if there are any) between the indicated interval in combination
with the given step. If we run the following querie we get the new
 trajectory segments:

      SELECT * FROM new_t_time_sr_seg;

![Segments of time sr gmaps dataset](gmaps_time.png)
@image latex gmaps_time.png "Segments of time sr gmaps dataset" width=\textwidth

Additionally, due to csv_file parameter being True, we get a cvs file
in the data folder. The name of the file would be *gmaps\_transformed\_\_timesr\_\_2008-12-31 19\_30\_30\_\_2008-12-31 19\_50\_30\_\_600*. Generally, the name of the csv file will
be *dataset + \_transformed + \_kind of transformation + \_parameters*

If we load the initial and the transformed datasets in Google Maps \(from the
csv files\), we get the following result:

![Initial (blue) and transformed (green) dataset](time.png)
@image latex time.png "Initial \(blue\) and transformed \(green\) dataset" width=\textwidth

## Adding Noise Points Transformation {#add_noise}
**trajectory\_transformation\_add\_noise (dataset, rate, distance\[, save \[, new\_dataset\_name\[, csv_file\]\]\])**

###Returns###
The integer 1 to indicate that everything went right.
###Parameters Description###
- **dataset** Text, the name with which the dataset is stored in Hermes.
- **rate** Float between 0 and 1, number of points to be added
(rate \* |points|).
- **distance** Integer, outlier - trajectory maximum distance, in meters.
- **save** \(Optional\) Boolean, indicates whether the new dataset will be
saved in Hermes or not. If left blank, the new dataset will be saved.
- **new\_dataset\_name** \(Optional\) Text, If save is True, the name of the
new dataset inside Hermes. If left blank, the name of the new_dataset will be
*transformed\_add\_noise*
- **csv\_file** \(Optional\) Boolean, Indicates whether the new dataset will be
exported to a csv file, with the default format of Hermes. If left blank, the
dataset will be exported to a csv file.

###Example###
For example purposes, suppose we have a dataset, saved in Hermes, containing one
trajectory.
The name of the dataset is gmaps. The segments of the dataset are shown below.
![Segments of gmaps dataset](gmaps.png)
@image latex gmaps.png "Segments of gmaps dataset" width=\textwidth

It is clear that the trajectory consists of 7 ST Points. If we wanted to add
40% more noise points, each one of them would be 2 km, at most, apart from the
trajectory, we could run the following querie:

      SELECT trajectory_transformation_add_noise('gmaps', 0.4, 2000, True, 'new_n', True);

As a result, we get a new dataset named new\_d\_add\_noise, which consists of
the initial points, plus 40% (3) more points, as described above. If we run the
following querie we get the new trajectory segments:

      SELECT * FROM new_n_add_noise_seg;

![Segments of noise gmaps dataset](gmaps_noise.png)
@image latex gmaps_noise.png "Segments of noise gmaps dataset" width=\textwidth

Additionally, due to csv_file parameter being True, we get a cvs file
in the data folder. The name of the file would be *gmaps\_transformed\_addnoise\_0.4\_2000.txt*. Generally, the name of the csv file will
be *dataset + \_transformed + \_kind of transformation + \_parameters*

If we load the initial and the transformed datasets in Google Maps \(from the
csv files\), we get the following result:

![Initial (blue) and transformed (purple) dataset](noise_04.png)
@image latex noise_04.png "Initial \(blue\) and transformed \(purple\) dataset" width=\textwidth

## Trajectory Synced Shift Transformation {#synced_shift}
**trajectory\_transformation\_synced\_shift (dataset, rate, distance\[, save \[, new\_dataset\_name\[, csv_file\]\]\])**
###Returns###
The integer 1 to indicate that everything went right.
###Parameters Description###
- **dataset** Text, the name with which the dataset is stored in Hermes.
- **rate** Float between 0 and 1, number of points to be shifted
(rate \* |points|).
- **distance** Integer, shifted point - trajectory point maximum distance, in
meters.
- **save** \(Optional\) Boolean, indicates whether the new dataset will be
saved in Hermes or not. If left blank, the new dataset will be saved.
- **new\_dataset\_name** \(Optional\) Text, If save is True, the name of the
new dataset inside Hermes. If left blank, the name of the new_dataset will be
*transformed\_synced\_shift*
- **csv\_file** \(Optional\) Boolean, Indicates whether the new dataset will be
exported to a csv file, with the default format of Hermes. If left blank, the
dataset will be exported to a csv file.

###Example###
For example purposes, suppose we have a dataset, saved in Hermes, containing one
trajectory.
The name of the dataset is gmaps. The segments of the dataset are shown below.
![Segments of gmaps dataset](gmaps.png)
@image latex gmaps.png "Segments of gmaps dataset" width=\textwidth

It is clear that the trajectory consists of 7 ST Points. If we wanted to shift
90% of the trajectory points, by the same direction and distance \(distance at
  most 1500 meters\) we could run the following querie:

      SELECT trajectory_transformation_synced_shift('gmaps', 0.9, 1500, True, 'new_s', True);

As a result, we get a new dataset named new\_s\_synced\_shift, which consists of
the initial points, 90% of them shifted by the same direction and offset. By
running the following querie we can see the new trajectory segments.

      SELECT * FROM new_s_synced_shift_seg;

![Segments of synced shift gmaps dataset](gmaps_sync.png)
@image latex gmaps_sync.png "Segments of synced shift gmaps dataset" width=\textwidth

Additionally, due to csv_file parameter being True, we get a cvs file
in the data folder. The name of the file would be *gmaps\_transformed\_syncedshift\_0.9\_1500.txt*. Generally, the name of the csv file will
be *dataset + \_transformed + \_kind of transformation + \_parameters*

If we load the initial and the transformed datasets in Google Maps \(from the
csv files\), we get the following result:

![Initial (blue) and transformed (green) dataset](synced.png)
@image latex synced.png "Initial \(blue\) and transformed \(green\) dataset" width=\textwidth

## Trajectory Random Shift Transformation {#random_shift}
**trajectory\_transformation\_random\_shift (dataset, rate, distance\[, save \[, new\_dataset\_name\[, csv_file\]\]\])**
###Returns###
The integer 1 to indicate that everything went right.
###Parameters Description###
- **dataset** Text, the name with which the dataset is stored in Hermes.
- **rate** Float between 0 and 1, number of points to be shifted
(rate \* |points|).
- **distance** Integer, shifted point - trajectory point maximum distance, in
meters.
- **save** \(Optional\) Boolean, indicates whether the new dataset will be
saved in Hermes or not. If left blank, the new dataset will be saved.
- **new\_dataset\_name** \(Optional\) Text, If save is True, the name of the
new dataset inside Hermes. If left blank, the name of the new_dataset will be
*transformed\_random\_shift*
- **csv\_file** \(Optional\) Boolean, Indicates whether the new dataset will be
exported to a csv file, with the default format of Hermes. If left blank, the
dataset will be exported to a csv file.

###Example###
For example purposes, suppose we have a dataset, saved in Hermes, containing one
trajectory.
The name of the dataset is gmaps. The segments of the dataset are shown below.
![Segments of gmaps dataset](gmaps.png)
@image latex gmaps.png "Segments of gmaps dataset" width=\textwidth

It is clear that the trajectory consists of 7 ST Points. If we wanted to shift
60% of the trajectory points, by random direction and distance \(distance at
  most 1500 meters\) we could run the following querie:

      SELECT trajectory_transformation_random_shift('gmaps', 0.6, 1500, True, 'new_r', True);

As a result, we get a new dataset named new\_r\_random\_shift, which consists of
the initial points, 60% of them shifted by random direction and offset each. By
running the following querie we can see the new trajectory segments.

      SELECT * FROM new_r_random_shift_seg;

![Segments of random shift gmaps dataset](gmaps_random.png)
@image latex gmaps_random.png "Segments of random shift gmaps dataset" width=\textwidth

Additionally, due to csv_file parameter being True, we get a cvs file
in the data folder. The name of the file would be *gmaps\_transformed\_randomshift\_0.6\_1500.txt*. Generally, the name of the csv file will
be *dataset + \_transformed + \_kind of transformation + \_parameters*

If we load the initial and the transformed datasets in Google Maps \(from the
csv files\), we get the following result:

![Initial (blue) and transformed (pink) dataset](random.png)
@image latex random.png "Initial \(blue\) and transformed \(pink\) dataset" width=\textwidth
