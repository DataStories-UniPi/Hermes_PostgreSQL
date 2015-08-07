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
![Segments of gmaps dataset](images\gmaps.png)
@image latex gmaps.png "Segments of gmaps dataset" width=\textwidth

It is clear that the trajectory consists of 7 ST Points. If we wanted to perform
a *decreased sampling rate* transformation by keeping 60% of the initial
points, we could run the following querie:

      SELECT trajectory_transformation_dec_sr('gmaps', 0.4, True, 'new_d', True);

As a result, we get a new dataset named new\_d\_dec\_sr, which consists of random
60% of the initial points. If we run the following querie we get the new
trajectory segments:

      SELECT * FROM new_d_dec_sr_seg;

![Segments of decreased sr gmaps dataset](images\gmaps_dec.png)
@image latex gmaps_dec.png "Segments of decreased sr gmaps dataset" width=\textwidth

Additionally, due to csv_file parameter being True, we get a cvs file
in the data folder. The name of the file would be *gmaps\_transformed\_decsr\_0.6.txt*. Generally, the name of the csv file will
be *dataset + \_transformed + \_kind of transformation + \_parameters*

If we load the initial and the transformed datasets in Google Maps \(from the
csv files\), we get the following result:

![Initial \(blue\) and transformed \(red\) dataset](images\dec_04.png)
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
![Segments of gmaps dataset](images\gmaps.png)
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

![Segments of increased sr gmaps dataset](images\gmaps_inc.png)
@image latex gmaps_inc.png "Segments of increased sr gmaps dataset" width=\textwidth

Additionally, due to csv_file parameter being True, we get a cvs file
in the data folder. The name of the file would be *gmaps\_transformed\_incsr\_0.6.txt*. Generally, the name of the csv file will
be *dataset + \_transformed + \_kind of transformation + \_parameters*

If we load the initial and the transformed datasets in Google Maps \(from the
csv files\), we get the following result:

![Initial \(blue\) and transformed dataset](images\inc_04.png)
@image latex inc_04.png "Initial \(blue\) and transformed dataset" width=\textwidth

## Time Interval With Step Transformation {#time_sr}
**trajectory\_transformation\_time\_sr (dataset, start\_date, end\_date, step, \[save\], \[new\_dataset\_name\], \[csv_file\])**

## Adding Noise Points Transformation {#add_noise}
**trajectory\_transformation\_add\_noise (dataset, rate, distance, \[save\], \[new\_dataset\_name\], \[csv_file\])**

## Trajectory Synced Shift Transformation {#synced_shift}
**trajectory\_transformation\_synced\_shift (dataset, rate, distance, \[save\], \[new\_dataset\_name\], \[csv_file\])**

## Trajectory Random Shift Transformation {#random_shift}
**trajectory\_transformation\_random\_shift (dataset, rate, distance, \[save\], \[new\_dataset\_name\], \[csv_file\])**
