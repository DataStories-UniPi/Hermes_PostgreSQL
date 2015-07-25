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

#-------------------------helper functions----------------------------------
	def load_tstamps_file_to_list(tstamps_file):
		tstamps_list = []
		inputfile = open(tstamps_file, 'r')
		for line in inputfile:
			line = line.rstrip()
			tstamps_list.append(line)
		inputfile.close()
		return tstamps_list
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
					traj_stripped = traj_stripped[0] + " " + traj_stripped[1] + "," + traj_stripped[2] + "," + traj_stripped[3]
					generated_trajectories.write("%d,%d,%s\n" % (seg_result[j]['obj_id'], seg_result[j]['traj_id'], traj_stripped))
		generated_trajectories.close()
		return

	def increase_sampling_rate(dataset, generated_trajectories, rate):
		traj_qry = "SELECT * FROM " + dataset + "_traj"
		traj_result = plpy.execute(traj_qry)
		for i in range(0, len(traj_result)):
			seg_qry = "SELECT * FROM " + dataset + "_seg WHERE obj_id =" + str(traj_result[i]['obj_id']) + "AND traj_id =" + str(traj_result[i]['traj_id'])
			seg_result = plpy.execute(seg_qry)
			for j in range(0, len(seg_result)):

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

		generated_trajectories.close()
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

	#if save == True:
	#	plpy.execute("SELECT HLoader('transformed', 'Tranformed trajectories')")
	#	plpy.execute("SELECT HLoaderCSV_II('transformed', 'new_traj.txt')")
	#	plpy.execute("SELECT HDatasetsOfflineStatistics('transformed')")
	#	plpy.execute("CREATE INDEX ON transformed_seg USING gist (seg) WITH (FILLFACTOR = 100)")

	if not csv_file:
		os.remove('new_traj.txt')
		
	return 1
		
$$ LANGUAGE plpython3u;

SELECT trajectory_transformation('test_dataset', 'time_sr', 0.5, 0.3, True, True, '2008-12-31 19:29:30', '2008-12-31 19:29:42', 3);

--SELECT * FROM test_dataset_seg;
--DROP FUNCTION trajectory_transformation(text, text, float, float, boolean, boolean);