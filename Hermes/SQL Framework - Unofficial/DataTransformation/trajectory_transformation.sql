CREATE OR REPLACE FUNCTION trajectory_transformation (dataset text, transf_method text, rate float DEFAULT 0.2, distance float DEFAULT 0.0, save boolean DEFAULT True, csv_file boolean DEFAULT True)
RETURNS integer
AS $$
	import random
	import os

#-------------------------Helper functions----------------------------------
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

#-------------------------Main----------------------------------
	generated_trajectories = open('new_traj.txt', 'w')
	generated_trajectories.write('objectID,trajectoryID,t,lon,lat\n')
	if transf_method == 'dec_sr':
		decrease_sampling_rate(dataset, generated_trajectories, rate)
	else:
		pass

	if not csv_file:
		os.remove('new_traj.txt')
		
	return 1
		
$$ LANGUAGE plpython3u;

SELECT trajectory_transformation('test_dataset', 'dec_sr', 0.5, 0.3, True, True);

--SELECT * FROM test_dataset_seg;
--DROP FUNCTION trajectory_transformation(text, text, float, float, boolean, boolean);