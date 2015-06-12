# Clustering  {#clustering}

According to @cite wiki:cluster_analysis clustering is the task of grouping a set of objects in such way that objects in the same group are more similar to each other than to those in other groups.

# TRACLUS algorithm # {#traclus}

TRACLUS algorithm was introduced by @cite lee2007trajectory in an effort to cluster similar trajectories. The main difference from the rest algorithms that cluster trajectories is that it partitions a trajectory into a set of line segments, and then, groups similar line segments together into a cluster and the primary advantage of this algorithm is that is able to discover common sub-trajectories from trajectory database.

The algorithm works on two phases. First it partitions the trajectories using the minimum description length principle and then it groups them using a density-based line-segment clustering algorithm.

An example using the TRACLUS algorithm with Hermes is shown below

    SELECT HLoader('imis', 'imis3days'); 

\see http://hanj.cs.illinois.edu/pdf/sigmod07_jglee.pdf