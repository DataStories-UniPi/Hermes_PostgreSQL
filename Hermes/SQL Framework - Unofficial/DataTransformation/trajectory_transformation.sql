/*
 * Authors: Kiriakos Velissariou (kir.velissariou@gmail.com)
 */


CREATE OR REPLACE FUNCTION trajectory_transformation_dec_sr (dataset text, rate float, save boolean DEFAULT True, new_dataset_name text DEFAULT 'transformed', csv_file boolean DEFAULT True)
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

	generated_trajectories.close()

	if save:
		plpy.execute("SELECT HLoader('" + new_dataset_name + "', 'Tranformed trajectories')")
		plpy.execute("SELECT HLoaderCSV_II('" + new_dataset_name + "', 'new_traj.txt')")
		plpy.execute("SELECT HDatasetsOfflineStatistics('" + new_dataset_name + "')")
		plpy.execute("CREATE INDEX ON " + new_dataset_name + "_seg USING gist (seg) WITH (FILLFACTOR = 100)")

	if not csv_file:
		os.remove('new_traj.txt')
		
	return 1
$$ LANGUAGE plpython3u;

CREATE OR REPLACE FUNCTION trajectory_transformation_inc_sr (dataset text, rate float, save boolean DEFAULT True, new_dataset_name text DEFAULT 'transformed', csv_file boolean DEFAULT True)
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
	generated_trajectories = open('new_traj.txt', 'w')
	generated_trajectories.write('objectID,trajectoryID,t,lon,lat\n')

	traj_qry = "SELECT * FROM " + dataset + "_traj"
	traj_result = plpy.execute(traj_qry)
	for i in range(0, len(traj_result)):
		seg_qry = "SELECT * FROM " + dataset + "_seg WHERE obj_id =" + str(traj_result[i]['obj_id']) + "AND traj_id =" + str(traj_result[i]['traj_id'])
		seg_result = plpy.execute(seg_qry)
		for j in range(0, len(seg_result)):
			traj_stripped = seg_result[j]['seg'].split(" '")
			traj_stripped = traj_stripped[0]
			traj_stripped = traj_stripped.replace("'", '')
			traj_stripped = traj_stripped.split()
			coords = xy_lanlot(traj_stripped[2], traj_stripped[3], "3857")
			coords = coords.split(' ')
			start_timestamp = traj_stripped[0] + " " + traj_stripped[1]
			x1 = traj_stripped[2]
			y1 = traj_stripped[3]
			traj_stripped = traj_stripped[0] + " " + traj_stripped[1] + "," + coords[0] + "," + coords[1]
			generated_trajectories.write("%d,%d,%s\n" % (seg_result[j]['obj_id'], seg_result[j]['traj_id'], traj_stripped))
				

			add_possibility = random.random()
			if add_possibility <= rate:
				next_traj_stripped = seg_result[j]['seg'].split(" '")
				next_traj_stripped = next_traj_stripped[1]
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
				coords = xy_lanlot(interpolation[2], interpolation[3], "3857")
				coords = coords.split(' ')
				interpolation = interpolation[0] + " " + interpolation[1] + "," + coords[0] + "," + coords[1]
				generated_trajectories.write("%d,%d,%s\n" % (seg_result[j]['obj_id'], seg_result[j]['traj_id'], interpolation))

		traj_stripped = seg_result[len(seg_result) - 1]['seg'].split(" '")
		traj_stripped = traj_stripped[1]
		traj_stripped = traj_stripped.replace("'", '')
		traj_stripped = traj_stripped.split()
		coords = xy_lanlot(traj_stripped[2], traj_stripped[3], "3857")
		coords = coords.split(' ')
		start_timestamp = traj_stripped[0] + " " + traj_stripped[1]
		traj_stripped = traj_stripped[0] + " " + traj_stripped[1] + "," + coords[0] + "," + coords[1]
		generated_trajectories.write("%d,%d,%s\n" % (seg_result[j]['obj_id'], seg_result[j]['traj_id'], traj_stripped))

	generated_trajectories.close()

	if save:
		plpy.execute("SELECT HLoader('" + new_dataset_name + "', 'Tranformed trajectories')")
		plpy.execute("SELECT HLoaderCSV_II('" + new_dataset_name + "', 'new_traj.txt')")
		plpy.execute("SELECT HDatasetsOfflineStatistics('" + new_dataset_name + "')")
		plpy.execute("CREATE INDEX ON " + new_dataset_name + "_seg USING gist (seg) WITH (FILLFACTOR = 100)")

	if not csv_file:
		os.remove('new_traj.txt')
		
	return 1
$$ LANGUAGE plpython3u;	

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
	generated_trajectories = open('new_traj.txt', 'w')
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
		plpy.execute("SELECT HLoader('" + new_dataset_name + "', 'Tranformed trajectories')")
		plpy.execute("SELECT HLoaderCSV_II('" + new_dataset_name + "', 'new_traj.txt')")
		plpy.execute("SELECT HDatasetsOfflineStatistics('" + new_dataset_name + "')")
		plpy.execute("CREATE INDEX ON " + new_dataset_name + "_seg USING gist (seg) WITH (FILLFACTOR = 100)")

	if not csv_file:
		os.remove('new_traj.txt')
		
	return 1
$$ LANGUAGE plpython3u;	


CREATE OR REPLACE FUNCTION trajectory_transformation_add_noise (dataset text, rate float, distance float, save boolean DEFAULT True, new_dataset_name text DEFAULT 'transformed', csv_file boolean DEFAULT True)
RETURNS integer
AS $$
	import random
	import os
	import math
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
	generated_trajectories = open('new_traj.txt', 'w')
	generated_trajectories.write('objectID,trajectoryID,t,lon,lat\n')

	traj_qry = "SELECT * FROM " + dataset + "_traj"
	traj_result = plpy.execute(traj_qry)
	for i in range(0, len(traj_result)):
		seg_qry = "SELECT * FROM " + dataset + "_seg WHERE obj_id =" + str(traj_result[i]['obj_id']) + "AND traj_id =" + str(traj_result[i]['traj_id'])
		seg_result = plpy.execute(seg_qry)
		for j in range(0, len(seg_result)):
			traj_stripped = seg_result[j]['seg'].split(" '")
			traj_stripped = traj_stripped[0]
			traj_stripped = traj_stripped.replace("'", '')
			traj_stripped = traj_stripped.split()
			coords = xy_lanlot(traj_stripped[2], traj_stripped[3], "3857")
			coords = coords.split(' ')
			start_timestamp = traj_stripped[0] + " " + traj_stripped[1]
			x1 = traj_stripped[2]
			y1 = traj_stripped[3]
			traj_stripped = traj_stripped[0] + " " + traj_stripped[1] + "," + coords[0] + "," + coords[1]
			generated_trajectories.write("%d,%d,%s\n" % (seg_result[j]['obj_id'], seg_result[j]['traj_id'], traj_stripped))
				

			add_possibility = random.random()
			if add_possibility <= rate:
				next_traj_stripped = seg_result[j]['seg'].split(" '")
				next_traj_stripped = next_traj_stripped[1]
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

				#The noise addition will take place here
				interpolation = interpolation[0]['atinstant']
				interpolation = interpolation.replace("'", '')
				interpolation = interpolation.split()

				# Choose a random point inside a circle with center the point of the interpolation and radius = distance
				xc = float(interpolation[2])
				yc = float(interpolation[3])
				r = distance * math.sqrt(random.random())
				t = 2 * math.pi * random.random()
				x = str(xc + r * math.cos(t))
				y = str(yc + r * math.sin(t))
				coords = xy_lanlot(x, y, "3857")
				coords = coords.split(' ')
				interpolation = interpolation[0] + " " + interpolation[1] + "," + coords[0] + "," + coords[1]
				generated_trajectories.write("%d,%d,%s\n" % (seg_result[j]['obj_id'], seg_result[j]['traj_id'], interpolation))

		traj_stripped = seg_result[len(seg_result) - 1]['seg'].split(" '")
		traj_stripped = traj_stripped[1]
		traj_stripped = traj_stripped.replace("'", '')
		traj_stripped = traj_stripped.split()
		coords = xy_lanlot(traj_stripped[2], traj_stripped[3], "3857")
		coords = coords.split(' ')
		start_timestamp = traj_stripped[0] + " " + traj_stripped[1]
		traj_stripped = traj_stripped[0] + " " + traj_stripped[1] + "," + coords[0] + "," + coords[1]
		generated_trajectories.write("%d,%d,%s\n" % (seg_result[j]['obj_id'], seg_result[j]['traj_id'], traj_stripped))

	generated_trajectories.close()

	if save:
		plpy.execute("SELECT HLoader('" + new_dataset_name + "', 'Tranformed trajectories')")
		plpy.execute("SELECT HLoaderCSV_II('" + new_dataset_name + "', 'new_traj.txt')")
		plpy.execute("SELECT HDatasetsOfflineStatistics('" + new_dataset_name + "')")
		plpy.execute("CREATE INDEX ON " + new_dataset_name + "_seg USING gist (seg) WITH (FILLFACTOR = 100)")

	if not csv_file:
		os.remove('new_traj.txt')
		
	return 1
$$ LANGUAGE plpython3u;	

CREATE OR REPLACE FUNCTION trajectory_transformation_random_shift (dataset text, rate float, distance float, save boolean DEFAULT True, new_dataset_name text DEFAULT 'transformed', csv_file boolean DEFAULT True)
RETURNS integer
AS $$
	import random
	import os
	import math
	from datetime import datetime
	from datetime import timedelta
	from random import randrange

#-------------------------helper functions----------------------------------
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
	generated_trajectories = open('new_traj.txt', 'w')
	generated_trajectories.write('objectID,trajectoryID,t,lon,lat\n')

	traj_qry = "SELECT * FROM " + dataset + "_traj"
	traj_result = plpy.execute(traj_qry)
	for i in range(0, len(traj_result)):
		seg_qry = "SELECT * FROM " + dataset + "_seg WHERE obj_id =" + str(traj_result[i]['obj_id']) + "AND traj_id =" + str(traj_result[i]['traj_id'])
		seg_result = plpy.execute(seg_qry)
		for j in range(0, len(seg_result)):
			traj_stripped = seg_result[j]['seg'].split(" '")
			traj_stripped = traj_stripped[0]
			traj_stripped = traj_stripped.replace("'", '')
			traj_stripped = traj_stripped.split()
			shift_possibility = random.random()

			if shift_possibility > rate:
			# Don't shift the point
				coords = xy_lanlot(traj_stripped[2], traj_stripped[3], "3857")
				coords = coords.split(' ')
				traj_stripped = traj_stripped[0] + " " + traj_stripped[1] + "," + coords[0] + "," + coords[1]
				generated_trajectories.write("%d,%d,%s\n" % (seg_result[j]['obj_id'], seg_result[j]['traj_id'], traj_stripped))
				
			else:
				# Shift the point
				# Choose a random point inside a circle with the initial coordinates as its center
				xc = float(traj_stripped[2])
				yc = float(traj_stripped[3])
				r = distance * math.sqrt(random.random())
				t = 2 * math.pi * random.random()
				x = str(xc + r * math.cos(t))
				y = str(yc + r * math.sin(t))
				coords = xy_lanlot(x, y, "3857")
				coords = coords.split(' ')
				traj_stripped = traj_stripped[0] + " " + traj_stripped[1] + "," + coords[0] + "," + coords[1]
				generated_trajectories.write("%d,%d,%s\n" % (seg_result[j]['obj_id'], seg_result[j]['traj_id'], traj_stripped))

		traj_stripped = seg_result[len(seg_result) - 1]['seg'].split(" '")
		traj_stripped = traj_stripped[1]
		traj_stripped = traj_stripped.replace("'", '')
		traj_stripped = traj_stripped.split()
		coords = xy_lanlot(traj_stripped[2], traj_stripped[3], "3857")
		coords = coords.split(' ')
		start_timestamp = traj_stripped[0] + " " + traj_stripped[1]
		traj_stripped = traj_stripped[0] + " " + traj_stripped[1] + "," + coords[0] + "," + coords[1]
		generated_trajectories.write("%d,%d,%s\n" % (seg_result[j]['obj_id'], seg_result[j]['traj_id'], traj_stripped))

	generated_trajectories.close()

	if save:
		plpy.execute("SELECT HLoader('" + new_dataset_name + "', 'Tranformed trajectories')")
		plpy.execute("SELECT HLoaderCSV_II('" + new_dataset_name + "', 'new_traj.txt')")
		plpy.execute("SELECT HDatasetsOfflineStatistics('" + new_dataset_name + "')")
		plpy.execute("CREATE INDEX ON " + new_dataset_name + "_seg USING gist (seg) WITH (FILLFACTOR = 100)")

	if not csv_file:
		os.remove('new_traj.txt')
		
	return 1
$$ LANGUAGE plpython3u;	


CREATE OR REPLACE FUNCTION trajectory_transformation_synced_shift (dataset text, rate float, distance float, save boolean DEFAULT True, new_dataset_name text DEFAULT 'transformed', csv_file boolean DEFAULT True)
RETURNS integer
AS $$
	import random
	import os
	import math
	from datetime import datetime
	from datetime import timedelta
	from random import randrange

#-------------------------helper functions----------------------------------
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
	generated_trajectories = open('new_traj.txt', 'w')
	generated_trajectories.write('objectID,trajectoryID,t,lon,lat\n')

	traj_qry = "SELECT * FROM " + dataset + "_traj"
	traj_result = plpy.execute(traj_qry)
	r = distance * math.sqrt(random.random())
	t = 2 * math.pi * random.random()
	for i in range(0, len(traj_result)):
		seg_qry = "SELECT * FROM " + dataset + "_seg WHERE obj_id =" + str(traj_result[i]['obj_id']) + "AND traj_id =" + str(traj_result[i]['traj_id'])
		seg_result = plpy.execute(seg_qry)
		for j in range(0, len(seg_result)):
			traj_stripped = seg_result[j]['seg'].split(" '")
			traj_stripped = traj_stripped[0]
			traj_stripped = traj_stripped.replace("'", '')
			traj_stripped = traj_stripped.split()
			shift_possibility = random.random()

			if shift_possibility > rate:
			# Don't shift the point
				coords = xy_lanlot(traj_stripped[2], traj_stripped[3], "3857")
				coords = coords.split(' ')
				traj_stripped = traj_stripped[0] + " " + traj_stripped[1] + "," + coords[0] + "," + coords[1]
				generated_trajectories.write("%d,%d,%s\n" % (seg_result[j]['obj_id'], seg_result[j]['traj_id'], traj_stripped))
				
			else:
				# Shift the point
				# Choose a random point inside a circle with the initial coordinates as its center
				xc = float(traj_stripped[2])
				yc = float(traj_stripped[3])
				x = str(xc + r * math.cos(t))
				y = str(yc + r * math.sin(t))
				coords = xy_lanlot(x, y, "3857")
				coords = coords.split(' ')
				traj_stripped = traj_stripped[0] + " " + traj_stripped[1] + "," + coords[0] + "," + coords[1]
				generated_trajectories.write("%d,%d,%s\n" % (seg_result[j]['obj_id'], seg_result[j]['traj_id'], traj_stripped))

		traj_stripped = seg_result[len(seg_result) - 1]['seg'].split(" '")
		traj_stripped = traj_stripped[1]
		traj_stripped = traj_stripped.replace("'", '')
		traj_stripped = traj_stripped.split()
		coords = xy_lanlot(traj_stripped[2], traj_stripped[3], "3857")
		coords = coords.split(' ')
		start_timestamp = traj_stripped[0] + " " + traj_stripped[1]
		traj_stripped = traj_stripped[0] + " " + traj_stripped[1] + "," + coords[0] + "," + coords[1]
		generated_trajectories.write("%d,%d,%s\n" % (seg_result[j]['obj_id'], seg_result[j]['traj_id'], traj_stripped))

	generated_trajectories.close()

	if save:
		plpy.execute("SELECT HLoader('" + new_dataset_name + "', 'Tranformed trajectories')")
		plpy.execute("SELECT HLoaderCSV_II('" + new_dataset_name + "', 'new_traj.txt')")
		plpy.execute("SELECT HDatasetsOfflineStatistics('" + new_dataset_name + "')")
		plpy.execute("CREATE INDEX ON " + new_dataset_name + "_seg USING gist (seg) WITH (FILLFACTOR = 100)")

	if not csv_file:
		os.remove('new_traj.txt')
		
	return 1
$$ LANGUAGE plpython3u;	
-------------------------Tests--------------------------------------------------------------

--SELECT trajectory_transformation_dec_sr('lol', 0.5, True, 'lllllll', True);
--SELECT trajectory_transformation_inc_sr('lol', 0.5, True, 'ddddddddd', True);
--SELECT trajectory_transformation_time_sr('lol','2008-12-31 19:29:30', '2008-12-31 19:29:42', 20, True, 'aaaaanew', True); 
--SELECT trajectory_transformation_add_noise('interval', 0.5, 5000000, True, 'noisy', True);
SELECT trajectory_transformation_synced_shift('gmaps', 1, 1000, True, 'synced', True);

