/**
 * @file
 * @author Kiriakos Velissariou (kir.velissariou@gmail.com).
 * @brief The file implements trajectory transformation with time interval
 *
 *
 */

/** @brief This function performs trajectory transformation within a specified time period, with a specified step
 *
 *	@param[in] dataset Name of the dataset
 *	@param[in] start_date Initial timestamp
 *	@param[in] end_date Last timestamp
 *	@param[in] step The step to be used in the search measured in seconds
 *	@param[in] save Boolean which indicates whether the new dataset will be saved in Hermes
 *	@param[in] new_dataset_name The name with which the transformed dataset will be stored in Hermes
 *	@param[in] csv_file Boolean which indicates whether the new dataset will exported in a cvs format (traj_new.txt)
 *
 *  @return Interger 1 to indicate succesfull completion
 *
 */
CREATE OR REPLACE FUNCTION trajectory_transformation_time_sr (dataset text, start_date text, end_date text, step integer,save boolean DEFAULT True, new_dataset_name text DEFAULT 'transformed', csv_file boolean DEFAULT True)
RETURNS integer
AS $$
	import random
	import os
	from datetime import datetime
	from datetime import timedelta
	from random import randrange

#-------------------------helper functions----------------------------------
	def date_between(start, end):
		"""
		This function will return a random datetime between two datetime
		objects.
		"""
		delta = end - start
		int_delta = (delta.days * 24 * 60 * 60) + delta.seconds
		random_second = randrange(1, int_delta)
		return start + timedelta(seconds=random_second)

	def xy_lanlot(x, y, srid):
		"Returns the given points converted from the given srid to 4326"

		def helper_xy_lanlot(x, y, srid):
			qry = "SELECT ST_AsText(ST_Transform(ST_GeomFromText('POINT(" + x + " " + y + ")'," + srid + "), 4326))"
			a = plpy.execute(qry)
			return a

		a = helper_xy_lanlot(x, y, srid)
		result = a[0]['st_astext']
		result = result.strip('POINT')
		result = result.strip('(')
		result = result.strip(')')
		return result

#---------------------------Main--------------------------------------------
	start_date_name = start_date.replace(':', '_')
	end_date_name = end_date.replace(':', '_')
	csv_name = dataset + '_transformed' + '__timesr__' + start_date_name + "__" + end_date_name + "__" + str(step) + ".txt"
	generated_trajectories = open(csv_name, 'w')
	generated_trajectories.write('objectID,trajectoryID,t,lon,lat\n')
	new_start_date = datetime.strptime(start_date, '%Y-%m-%d %H:%M:%S')
	new_end_date = datetime.strptime(end_date, '%Y-%m-%d %H:%M:%S')

	traj_qry = "SELECT * FROM " + dataset + "_traj"
	traj_result = plpy.execute(traj_qry)
	for i in range(0, len(traj_result)):
		seg_qry = "SELECT * FROM " + dataset + "_seg WHERE obj_id =" + str(traj_result[i]['obj_id']) + "AND traj_id =" + str(traj_result[i]['traj_id'])
		seg_result = plpy.execute(seg_qry)
		j = 0
		date_searching = new_start_date
		while j < len(seg_result) and date_searching <= new_end_date:
			traj_stripped = seg_result[j]['seg'].split(" '")
			traj_stripped = traj_stripped[0].rstrip()
			traj_stripped = traj_stripped.replace("'", '')
			traj_stripped = traj_stripped.split()
			point_date = traj_stripped[0] + " " + traj_stripped[1]
			point_date = datetime.strptime(point_date, '%Y-%m-%d %H:%M:%S')

			if point_date == date_searching:
				coords = xy_lanlot(traj_stripped[2], traj_stripped[3], "3857")
				coords = coords.split(' ')
				point_to_write = traj_stripped	[0] + " " + traj_stripped[1] + "," + coords[0] + "," + coords[1]
				generated_trajectories.write("%d,%d,%s\n" % (seg_result[j]['obj_id'], seg_result[j]['traj_id'], point_to_write))
				j += 1
				date_searching = date_searching + timedelta(seconds = step)
			elif point_date < date_searching:
				j += 1
			else:
				date_searching = date_searching + timedelta(seconds = step)

	generated_trajectories.close()

	if save:
		plpy.execute("SELECT HLoader('" + new_dataset_name + "_time_sr" + "', 'Tranformed trajectories')")
		plpy.execute("SELECT HLoaderCSV_II('" + new_dataset_name + "_time_sr" + "', '" + csv_name + "')")
		plpy.execute("SELECT HDatasetsOfflineStatistics('" + new_dataset_name + "_time_sr" + "')")
		plpy.execute("CREATE INDEX ON " + new_dataset_name + "_time_sr" + "_seg USING gist (seg) WITH (FILLFACTOR = 100)")

	if not csv_file:
		os.remove(csv_name)

	return 1
$$ LANGUAGE plpython3u;

