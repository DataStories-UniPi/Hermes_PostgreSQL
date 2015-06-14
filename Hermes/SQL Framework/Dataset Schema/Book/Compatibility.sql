/**
 * @author Marios Vodas (mvodas@gmail.com).
 * 
 * UNFORTUNATELLY THE DOXYGEN doesn`t support yet sql. So the documentation
 * of this file is on file doc/dataset_loading.md and should always be on sync with
 * this file !!!!
 */

/******************************************************************************/
CREATE FUNCTION HLoader(dataset_name text, dataset_long_name text) RETURNS void AS $$
BEGIN
	PERFORM HDatasetsRegister(dataset_name, dataset_long_name, dataset_segment_storage := true);
END;
$$ LANGUAGE plpgsql STRICT;

CREATE FUNCTION HLoaderCSV_II(dataset_name text, csv_file text) RETURNS void AS $$
BEGIN
	PERFORM HDatasetsLoad(dataset_name, csv_file);
END;
$$ LANGUAGE plpgsql STRICT;

CREATE FUNCTION HDatasetsOfflineStatistics(dataset_name text) RETURNS void AS $$
BEGIN
	PERFORM HDatasetsStatistics(dataset_name);
END;
$$ LANGUAGE plpgsql STRICT;

/******************************************************************************/
CREATE FUNCTION nm2meters(nm double precision) RETURNS double precision AS
	'$libdir/Hermes','nm2metresV1'
LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/
CREATE VIEW HDatasets_Online_Statistics AS
SELECT dataset, tmin, tmax, lx, ly, hx, hy
FROM HDatasets_Statistics;
