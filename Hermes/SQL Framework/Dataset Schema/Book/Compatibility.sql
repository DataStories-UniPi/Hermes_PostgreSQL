/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief The file implements the some compatibility functions
 *
 * @see @ref methods
 *
 */


/** @brief This function created the necessary tables that will
 * 	host the dateset
 *
 *	@param[in] dataset_name the short name of the dataset
 *	@param[in] dataset_long_name the description fo the dataset
 *
 *  
 */
CREATE FUNCTION HLoader(dataset_name text, dataset_long_name text) RETURNS void AS $$
BEGIN
	PERFORM HDatasetsRegister(dataset_name, dataset_long_name, dataset_segment_storage := true);
END;
$$ LANGUAGE plpgsql STRICT;

/** @brief This function loads the data from a csv file.
 *	Before loading the data in PostgreSql, one needs to save 
 *	the data files in the PostgreSQL directory in a file type of 
 *	csv with a header for column names with the following 
 *	structure: <obj_id, traj_id, t, lon, lat>.
 *
 *	@param[in] dataset_name the name of the dataset
 *	@param[in] csv_file the file that contains the data
 *
 */
CREATE FUNCTION HLoaderCSV_II(dataset_name text, csv_file text) RETURNS void AS $$
BEGIN
	PERFORM HDatasetsLoad(dataset_name, csv_file);
END;
$$ LANGUAGE plpgsql STRICT;


/** @brief This function calculates some statistics for the dataset, 
 * 	such as average number of points per trajectory, average duration 
 * 	of trajectories, and average length of trajectories. 
 *
 *	@param[in] dataset_name the name of the dataset
 *
 */
CREATE FUNCTION HDatasetsOfflineStatistics(dataset_name text) RETURNS void AS $$
BEGIN
	PERFORM HDatasetsStatistics(dataset_name);
END;
$$ LANGUAGE plpgsql STRICT;

/** @brief This function transform nautical miles to meters
 *
 *	@param[in] nm the nautical miles
 *
 *  @return the meters
 *  
 * Normally this function should have been inside Utilities/UnitConcersion.sql
 * but I am too bored to test the consequences of moving this function.
 * 
 */
CREATE FUNCTION nm2meters(nm double precision) RETURNS double precision AS
	'$libdir/Hermes','nm2metresV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE VIEW HDatasets_Online_Statistics AS
SELECT dataset, tmin, tmax, lx, ly, hx, hy
FROM HDatasets_Statistics;
