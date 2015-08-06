# Transformations {#transformations}

Hermes provides a way to perform a series of transformations upon a dataset
stored in the engine.

With the functions provided, the user can easily get new datasets, based on the
initial one. The user has the options of save the generated datasets to the
engine, export them to a csv file, or both.

Each one of the six different transformation functions is further described
below.

## Decreased Sampling Rate Transformation {#decreased_sr}
**trajectory\_transformation\_dec\_sr (dataset, rate, \[save\], \[new\_dataset\_name\], \[csv_file\])**

## Increased Sampling Rate Transformation {#increased_sr}
**trajectory\_transformation\_inc\_sr (dataset, rate, \[save\], \[new\_dataset\_name\], \[csv_file\])**

## Time Interval With Step Transformation {#time_sr}
**trajectory\_transformation\_time\_sr (dataset, start\_date, end\_date, step, \[save\], \[new\_dataset\_name\], \[csv_file\])**

## Adding Noise Points Transformation {#add_noise}
**trajectory\_transformation\_add\_noise (dataset, rate, distance, \[save\], \[new\_dataset\_name\], \[csv_file\])**

## Trajectory Synced Shift Transformation {#synced_shift}
**trajectory\_transformation\_synced\_shift (dataset, rate, distance, \[save\], \[new\_dataset\_name\], \[csv_file\])**

## Trajectory Random Shift Transformation {#random_shift}
**trajectory\_transformation\_random\_shift (dataset, rate, distance, \[save\], \[new\_dataset\_name\], \[csv_file\])**
