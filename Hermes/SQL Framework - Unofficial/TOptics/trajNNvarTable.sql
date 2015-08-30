/*
 * Authors: Kiriakos Velissariou (kir.velissariou@gmail.com)
 */

 /*
  * This script creates the temporary table TOptics_Traj_NN_vars table
  * which is used to stored variables essential for some distance functions
 */
CREATE OR REPLACE FUNCTION he_TOptics_Traj_create_var_table() RETURNS void AS $$
BEGIN
	DROP TABLE IF EXISTS TOptics_Traj_NN_vars CASCADE;

	CREATE TEMPORARY TABLE TOptics_Traj_NN_vars (
	key text NOT NULL,
	value text NOT NULL,
	PRIMARY KEY (key));
END;
$$ LANGUAGE plpgsql STRICT;

