/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief The file implements the statistics function for the datasets
 *
 * @see @ref dataset_ais
 *
 */


/** @brief This function create the necessary tables for the Hermes MOD 
 * 	Statistics
 *
 */
CREATE TABLE HDatasets_Statistics (
	dataset integer NOT NULL,

	--Counts
	nr_objects bigint,
	nr_trajectories bigint,
	nr_segments bigint,
	nr_points bigint,

	--Bounds
	tmin timestamp,
	tmax timestamp,
	lx integer,
	ly integer,
	hx integer,
	hy integer,
		--WGS 84
	llon double precision,
	llat double precision,
	hlon double precision,
	hlat double precision,

	--Centroids
	points_centroid_t timestamp,
	points_centroid_x integer,
	points_centroid_y integer,
	points_centroid_lon double precision,
	points_centroid_lat double precision,

	trajectories_centroid_t timestamp,
	trajectories_centroid_x integer,
	trajectories_centroid_y integer,
	trajectories_centroid_lon double precision,
	trajectories_centroid_lat double precision,
	
	--Per-Dataset Statistics
	duration interval,
	area bigint,
	normalized_sampling_rate double precision,

	--Per-Object Statistics
	min_object_trajectories bigint,
	max_object_trajectories bigint,
	avg_object_trajectories double precision,
	stddev_object_trajectories double precision,
	median_object_trajectories double precision,

	min_object_normalized_sampling_rate double precision,
	max_object_normalized_sampling_rate double precision,
	avg_object_normalized_sampling_rate double precision,
	stddev_object_normalized_sampling_rate double precision,
	median_object_normalized_sampling_rate double precision,

	--Per-Trajectory Statistics
	min_trajectory_points bigint,
	max_trajectory_points bigint,
	avg_trajectory_points double precision,
	stddev_trajectory_points double precision,
	median_trajectory_points double precision,
	
	min_trajectory_duration interval,
	max_trajectory_duration interval,
	avg_trajectory_duration interval,
	stddev_trajectory_duration interval,
	median_trajectory_duration interval,
	
	min_trajectory_length double precision,
	max_trajectory_length double precision,
	avg_trajectory_length double precision,
	stddev_trajectory_length double precision,
	median_trajectory_length double precision,
	
	min_trajectory_displacement double precision,
	max_trajectory_displacement double precision,
	avg_trajectory_displacement double precision,
	stddev_trajectory_displacement double precision,
	median_trajectory_displacement double precision,
	
	min_trajectory_area bigint,
	max_trajectory_area bigint,
	avg_trajectory_area double precision,
	stddev_trajectory_area double precision,
	median_trajectory_area double precision,

	min_trajectory_gyradius double precision,
	max_trajectory_gyradius double precision,
	avg_trajectory_gyradius double precision,
	stddev_trajectory_gyradius double precision,
	median_trajectory_gyradius double precision,

	min_trajectory_angle_xx_avg double precision,
	max_trajectory_angle_xx_avg double precision,
	avg_trajectory_angle_xx_avg double precision,
	stddev_trajectory_angle_xx_avg double precision,
	median_trajectory_angle_xx_avg double precision,
	
	min_trajectory_angle_xx double precision,
	max_trajectory_angle_xx double precision,
	avg_trajectory_angle_xx double precision,
	stddev_trajectory_angle_xx double precision,
	median_trajectory_angle_xx double precision,
	
	min_trajectory_average_speed double precision,
	max_trajectory_average_speed double precision,
	avg_trajectory_average_speed double precision,
	stddev_trajectory_average_speed double precision,
	median_trajectory_average_speed double precision,
	
	min_trajectory_normalized_sampling_rate double precision,
	max_trajectory_normalized_sampling_rate double precision,
	avg_trajectory_normalized_sampling_rate double precision,
	stddev_trajectory_normalized_sampling_rate double precision,
	median_trajectory_normalized_sampling_rate double precision,

	--Per-Segment Statistics
	min_segment_duration interval,
	max_segment_duration interval,
	avg_segment_duration interval,
	stddev_segment_duration interval,
	median_segment_duration interval,
	
	min_segment_length double precision,
	max_segment_length double precision,
	avg_segment_length double precision,
	stddev_segment_length double precision,
	median_segment_length double precision,
	
	min_segment_area bigint,
	max_segment_area bigint,
	avg_segment_area double precision,
	stddev_segment_area double precision,
	median_segment_area double precision,

	min_segment_angle_xx double precision,
	max_segment_angle_xx double precision,
	avg_segment_angle_xx double precision,
	stddev_segment_angle_xx double precision,
	median_segment_angle_xx double precision,
	
	min_segment_average_speed double precision,
	max_segment_average_speed double precision,
	avg_segment_average_speed double precision,
	stddev_segment_average_speed double precision,
	median_segment_average_speed double precision,
	
	PRIMARY KEY (dataset),
	FOREIGN KEY (dataset) REFERENCES HDatasets(id)
		ON DELETE CASCADE
		ON UPDATE CASCADE
);

/** @brief This function calculates the statistics for the 
 * 	dataset
 *
 *	@param[in] dataset_name the name of the dataset
 *
 */
CREATE FUNCTION HDatasetsStatistics(dataset_name text) RETURNS boolean AS $$
DECLARE
	dataset_id integer;

	dataset_segment_storage boolean;
	dataset_trajectory_storage boolean;
	dataset_subtrajectory_storage boolean;

	dataset_partitioning_t_range interval;
	dataset_partitioning_x_range integer;
	dataset_partitioning_y_range integer;

	nr_objects_new bigint;
	nr_trajectories_new bigint;
	nr_segments_new bigint;
	nr_points_new bigint;

	tmin_new timestamp;
	tmax_new timestamp;
	lx_new integer;
	ly_new integer;
	hx_new integer;
	hy_new integer;
	llon_new double precision;
	llat_new double precision;
	hlon_new double precision;
	hlat_new double precision;

	points_centroid_t_new timestamp;
	points_centroid_x_new integer;
	points_centroid_y_new integer;
	points_centroid_lon_new double precision;
	points_centroid_lat_new double precision;

	trajectories_centroid_t_new timestamp;
	trajectories_centroid_x_new integer;
	trajectories_centroid_y_new integer;
	trajectories_centroid_lon_new double precision;
	trajectories_centroid_lat_new double precision;
	
	duration_new interval;
	area_new bigint;
	normalized_sampling_rate_new double precision;

	min_object_trajectories_new bigint;
	max_object_trajectories_new bigint;
	avg_object_trajectories_new double precision;
	stddev_object_trajectories_new double precision;
	median_object_trajectories_new double precision;

	min_object_normalized_sampling_rate_new double precision;
	max_object_normalized_sampling_rate_new double precision;
	avg_object_normalized_sampling_rate_new double precision;
	stddev_object_normalized_sampling_rate_new double precision;
	median_object_normalized_sampling_rate_new double precision;

	min_trajectory_points_new bigint;
	max_trajectory_points_new bigint;
	avg_trajectory_points_new double precision;
	stddev_trajectory_points_new double precision;
	median_trajectory_points_new double precision;
	
	min_trajectory_duration_new interval;
	max_trajectory_duration_new interval;
	avg_trajectory_duration_new interval;
	stddev_trajectory_duration_new interval;
	median_trajectory_duration_new interval;
	
	min_trajectory_length_new double precision;
	max_trajectory_length_new double precision;
	avg_trajectory_length_new double precision;
	stddev_trajectory_length_new double precision;
	median_trajectory_length_new double precision;
	
	min_trajectory_displacement_new double precision;
	max_trajectory_displacement_new double precision;
	avg_trajectory_displacement_new double precision;
	stddev_trajectory_displacement_new double precision;
	median_trajectory_displacement_new double precision;
	
	min_trajectory_area_new bigint;
	max_trajectory_area_new bigint;
	avg_trajectory_area_new double precision;
	stddev_trajectory_area_new double precision;
	median_trajectory_area_new double precision;

	min_trajectory_gyradius_new double precision;
	max_trajectory_gyradius_new double precision;
	avg_trajectory_gyradius_new double precision;
	stddev_trajectory_gyradius_new double precision;
	median_trajectory_gyradius_new double precision;

	min_trajectory_angle_xx_avg_new double precision;
	max_trajectory_angle_xx_avg_new double precision;
	avg_trajectory_angle_xx_avg_new double precision;
	stddev_trajectory_angle_xx_avg_new double precision;
	median_trajectory_angle_xx_avg_new double precision;
	
	min_trajectory_angle_xx_new double precision;
	max_trajectory_angle_xx_new double precision;
	avg_trajectory_angle_xx_new double precision;
	stddev_trajectory_angle_xx_new double precision;
	median_trajectory_angle_xx_new double precision;
	
	min_trajectory_average_speed_new double precision;
	max_trajectory_average_speed_new double precision;
	avg_trajectory_average_speed_new double precision;
	stddev_trajectory_average_speed_new double precision;
	median_trajectory_average_speed_new double precision;
	
	min_trajectory_normalized_sampling_rate_new double precision;
	max_trajectory_normalized_sampling_rate_new double precision;
	avg_trajectory_normalized_sampling_rate_new double precision;
	stddev_trajectory_normalized_sampling_rate_new double precision;
	median_trajectory_normalized_sampling_rate_new double precision;

	min_segment_duration_new interval;
	max_segment_duration_new interval;
	avg_segment_duration_new interval;
	stddev_segment_duration_new interval;
	median_segment_duration_new interval;
	
	min_segment_length_new double precision;
	max_segment_length_new double precision;
	avg_segment_length_new double precision;
	stddev_segment_length_new double precision;
	median_segment_length_new double precision;
	
	min_segment_area_new bigint;
	max_segment_area_new bigint;
	avg_segment_area_new double precision;
	stddev_segment_area_new double precision;
	median_segment_area_new double precision;

	min_segment_angle_xx_new double precision;
	max_segment_angle_xx_new double precision;
	avg_segment_angle_xx_new double precision;
	stddev_segment_angle_xx_new double precision;
	median_segment_angle_xx_new double precision;
	
	min_segment_average_speed_new double precision;
	max_segment_average_speed_new double precision;
	avg_segment_average_speed_new double precision;
	stddev_segment_average_speed_new double precision;
	median_segment_average_speed_new double precision;
BEGIN
	SELECT id, segment_storage, trajectory_storage, subtrajectory_storage, partitioning_t_range, partitioning_x_range, partitioning_y_range INTO dataset_id, dataset_segment_storage, dataset_trajectory_storage, dataset_subtrajectory_storage, dataset_partitioning_t_range, dataset_partitioning_x_range, dataset_partitioning_y_range FROM HDatasets WHERE name = dataset_name;
	IF NOT FOUND THEN
		RETURN false;
	END IF;

	IF dataset_partitioning_t_range IS NOT NULL OR dataset_partitioning_x_range IS NOT NULL OR dataset_partitioning_y_range IS NOT NULL THEN
		RETURN false;
	END IF;

	EXECUTE 'SELECT count(*) FROM ' || quote_ident(dataset_name || '_obj')::regclass || ';' INTO nr_objects_new;
	EXECUTE 'SELECT count(*) FROM ' || quote_ident(dataset_name || '_traj')::regclass || ';' INTO nr_trajectories_new;

	IF dataset_trajectory_storage THEN
		EXECUTE '
			SELECT
				sum(NrSegments(traj)), sum(NrPoints(traj))
				, min(getTi(traj::BoxST)), max(getTe(traj::BoxST)), min(getLx(traj::BoxST)), min(getLy(traj::BoxST)), max(getHx(traj::BoxST)), max(getHy(traj::BoxST))
				, ''epoch''::timestamp + ''00:00:01''::interval * avg(extract(epoch from getT(centroid(traj)))), round(avg(getX(centroid(traj)))), round(avg(getY(centroid(traj))))
				, min(NrPoints(traj)), max(NrPoints(traj)), avg(NrPoints(traj)), stddev_pop(NrPoints(traj)), percentile_cont(0.5) WITHIN GROUP (ORDER BY NrPoints(traj))
				, min(duration(traj)), max(duration(traj)), avg(duration(traj)), stddev_pop(extract(epoch from duration(traj))) * ''00:00:01''::interval, percentile_cont(0.5) WITHIN GROUP (ORDER BY duration(traj))
				, min(length(traj)), max(length(traj)), avg(length(traj)), stddev_pop(length(traj)), percentile_cont(0.5) WITHIN GROUP (ORDER BY length(traj))
				, min(displacement(traj)), max(displacement(traj)), avg(displacement(traj)), stddev_pop(displacement(traj)), percentile_cont(0.5) WITHIN GROUP (ORDER BY displacement(traj))
				, min(area(getSp(traj::BoxST))), max(area(getSp(traj::BoxST))), avg(area(getSp(traj::BoxST))), stddev_pop(area(getSp(traj::BoxST))), percentile_cont(0.5) WITHIN GROUP (ORDER BY area(getSp(traj::BoxST)))
				, min(gyradius(traj)), max(gyradius(traj)), avg(gyradius(traj)), stddev_pop(gyradius(traj)), percentile_cont(0.5) WITHIN GROUP (ORDER BY gyradius(traj))
				, min(angleXXAvg(traj)), max(angleXXAvg(traj)), avg(angleXXAvg(traj)), stddev_pop(angleXXAvg(traj)), percentile_cont(0.5) WITHIN GROUP (ORDER BY angleXXAvg(traj))
				, min(angleXX(traj)), max(angleXX(traj)), avg(angleXX(traj)), stddev_pop(angleXX(traj)), percentile_cont(0.5) WITHIN GROUP (ORDER BY angleXX(traj))
				, min(averageSpeed(traj)), max(averageSpeed(traj)), avg(averageSpeed(traj)), stddev_pop(averageSpeed(traj)), percentile_cont(0.5) WITHIN GROUP (ORDER BY averageSpeed(traj))
				, min(normalizedSamplingRate(traj)), max(normalizedSamplingRate(traj)), avg(normalizedSamplingRate(traj)), stddev_pop(normalizedSamplingRate(traj)), percentile_cont(0.5) WITHIN GROUP (ORDER BY normalizedSamplingRate(traj))
			FROM ' || quote_ident(dataset_name || '_traj')::regclass || ';'
		INTO
			nr_segments_new, nr_points_new
			, tmin_new, tmax_new, lx_new, ly_new, hx_new, hy_new
			, trajectories_centroid_t_new, trajectories_centroid_x_new, trajectories_centroid_y_new
			, min_trajectory_points_new, max_trajectory_points_new, avg_trajectory_points_new, stddev_trajectory_points_new, median_trajectory_points_new
			, min_trajectory_duration_new, max_trajectory_duration_new, avg_trajectory_duration_new, stddev_trajectory_duration_new, median_trajectory_duration_new
			, min_trajectory_length_new, max_trajectory_length_new, avg_trajectory_length_new, stddev_trajectory_length_new, median_trajectory_length_new
			, min_trajectory_displacement_new, max_trajectory_displacement_new, avg_trajectory_displacement_new, stddev_trajectory_displacement_new, median_trajectory_displacement_new
			, min_trajectory_area_new, max_trajectory_area_new, avg_trajectory_area_new, stddev_trajectory_area_new, median_trajectory_area_new
			, min_trajectory_gyradius_new, max_trajectory_gyradius_new, avg_trajectory_gyradius_new, stddev_trajectory_gyradius_new, median_trajectory_gyradius_new
			, min_trajectory_angle_xx_avg_new, max_trajectory_angle_xx_avg_new, avg_trajectory_angle_xx_avg_new, stddev_trajectory_angle_xx_avg_new, median_trajectory_angle_xx_avg_new
			, min_trajectory_angle_xx_new, max_trajectory_angle_xx_new, avg_trajectory_angle_xx_new, stddev_trajectory_angle_xx_new, median_trajectory_angle_xx_new
			, min_trajectory_average_speed_new, max_trajectory_average_speed_new, avg_trajectory_average_speed_new, stddev_trajectory_average_speed_new, median_trajectory_average_speed_new
			, min_trajectory_normalized_sampling_rate_new, max_trajectory_normalized_sampling_rate_new, avg_trajectory_normalized_sampling_rate_new, stddev_trajectory_normalized_sampling_rate_new, median_trajectory_normalized_sampling_rate_new;

		SELECT
			getLon(lpll), getLat(lpll), getLon(hpll), getLat(hpll)
			, getLon(tc), getLat(tc)
		FROM (
			SELECT
				PointLL(PointSP(lx_new, ly_new), dataset_id) AS lpll, PointLL(PointSP(hx_new, hy_new), dataset_id) AS hpll
				, PointLL(PointSP(trajectories_centroid_x_new, trajectories_centroid_y_new), dataset_id) AS tc
		) AS tmp
		INTO llon_new, llat_new, hlon_new, hlat_new, trajectories_centroid_lon_new, trajectories_centroid_lat_new;

		EXECUTE '
			SELECT ''epoch''::timestamp + ''00:00:01''::interval * (sum(extract(epoch from t) * nr_points)::numeric / $1), round(sum(x::bigint * nr_points)::numeric / $1), round(sum(y::bigint * nr_points)::numeric / $1)
			FROM (
				SELECT getT(centroid(traj)) AS t, getX(centroid(traj)) AS x, getY(centroid(traj)) AS y, NrPoints(traj) AS nr_points
				FROM ' || quote_ident(dataset_name || '_traj')::regclass || '
			) AS tmp;'
		INTO points_centroid_t_new, points_centroid_x_new, points_centroid_y_new
		USING nr_points_new;
		
		SELECT getLon(pc), getLat(pc)
		FROM (
			SELECT PointLL(PointSP(points_centroid_x_new, points_centroid_y_new), dataset_id) AS pc
		) AS tmp
		INTO points_centroid_lon_new, points_centroid_lat_new;

		EXECUTE '
			SELECT
				min(nr_trajectories), max(nr_trajectories), avg(nr_trajectories), stddev_pop(nr_trajectories), percentile_cont(0.5) WITHIN GROUP (ORDER BY nr_trajectories)
				, min(normalized_sampling_rate), max(normalized_sampling_rate), avg(normalized_sampling_rate), stddev_pop(normalized_sampling_rate), percentile_cont(0.5) WITHIN GROUP (ORDER BY normalized_sampling_rate)
			FROM (
				SELECT obj_id
					, count(*) AS nr_trajectories
					, CASE WHEN max(getT(lastPoint(traj))) > min(getT(firstPoint(traj))) THEN sum(NrPoints(traj)) / extract(epoch from max(getT(lastPoint(traj))) - min(getT(firstPoint(traj)))) ELSE NULL END AS normalized_sampling_rate
				FROM ' || quote_ident(dataset_name || '_traj')::regclass || '
				GROUP BY obj_id
			) AS tmp;'
		INTO
			min_object_trajectories_new, max_object_trajectories_new, avg_object_trajectories_new, stddev_object_trajectories_new, median_object_trajectories_new
			, min_object_normalized_sampling_rate_new, max_object_normalized_sampling_rate_new, avg_object_normalized_sampling_rate_new, stddev_object_normalized_sampling_rate_new, median_object_normalized_sampling_rate_new;

		EXECUTE '
			SELECT
				min(duration(getT(seg))), max(duration(getT(seg))), avg(duration(getT(seg))), stddev_pop(extract(epoch from duration(getT(seg)))) * ''00:00:01''::interval, percentile_cont(0.5) WITHIN GROUP (ORDER BY duration(getT(seg)))
				, min(length(getSp(seg))), max(length(getSp(seg))), avg(length(getSp(seg))), stddev_pop(length(getSp(seg))), percentile_cont(0.5) WITHIN GROUP (ORDER BY length(getSp(seg)))
				, min(area(getSp(seg)::BoxSP)), max(area(getSp(seg)::BoxSP)), avg(area(getSp(seg)::BoxSP)), stddev_pop(area(getSp(seg)::BoxSP)), percentile_cont(0.5) WITHIN GROUP (ORDER BY area(getSp(seg)::BoxSP))
				, min(angleXX(getSp(seg)::LineSP)), max(angleXX(getSp(seg)::LineSP)), avg(angleXX(getSp(seg)::LineSP)), stddev_pop(angleXX(getSp(seg)::LineSP)), percentile_cont(0.5) WITHIN GROUP (ORDER BY angleXX(getSp(seg)::LineSP))
				, min(averageSpeed(seg)), max(averageSpeed(seg)), avg(averageSpeed(seg)), stddev_pop(averageSpeed(seg)), percentile_cont(0.5) WITHIN GROUP (ORDER BY averageSpeed(seg))
			FROM (
				SELECT segments(traj) AS seg
				FROM ' || quote_ident(dataset_name || '_traj')::regclass || '
			) AS tmp;'
		INTO
			min_segment_duration_new, max_segment_duration_new, avg_segment_duration_new, stddev_segment_duration_new, median_segment_duration_new
			, min_segment_length_new, max_segment_length_new, avg_segment_length_new, stddev_segment_length_new, median_segment_length_new
			, min_segment_area_new, max_segment_area_new, avg_segment_area_new, stddev_segment_area_new, median_segment_area_new
			, min_segment_angle_xx_new, max_segment_angle_xx_new, avg_segment_angle_xx_new, stddev_segment_angle_xx_new, median_segment_angle_xx_new
			, min_segment_average_speed_new, max_segment_average_speed_new, avg_segment_average_speed_new, stddev_segment_average_speed_new, median_segment_average_speed_new;
	ELSIF dataset_segment_storage THEN
		EXECUTE '
			SELECT sum(nr_segments), sum(nr_segments + 1)
			FROM (
				SELECT obj_id, traj_id, count(*) AS nr_segments
				FROM ' || quote_ident(dataset_name || '_seg')::regclass || '
				GROUP BY obj_id, traj_id
			) AS tmp;'
		INTO nr_segments_new, nr_points_new;

		EXECUTE '
			SELECT
				min(getTi(seg::BoxST)), max(getTe(seg::BoxST)), min(getLx(seg::BoxST)), min(getLy(seg::BoxST)), max(getHx(seg::BoxST)), max(getHy(seg::BoxST))
				, min(duration(getT(seg))), max(duration(getT(seg))), avg(duration(getT(seg))), stddev_pop(extract(epoch from duration(getT(seg)))) * ''00:00:01''::interval, percentile_cont(0.5) WITHIN GROUP (ORDER BY duration(getT(seg)))
				, min(length(getSp(seg))), max(length(getSp(seg))), avg(length(getSp(seg))), stddev_pop(length(getSp(seg))), percentile_cont(0.5) WITHIN GROUP (ORDER BY length(getSp(seg)))
				, min(area(getSp(seg)::BoxSP)), max(area(getSp(seg)::BoxSP)), avg(area(getSp(seg)::BoxSP)), stddev_pop(area(getSp(seg)::BoxSP)), percentile_cont(0.5) WITHIN GROUP (ORDER BY area(getSp(seg)::BoxSP))
				, min(angleXX(getSp(seg)::LineSP)), max(angleXX(getSp(seg)::LineSP)), avg(angleXX(getSp(seg)::LineSP)), stddev_pop(angleXX(getSp(seg)::LineSP)), percentile_cont(0.5) WITHIN GROUP (ORDER BY angleXX(getSp(seg)::LineSP))
				, min(averageSpeed(seg)), max(averageSpeed(seg)), avg(averageSpeed(seg)), stddev_pop(averageSpeed(seg)), percentile_cont(0.5) WITHIN GROUP (ORDER BY averageSpeed(seg))
			FROM ' || quote_ident(dataset_name || '_seg')::regclass || ';'
		INTO
			tmin_new, tmax_new, lx_new, ly_new, hx_new, hy_new
			, min_segment_duration_new, max_segment_duration_new, avg_segment_duration_new, stddev_segment_duration_new, median_segment_duration_new
			, min_segment_length_new, max_segment_length_new, avg_segment_length_new, stddev_segment_length_new, median_segment_length_new
			, min_segment_area_new, max_segment_area_new, avg_segment_area_new, stddev_segment_area_new, median_segment_area_new
			, min_segment_angle_xx_new, max_segment_angle_xx_new, avg_segment_angle_xx_new, stddev_segment_angle_xx_new, median_segment_angle_xx_new
			, min_segment_average_speed_new, max_segment_average_speed_new, avg_segment_average_speed_new, stddev_segment_average_speed_new, median_segment_average_speed_new;

		SELECT getLon(lpll), getLat(lpll), getLon(hpll), getLat(hpll)
		FROM (
			SELECT PointLL(PointSP(lx_new, ly_new), dataset_id) AS lpll, PointLL(PointSP(hx_new, hy_new), dataset_id) AS hpll
		) AS tmp
		INTO llon_new, llat_new, hlon_new, hlat_new;
	ELSE
		RETURN false;
	END IF;

	duration_new := tmax_new - tmin_new;
	area_new := area(BoxSP(lx_new, ly_new, hx_new, hy_new));

	IF duration_new > '00:00:00' THEN
		normalized_sampling_rate_new := nr_points_new / extract(epoch from duration_new);
	END IF;

	IF (SELECT exists(SELECT 1 FROM HDatasets_Statistics WHERE dataset = dataset_id LIMIT 1)) THEN
		UPDATE HDatasets_Statistics
		SET
			nr_objects = nr_objects_new,
			nr_trajectories = nr_trajectories_new,
			nr_segments = nr_segments_new,
			nr_points = nr_points_new,

			tmin = tmin_new,
			tmax = tmax_new,
			lx = lx_new,
			ly = ly_new,
			hx = hx_new,
			hy = hy_new,
			llon = llon_new,
			llat = llat_new,
			hlon = hlon_new,
			hlat = hlat_new,

			points_centroid_t = points_centroid_t_new,
			points_centroid_x = points_centroid_x_new,
			points_centroid_y = points_centroid_y_new,
			points_centroid_lon = points_centroid_lon_new,
			points_centroid_lat = points_centroid_lat_new,

			trajectories_centroid_t = trajectories_centroid_t_new,
			trajectories_centroid_x = trajectories_centroid_x_new,
			trajectories_centroid_y = trajectories_centroid_y_new,
			trajectories_centroid_lon = trajectories_centroid_lon_new,
			trajectories_centroid_lat = trajectories_centroid_lat_new,

			duration = duration_new,
			area = area_new,
			normalized_sampling_rate = normalized_sampling_rate_new,

			min_object_trajectories = min_object_trajectories_new,
			max_object_trajectories = max_object_trajectories_new,
			avg_object_trajectories = avg_object_trajectories_new,
			stddev_object_trajectories = stddev_object_trajectories_new,
			median_object_trajectories = median_object_trajectories_new,

			min_object_normalized_sampling_rate = min_object_normalized_sampling_rate_new,
			max_object_normalized_sampling_rate = max_object_normalized_sampling_rate_new,
			avg_object_normalized_sampling_rate = avg_object_normalized_sampling_rate_new,
			stddev_object_normalized_sampling_rate = stddev_object_normalized_sampling_rate_new,
			median_object_normalized_sampling_rate = median_object_normalized_sampling_rate_new,

			min_trajectory_points = min_trajectory_points_new,
			max_trajectory_points = max_trajectory_points_new,
			avg_trajectory_points = avg_trajectory_points_new,
			stddev_trajectory_points = stddev_trajectory_points_new,
			median_trajectory_points = median_trajectory_points_new,

			min_trajectory_duration = min_trajectory_duration_new,
			max_trajectory_duration = max_trajectory_duration_new,
			avg_trajectory_duration = avg_trajectory_duration_new,
			stddev_trajectory_duration = stddev_trajectory_duration_new,
			median_trajectory_duration = median_trajectory_duration_new,

			min_trajectory_length = min_trajectory_length_new,
			max_trajectory_length = max_trajectory_length_new,
			avg_trajectory_length = avg_trajectory_length_new,
			stddev_trajectory_length = stddev_trajectory_length_new,
			median_trajectory_length = median_trajectory_length_new,

			min_trajectory_displacement = min_trajectory_displacement_new,
			max_trajectory_displacement = max_trajectory_displacement_new,
			avg_trajectory_displacement = avg_trajectory_displacement_new,
			stddev_trajectory_displacement = stddev_trajectory_displacement_new,
			median_trajectory_displacement = median_trajectory_displacement_new,

			min_trajectory_area = min_trajectory_area_new,
			max_trajectory_area = max_trajectory_area_new,
			avg_trajectory_area = avg_trajectory_area_new,
			stddev_trajectory_area = stddev_trajectory_area_new,
			median_trajectory_area = median_trajectory_area_new,

			min_trajectory_gyradius = min_trajectory_gyradius_new,
			max_trajectory_gyradius = max_trajectory_gyradius_new,
			avg_trajectory_gyradius = avg_trajectory_gyradius_new,
			stddev_trajectory_gyradius = stddev_trajectory_gyradius_new,
			median_trajectory_gyradius = median_trajectory_gyradius_new,

			min_trajectory_angle_xx_avg = min_trajectory_angle_xx_avg_new,
			max_trajectory_angle_xx_avg = max_trajectory_angle_xx_avg_new,
			avg_trajectory_angle_xx_avg = avg_trajectory_angle_xx_avg_new,
			stddev_trajectory_angle_xx_avg = stddev_trajectory_angle_xx_avg_new,
			median_trajectory_angle_xx_avg = median_trajectory_angle_xx_avg_new,

			min_trajectory_angle_xx = min_trajectory_angle_xx_new,
			max_trajectory_angle_xx = max_trajectory_angle_xx_new,
			avg_trajectory_angle_xx = avg_trajectory_angle_xx_new,
			stddev_trajectory_angle_xx = stddev_trajectory_angle_xx_new,
			median_trajectory_angle_xx = median_trajectory_angle_xx_new,

			min_trajectory_average_speed = min_trajectory_average_speed_new,
			max_trajectory_average_speed = max_trajectory_average_speed_new,
			avg_trajectory_average_speed = avg_trajectory_average_speed_new,
			stddev_trajectory_average_speed = stddev_trajectory_average_speed_new,
			median_trajectory_average_speed = median_trajectory_average_speed_new,

			min_trajectory_normalized_sampling_rate = min_trajectory_normalized_sampling_rate_new,
			max_trajectory_normalized_sampling_rate = max_trajectory_normalized_sampling_rate_new,
			avg_trajectory_normalized_sampling_rate = avg_trajectory_normalized_sampling_rate_new,
			stddev_trajectory_normalized_sampling_rate = stddev_trajectory_normalized_sampling_rate_new,
			median_trajectory_normalized_sampling_rate = median_trajectory_normalized_sampling_rate_new,

			min_segment_duration = min_segment_duration_new,
			max_segment_duration = max_segment_duration_new,
			avg_segment_duration = avg_segment_duration_new,
			stddev_segment_duration = stddev_segment_duration_new,
			median_segment_duration = median_segment_duration_new,

			min_segment_length = min_segment_length_new,
			max_segment_length = max_segment_length_new,
			avg_segment_length = avg_segment_length_new,
			stddev_segment_length = stddev_segment_length_new,
			median_segment_length = median_segment_length_new,

			min_segment_area = min_segment_area_new,
			max_segment_area = max_segment_area_new,
			avg_segment_area = avg_segment_area_new,
			stddev_segment_area = stddev_segment_area_new,
			median_segment_area = median_segment_area_new,

			min_segment_angle_xx = min_segment_angle_xx_new,
			max_segment_angle_xx = max_segment_angle_xx_new,
			avg_segment_angle_xx = avg_segment_angle_xx_new,
			stddev_segment_angle_xx = stddev_segment_angle_xx_new,
			median_segment_angle_xx = median_segment_angle_xx_new,

			min_segment_average_speed = min_segment_average_speed_new,
			max_segment_average_speed = max_segment_average_speed_new,
			avg_segment_average_speed = avg_segment_average_speed_new,
			stddev_segment_average_speed = stddev_segment_average_speed_new,
			median_segment_average_speed = median_segment_average_speed_new
		WHERE dataset = dataset_id;
	ELSE
		INSERT INTO HDatasets_Statistics(
			dataset,
			nr_objects, nr_trajectories, nr_segments, nr_points,
			tmin, tmax, lx, ly, hx, hy, llon, llat, hlon, hlat,
			points_centroid_t, points_centroid_x, points_centroid_y, points_centroid_lon, points_centroid_lat,
			trajectories_centroid_t, trajectories_centroid_x, trajectories_centroid_y, trajectories_centroid_lon, trajectories_centroid_lat,
			duration, area, normalized_sampling_rate,

			min_object_trajectories,
			max_object_trajectories,
			avg_object_trajectories,
			stddev_object_trajectories,
			median_object_trajectories,

			min_object_normalized_sampling_rate,
			max_object_normalized_sampling_rate,
			avg_object_normalized_sampling_rate,
			stddev_object_normalized_sampling_rate,
			median_object_normalized_sampling_rate,

			min_trajectory_points,
			max_trajectory_points,
			avg_trajectory_points,
			stddev_trajectory_points,
			median_trajectory_points,

			min_trajectory_duration,
			max_trajectory_duration,
			avg_trajectory_duration,
			stddev_trajectory_duration,
			median_trajectory_duration,

			min_trajectory_length,
			max_trajectory_length,
			avg_trajectory_length,
			stddev_trajectory_length,
			median_trajectory_length,

			min_trajectory_displacement,
			max_trajectory_displacement,
			avg_trajectory_displacement,
			stddev_trajectory_displacement,
			median_trajectory_displacement,

			min_trajectory_area,
			max_trajectory_area,
			avg_trajectory_area,
			stddev_trajectory_area,
			median_trajectory_area,

			min_trajectory_gyradius,
			max_trajectory_gyradius,
			avg_trajectory_gyradius,
			stddev_trajectory_gyradius,
			median_trajectory_gyradius,

			min_trajectory_angle_xx_avg,
			max_trajectory_angle_xx_avg,
			avg_trajectory_angle_xx_avg,
			stddev_trajectory_angle_xx_avg,
			median_trajectory_angle_xx_avg,

			min_trajectory_angle_xx,
			max_trajectory_angle_xx,
			avg_trajectory_angle_xx,
			stddev_trajectory_angle_xx,
			median_trajectory_angle_xx,

			min_trajectory_average_speed,
			max_trajectory_average_speed,
			avg_trajectory_average_speed,
			stddev_trajectory_average_speed,
			median_trajectory_average_speed,

			min_trajectory_normalized_sampling_rate,
			max_trajectory_normalized_sampling_rate,
			avg_trajectory_normalized_sampling_rate,
			stddev_trajectory_normalized_sampling_rate,
			median_trajectory_normalized_sampling_rate,

			min_segment_duration,
			max_segment_duration,
			avg_segment_duration,
			stddev_segment_duration,
			median_segment_duration,

			min_segment_length,
			max_segment_length,
			avg_segment_length,
			stddev_segment_length,
			median_segment_length,

			min_segment_area,
			max_segment_area,
			avg_segment_area,
			stddev_segment_area,
			median_segment_area,

			min_segment_angle_xx,
			max_segment_angle_xx,
			avg_segment_angle_xx,
			stddev_segment_angle_xx,
			median_segment_angle_xx,

			min_segment_average_speed,
			max_segment_average_speed,
			avg_segment_average_speed,
			stddev_segment_average_speed,
			median_segment_average_speed
		) VALUES (
			dataset_id,
			nr_objects_new, nr_trajectories_new, nr_segments_new, nr_points_new,
			tmin_new, tmax_new, lx_new, ly_new, hx_new, hy_new, llon_new, llat_new, hlon_new, hlat_new,
			points_centroid_t_new, points_centroid_x_new, points_centroid_y_new, points_centroid_lon_new, points_centroid_lat_new,
			trajectories_centroid_t_new, trajectories_centroid_x_new, trajectories_centroid_y_new, trajectories_centroid_lon_new, trajectories_centroid_lat_new,
			duration_new, area_new, normalized_sampling_rate_new,

			min_object_trajectories_new,
			max_object_trajectories_new,
			avg_object_trajectories_new,
			stddev_object_trajectories_new,
			median_object_trajectories_new,

			min_object_normalized_sampling_rate_new,
			max_object_normalized_sampling_rate_new,
			avg_object_normalized_sampling_rate_new,
			stddev_object_normalized_sampling_rate_new,
			median_object_normalized_sampling_rate_new,

			min_trajectory_points_new,
			max_trajectory_points_new,
			avg_trajectory_points_new,
			stddev_trajectory_points_new,
			median_trajectory_points_new,

			min_trajectory_duration_new,
			max_trajectory_duration_new,
			avg_trajectory_duration_new,
			stddev_trajectory_duration_new,
			median_trajectory_duration_new,

			min_trajectory_length_new,
			max_trajectory_length_new,
			avg_trajectory_length_new,
			stddev_trajectory_length_new,
			median_trajectory_length_new,

			min_trajectory_displacement_new,
			max_trajectory_displacement_new,
			avg_trajectory_displacement_new,
			stddev_trajectory_displacement_new,
			median_trajectory_displacement_new,

			min_trajectory_area_new,
			max_trajectory_area_new,
			avg_trajectory_area_new,
			stddev_trajectory_area_new,
			median_trajectory_area_new,

			min_trajectory_gyradius_new,
			max_trajectory_gyradius_new,
			avg_trajectory_gyradius_new,
			stddev_trajectory_gyradius_new,
			median_trajectory_gyradius_new,

			min_trajectory_angle_xx_avg_new,
			max_trajectory_angle_xx_avg_new,
			avg_trajectory_angle_xx_avg_new,
			stddev_trajectory_angle_xx_avg_new,
			median_trajectory_angle_xx_avg_new,

			min_trajectory_angle_xx_new,
			max_trajectory_angle_xx_new,
			avg_trajectory_angle_xx_new,
			stddev_trajectory_angle_xx_new,
			median_trajectory_angle_xx_new,

			min_trajectory_average_speed_new,
			max_trajectory_average_speed_new,
			avg_trajectory_average_speed_new,
			stddev_trajectory_average_speed_new,
			median_trajectory_average_speed_new,

			min_trajectory_normalized_sampling_rate_new,
			max_trajectory_normalized_sampling_rate_new,
			avg_trajectory_normalized_sampling_rate_new,
			stddev_trajectory_normalized_sampling_rate_new,
			median_trajectory_normalized_sampling_rate_new,

			min_segment_duration_new,
			max_segment_duration_new,
			avg_segment_duration_new,
			stddev_segment_duration_new,
			median_segment_duration_new,

			min_segment_length_new,
			max_segment_length_new,
			avg_segment_length_new,
			stddev_segment_length_new,
			median_segment_length_new,

			min_segment_area_new,
			max_segment_area_new,
			avg_segment_area_new,
			stddev_segment_area_new,
			median_segment_area_new,

			min_segment_angle_xx_new,
			max_segment_angle_xx_new,
			avg_segment_angle_xx_new,
			stddev_segment_angle_xx_new,
			median_segment_angle_xx_new,

			min_segment_average_speed_new,
			max_segment_average_speed_new,
			avg_segment_average_speed_new,
			stddev_segment_average_speed_new,
			median_segment_average_speed_new
		);
	END IF;

	IF NOT dataset_trajectory_storage AND dataset_segment_storage THEN
		RETURN false;
	END IF;

	RETURN true;
END;
$$ LANGUAGE plpgsql STRICT;
