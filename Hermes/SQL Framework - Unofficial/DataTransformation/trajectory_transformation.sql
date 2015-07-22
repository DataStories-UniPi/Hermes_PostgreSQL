CREATE OR REPLACE FUNCTION trajectory_transformation (dataset text, transf_method text, rate float DEFAULT 0.1, distance float DEFAULT 0.0, save boolean DEFAULT True, csv_file boolean DEFAULT True)
  RETURNS integer
 AS $$
 $$ LANGUAGE plpython3u