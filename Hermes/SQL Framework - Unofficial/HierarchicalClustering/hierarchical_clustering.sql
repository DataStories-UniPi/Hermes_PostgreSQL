/**
 * @file
 * @author Kiriakos Velissariou (kir.velissariou@gmail.com).
 * @brief The file implements hierarchical clustering
 *
 *
 */

CREATE OR REPLACE FUNCTION hierarchical_clustering(dataset text, metric text)
RETURNS integer
AS $$
    import numpy as np
    import scipy.cluster.hierarchy as hac
    import scipy.spatial.distance as spd
    from matplotlib.pyplot import show
$$ LANGUAGE plpython3u;
