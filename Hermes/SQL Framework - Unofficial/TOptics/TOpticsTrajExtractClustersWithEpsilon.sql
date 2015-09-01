/**
 * @file
 * @author Kiriakos Velissariou (kir.velissariou@gmail.com).
 * @brief The function extract clusters from a table ordered by toptics
 * algorithm
 */
CREATE OR REPLACE FUNCTION TOpticsExtractClusters(epsilon integer,
    minpts integer) RETURNS void AS $$
DECLARE
    cluster_counter integer;
    trajectory record;
BEGIN
    DROP TABLE IF EXISTS TOptics_Traj_clustering CASCADE;
    CREATE TABLE TOptics_Traj_clustering (
		object text NOT NULL,
		cluster text NOT NULL,
		PRIMARY KEY (object)
	);
    cluster_counter := 0;
    FOR trajectory IN SELECT * FROM TOptics_Traj_ordering LOOP
        IF trajectory.reachability_distance IS NULL OR
            trajectory.reachability_distance > epsilon THEN
            IF trajectory.core_distance <= epsilon THEN
                cluster_counter := cluster_counter + 1;
			    INSERT INTO TOptics_Traj_clustering(object, cluster)
                    VALUES (trajectory.obj_id, cluster_counter);
            ELSE
			    INSERT INTO TOptics_Traj_clustering(object, cluster)
                    VALUES (trajectory.obj_id, 'NOISE');
            END IF;
        ELSE 
            INSERT INTO TOptics_Traj_clustering(object, cluster)
                VALUES (trajectory.obj_id, cluster_counter);
        END IF;
    END LOOP;
END;
$$ LANGUAGE plpgsql STRICT;
