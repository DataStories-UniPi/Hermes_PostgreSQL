CREATE OR REPLACE FUNCTION data_transformation (traj_file text, transf_method text, rate float DEFAULT 0.1, distance float DEFAULT 0.0, traj_num integer  DEFAULT 1, save boolean DEFAULT True)
  RETURNS integer
AS $$
	import random
	import os
	def load_data_to_list(traj_file):
		"""	
		Input: A file containing a trajectory
		Output: A python list of lists containing the trajectory data
		"""
		original_trajectory = []
		inputfile = open(traj_file, 'r')
		next(inputfile)
		for line in inputfile:
			original_trajectory.append(line.strip().split(','))
		inputfile.close()
		return original_trajectory

	def decrease_sampling_rate(original_trajectory, generated_trajectories, rate, traj_num):
		"""
		Performs sampling with decreasing rate
		"""

		number_of_points_to_delete = int(len(original_trajectory) * rate)
		incremented_id = 1

		for i in range(0, traj_num):
			# create a copy of the list in order to delete points
			o_t_copy = list(original_trajectory)
			for i in range(number_of_points_to_delete):
				index_to_delete = random.randint(0, len(o_t_copy) - 1)
				del o_t_copy[index_to_delete]
			for line in o_t_copy:
				line[1] = incremented_id
				for j in range(0, len(line) - 1):
					generated_trajectories.write("%s," % line[j])	
				generated_trajectories.write("%s\n" % line[-1])
			incremented_id += 1
		generated_trajectories.close()
		return

	original_trajectory = load_data_to_list(traj_file)
	

	# File to be returned
	generated_trajectories = open('new_traj.txt', 'w')
	generated_trajectories.write('objectID,trajectoryID,t,lon,lat\n')

	if transf_method == "dec_sr":
		decrease_sampling_rate(original_trajectory, generated_trajectories, rate, traj_num)
	else:
		pass
	return 1
$$ LANGUAGE plpython3u;

--SELECT data_transformation('trajectory.txt', 'dec_sr', 0.8, 0.1, 1, False);
--DROP FUNCTION data_transformation(text,text,double precision,double precision,integer)