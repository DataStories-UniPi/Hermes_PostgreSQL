/*
 * Authors: Kiriakos Velissariou (kir.velissariou@gmail.com)
 */

CREATE OR REPLACE FUNCTION trajectory_transformation (dataset text, transf_method text, rate float DEFAULT 0.2, distance float DEFAULT 0.0, save boolean DEFAULT True, csv_file boolean DEFAULT True, start_date text DEFAULT 'None', end_date text DEFAULT 'None', step integer DEFAULT 0)
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


#-------------------------transformation functions----------------------------------
	def decrease_sampling_rate(dataset, generated_trajectories, rate):
		traj_qry = "SELECT * FROM " + dataset + "_traj"
		traj_result = plpy.execute(traj_qry)
		for i in range(0, len(traj_result)):
			seg_qry = "SELECT * FROM " + dataset + "_seg WHERE obj_id =" + str(traj_result[i]['obj_id']) + "AND traj_id =" + str(traj_result[i]['traj_id'])
			seg_result = plpy.execute(seg_qry)
			for j in range(0, len(seg_result)):
				stay_possibility = random.random()
				if stay_possibility > rate:
					traj_stripped = seg_result[j]['seg'].split(" '")
					traj_stripped = traj_stripped[0]
					traj_stripped = traj_stripped.replace("'", '')
					traj_stripped = traj_stripped.split()
					coords = xy_lanlot(traj_stripped[2], traj_stripped[3], "3857")
					coords = coords.split(' ')
					traj_stripped = traj_stripped[0] + " " + traj_stripped[1] + "," + coords[0] + "," + coords[1]
					generated_trajectories.write("%d,%d,%s\n" % (seg_result[j]['obj_id'], seg_result[j]['traj_id'], traj_stripped))

			traj_stripped = seg_result[len(seg_result) - 1]['seg'].split(" '")
			traj_stripped = traj_stripped[1]
			traj_stripped = traj_stripped.replace("'", '')
			traj_stripped = traj_stripped.split()
			coords = xy_lanlot(traj_stripped[2], traj_stripped[3], "3857")
			coords = coords.split(' ')
			traj_stripped = traj_stripped[0] + " " + traj_stripped[1] + "," + coords[0] + "," + coords[1]
			generated_trajectories.write("%d,%d,%s\n" % (seg_result[j]['obj_id'], seg_result[j]['traj_id'], traj_stripped))

		return

	def increase_sampling_rate(dataset, generated_trajectories, rate):
		traj_qry = "SELECT * FROM " + dataset + "_traj"
		traj_result = plpy.execute(traj_qry)
		for i in range(0, len(traj_result)):
			seg_qry = "SELECT * FROM " + dataset + "_seg WHERE obj_id =" + str(traj_result[i]['obj_id']) + "AND traj_id =" + str(traj_result[i]['traj_id'])
			seg_result = plpy.execute(seg_qry)
			for j in range(0, len(seg_result) - 1):
				traj_stripped = seg_result[j]['seg'].split(" '")
				traj_stripped = traj_stripped[0]
				traj_stripped = traj_stripped.replace("'", '')
				traj_stripped = traj_stripped.split()
				start_timestamp = traj_stripped[0] + " " + traj_stripped[1]
				x1 = traj_stripped[2]
				y1 = traj_stripped[3]
				traj_stripped = traj_stripped[0] + " " + traj_stripped[1] + "," + traj_stripped[2] + "," + traj_stripped[3]
				generated_trajectories.write("%d,%d,%s\n" % (seg_result[j]['obj_id'], seg_result[j]['traj_id'], traj_stripped))
				

				add_possibility = random.random()
				if add_possibility <= rate:
					next_traj_stripped = seg_result[j + 1]['seg'].split(" '")
					next_traj_stripped = next_traj_stripped[0]
					next_traj_stripped = next_traj_stripped.replace("'", '')
					next_traj_stripped = next_traj_stripped.split()
					end_timestamp = next_traj_stripped[0] + " " + next_traj_stripped[1]
					x2 = (next_traj_stripped[2])
					y2 = (next_traj_stripped[3])

					start_timestamp = datetime.strptime(start_timestamp, '%Y-%m-%d %H:%M:%S')
					end_timestamp = datetime.strptime(end_timestamp, '%Y-%m-%d %H:%M:%S')
					new_timestamp = date_between(start_timestamp, end_timestamp)
					new_timestamp = new_timestamp.strftime('%Y-%m-%d %H:%M:%S')
					start_timestamp = start_timestamp.strftime('%Y-%m-%d %H:%M:%S')
					end_timestamp = end_timestamp.strftime('%Y-%m-%d %H:%M:%S')
					at_instant_qry = "SELECT atInstant(SegmentST(" + "'" +  start_timestamp  + "'" + "," + x1 + "," + y1 + "," + "'" + end_timestamp + "'" + "," + x2 + "," + y2 + "), " + "'" + new_timestamp + "'" + ");" 
					interpolation = plpy.execute(at_instant_qry)

					interpolation = interpolation[0]['atinstant']
					interpolation = interpolation.replace("'", '')
					interpolation = interpolation.split()
					interpolation = interpolation[0] + " " + interpolation[1] + "," + interpolation[2] + "," + interpolation[3]
					generated_trajectories.write("%d,%d,%s\n" % (seg_result[j]['obj_id'], seg_result[j]['traj_id'], interpolation))

			traj_stripped = seg_result[len(seg_result) - 1]['seg'].split(" '")
			traj_stripped = traj_stripped[1]
			traj_stripped = traj_stripped.replace("'", '')
			traj_stripped = traj_stripped.split()
			start_timestamp = traj_stripped[0] + " " + traj_stripped[1]
			traj_stripped = traj_stripped[0] + " " + traj_stripped[1] + "," + traj_stripped[2] + "," + traj_stripped[3]
			generated_trajectories.write("%d,%d,%s\n" % (seg_result[j]['obj_id'], seg_result[j]['traj_id'], traj_stripped))


	def given_tstamp_sampling_rate(dataset, generated_trajectories, start_date, end_date, step):
		start_date = datetime.strptime(start_date, '%Y-%m-%d %H:%M:%S')
		end_date = datetime.strptime(end_date, '%Y-%m-%d %H:%M:%S')

		traj_qry = "SELECT * FROM " + dataset + "_traj"
		traj_result = plpy.execute(traj_qry)
		for i in range(0, len(traj_result)):
			seg_qry = "SELECT * FROM " + dataset + "_seg WHERE obj_id =" + str(traj_result[i]['obj_id']) + "AND traj_id =" + str(traj_result[i]['traj_id'])
			seg_result = plpy.execute(seg_qry)
			j = 0
			date_searching = start_date
			while j < len(seg_result) and date_searching <= end_date:
				traj_stripped = seg_result[j]['seg'].split(" '")
				traj_stripped = traj_stripped[0].rstrip()
				traj_stripped = traj_stripped.replace("'", '')
				traj_stripped = traj_stripped.split()
				point_date = traj_stripped[0] + " " + traj_stripped[1]
				point_date = datetime.strptime(point_date, '%Y-%m-%d %H:%M:%S')

				if point_date == date_searching:
					point_to_write = traj_stripped	[0] + " " + traj_stripped[1] + "," + traj_stripped[2] + "," + traj_stripped[3]
					generated_trajectories.write("%d,%d,%s\n" % (seg_result[j]['obj_id'], seg_result[j]['traj_id'], point_to_write))
					j += 1
					date_searching = date_searching + timedelta(seconds = step)
				elif point_date < date_searching:
					j += 1
				else:
					date_searching = date_searching + timedelta(seconds = step)
		return



#-------------------------Main----------------------------------
	generated_trajectories = open('new_traj.txt', 'w')
	generated_trajectories.write('objectID,trajectoryID,t,lon,lat\n')
	if transf_method == 'dec_sr':
		decrease_sampling_rate(dataset, generated_trajectories, rate)
	elif transf_method == 'inc_sr':
		increase_sampling_rate(dataset, generated_trajectories, rate)
	elif transf_method == 'time_sr':
		given_tstamp_sampling_rate(dataset, generated_trajectories, start_date, end_date, step)
	else:
		pass

	generated_trajectories.close()

	if save == True:
		plpy.execute("SELECT HLoader('transformed', 'Tranformed trajectories')")
		plpy.execute("SELECT HLoaderCSV_II('transformed', 'new_traj.txt')")
		plpy.execute("SELECT HDatasetsOfflineStatistics('transformed')")
		plpy.execute("CREATE INDEX ON transformed_seg USING gist (seg) WITH (FILLFACTOR = 100)")

	if not csv_file:
		os.remove('new_traj.txt')
		
	return 1
		
$$ LANGUAGE plpython3u;

SELECT trajectory_transformation('lol', 'dec_sr', 0.9, 0.3, True, False, '2008-12-31 19:29:30', '2008-12-31 19:29:42', 3);
--DROP FUNCTION trajectory_transformation(text, text, float, float, boolean, boolean);
--SELECT * FROM test_dataset_seg;
--select * from transformed_seg;
--DROP FUNCTION trajectory_transformation(text, text, float, float, boolean, boolean);