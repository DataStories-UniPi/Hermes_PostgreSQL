/*
 * Authors: Panagiotis Tampakis (panosbp@gmail.com).
 */

CREATE OR REPLACE FUNCTION drop_ReTraTree(data_source character varying)
RETURNS void AS

$BODY$

declare

i integer;
j integer;
n_of_chunks integer;
n_of_subchunks integer;
 
 begin


	execute 'UPDATE '||data_source||'_seg SET insert_to_chunk = TRUE';
	execute 'SELECT substring(max(tablename) from (position(''_c_'' in max(tablename))+3) for (position(''_sc_'' in max(tablename))-position(''_c_'' in max(tablename))-3))::integer FROM pg_tables WHERE tablename LIKE '''||data_source||'_c_%_sc_%_outliers_seg''' into n_of_chunks;
	
	for i in 1..n_of_chunks loop
		
		execute 'SELECT substring(max(tablename) from (position(''_sc_'' in max(tablename))+4) for (position(''_outliers_'' in max(tablename))-position(''_sc_'' in max(tablename))-4))::integer FROM pg_tables WHERE tablename LIKE '''||data_source||'_c_'||i||'_sc_%_outliers_seg''' into n_of_subchunks;
		
		for j in 1..n_of_subchunks loop
		
			execute 'DROP TABLE IF EXISTS '||data_source||'_c_'||i||'_sc_'||j||'_outliers_seg CASCADE ';
			execute 'DROP TABLE IF EXISTS '||data_source||'_c_'||i||'_sc_'||j||' CASCADE ';
			execute 'DROP TABLE IF EXISTS '||data_source||'_c_'||i||'_sc_'||j||'_outliers_sample CASCADE ';
			execute 'DROP TABLE IF EXISTS '||data_source||'_c_'||i||'_sc_'||j||'_repr CASCADE ';
			execute 'DROP TABLE IF EXISTS '||data_source||'_c_'||i||'_sc_'||j||'_representatives CASCADE ';
			execute 'DROP TABLE IF EXISTS "'||data_source||'_c_'||i||'_sc_'||j||'_outliers_Hermes_stats" CASCADE ';
			execute 'DROP TABLE IF EXISTS '||data_source||'_c_'||i||'_sc_'||j||'_outliers_subtraj CASCADE ';
			execute 'DROP TABLE IF EXISTS '||data_source||'_c_'||i||'_sc_'||j||'_outliers_seg_voting CASCADE ';
			execute 'DROP TABLE IF EXISTS '||data_source||'_c_'||i||'_sc_'||j||'_outliers_seg_sample CASCADE ';
		
		end loop;
		
	end loop;

	execute 'DROP TABLE IF EXISTS '||data_source||'_sub_chunks_metadata CASCADE ';
	execute 'DROP TABLE IF EXISTS 	'||data_source||'_outliers_archive_seg'; 

	execute 'UPDATE '||data_source||'_seg SET subtraj_id = 1';

end
 
$BODY$

LANGUAGE plpgsql;

--select drop_ReTraTree('imis1week_subset_1_seg');