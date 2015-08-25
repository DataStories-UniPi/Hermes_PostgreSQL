/**
 * @file
 * @author Kiriakos Velissariou (kir.velissariou@gmail.com).
 * @brief The file implements a wrapper function to simplify the uploading of
 * a dataset
 *
 *
 */

/** @brief This function performs dataset uploading
 *
 *	@param[in] name Name of the dataset
 *	@param[in] description Short description of the dataset
 *	@param[in] file The csv file to be uploaded
 *
 *  @return Interger 1 to indicate succesfull completion
 *
 */
CREATE OR REPLACE FUNCTION load_dataset(name text, description text, file text, trajectory_storage boolean)
RETURNS integer
AS $$
    if trajectory_storage:
        plpy.execute("SELECT hdatasetsregister('" + name + "', '" + name + "', dataset_trajectory_storage:=TRUE, dataset_segment_storage:=TRUE)")
    else:
        plpy.execute("SELECT HLoader('" + name  + "', '" + description + "')")
    plpy.execute("SELECT HLoaderCSV_II('" + name +  "', '" + file + "')")
    plpy.execute("SELECT HDatasetsOfflineStatistics('" + name + "')")
    plpy.execute("CREATE INDEX ON " + name  + "_seg USING gist (seg) WITH (FILLFACTOR = 100)")
    return 1
$$ LANGUAGE plpython3u;
