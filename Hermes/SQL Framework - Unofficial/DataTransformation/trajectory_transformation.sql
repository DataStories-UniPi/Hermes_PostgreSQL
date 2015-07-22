CREATE OR REPLACE FUNCTION trajectory_transformation (dataset text, transf_method text, rate float DEFAULT 0.8, distance float DEFAULT 0.0, save boolean DEFAULT True, csv_file boolean DEFAULT True)
RETURNS integer
AS $$
	import random

	generated_trajectories = open('new_traj.txt', 'w')
	generated_trajectories.write('objectID,trajectoryID,t,lon,lat\n')

	traj_qry = "SELECT * FROM " + dataset + "_traj"
	traj_result = plpy.execute(traj_qry)
	for i in range(0, len(traj_result)):
		seg_qry = "SELECT * FROM " + dataset + "_seg WHERE obj_id =" + str(traj_result[i]['obj_id']) + "AND traj_id =" + str(traj_result[i]['traj_id'])
		seg_result = plpy.execute(seg_qry)
		for j in range(0, len(seg_result)):
			stay_possibility = random.random()
			if stay_possibility > rate:
				generated_trajectories.write("%d,%d,%s\n" % (seg_result[j]['obj_id'], seg_result[j]['traj_id'], seg_result[j]['seg']))
	generated_trajectories.close()
	return 1
		
$$ LANGUAGE plpython3u;

SELECT trajectory_transformation('test_dataset', 'adfs');

--SELECT * FROM test_dataset_seg;
--DROP FUNCTION trajectory_transformation(text, text, float, float, boolean, boolean);