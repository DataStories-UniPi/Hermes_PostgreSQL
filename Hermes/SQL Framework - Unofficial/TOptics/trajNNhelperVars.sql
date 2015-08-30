/*
 * Authors: Kiriakos Velissariou (kir.velissariou@gmail.com)
 */

 /*
  * These are some helper functions which help the user pass values to
  * the TOptics_Traj_NN_vars table. These values are necessary for some
  * distance functions to work.
 */

CREATE OR REPLACE FUNCTION he_TOptics_Traj_NN_DTW_Vars(wrap_window text, lp text) RETURNS void AS $$
BEGIN
    INSERT INTO TOptics_Traj_NN_vars(key, value) VALUES ('wrap_window', wrap_window);
    INSERT INTO TOptics_Traj_NN_vars(key, value) VALUES ('lp', lp); --1-Manhattan 2-Euclidean
END;
$$ LANGUAGE plpgsql STRICT;


CREATE OR REPLACE FUNCTION he_TOptics_Traj_NN_LCSS_Vars(max_time text, d_perc text, max_distance text) RETURNS void AS $$
BEGIN
    INSERT INTO TOptics_Traj_NN_vars(key, value) VALUES ('max_time', max_time); --seconds
    INSERT INTO TOptics_Traj_NN_vars(key, value) VALUES ('d_perc', d_perc); --double
    INSERT INTO TOptics_Traj_NN_vars(key, value) VALUES ('max_distance', max_distance); -- 0 <=float<=1
END;
$$ LANGUAGE plpgsql STRICT;


CREATE OR REPLACE FUNCTION he_TOptics_Traj_NN_EDR_Vars(e text) RETURNS void AS $$
BEGIN
    INSERT INTO TOptics_Traj_NN_vars(key, value) VALUES ('e', e); --float
END;
$$ LANGUAGE plpgsql STRICT;


CREATE OR REPLACE FUNCTION he_TOptics_Traj_NN_ERP_Vars(lp text) RETURNS void AS $$
BEGIN
    INSERT INTO TOptics_Traj_NN_vars(key, value) VALUES ('lp', lp); --integer
END;
$$ LANGUAGE plpgsql STRICT;
