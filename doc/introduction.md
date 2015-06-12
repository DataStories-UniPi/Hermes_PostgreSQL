# Introduction {#instroduction}

Moving Object Database (MOD) engines enable us to process, manage and analyze mobility data @cite hermes_mod. Hermes is a robust MOD engine designed to efficiently process moving object trajectories through a data type system that unifies the spatial and temporal dimensions of moving object trajectories @cite pelekis2014mobility.

In general, the big picture of Hermes aiming for the efficient management and exploration of mobility data is illustrated in the following figure @cite hermes_mod.

![The general architecture of Hermes for efficient mobility data management and exploration.](hermes_figure1.png)
@image latex hermes_figure1.png "The general architecture of Hermes for efficient mobility data management and exploration" width=\textwidth

According to the above figure, the telecommunications network provides raw unprocessed data (e.g. GPS recordings), which, in turn, is processed and transformed in mobility data (e.g. trajectories of moving objects) stored in MOD systems (in their detailed form) and Data Warehouse systems (in aggregations over detailed information, respectively). The stored information can be queried as well as provide input for advanced analysis, such as multidimensional (OLAP) analysis and data mining, the output of which is appropriately visualized to end users @cite hermes_mod.






