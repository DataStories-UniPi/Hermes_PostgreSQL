/*
 * Authors: Giannis Kostis (jkostis@gmail.com).
 */

#ifndef HE_TRAJECTORY_SIMILARITY_H_
#define	HE_TRAJECTORY_SIMILARITY_H_

#include "Definition.h"

//Start only
float8 *EuclideanStart(const Trajectory *trajectoryA, const Trajectory *trajectoryB, float8 *result);
PGDLLEXPORT Datum EuclideanStartV1(PG_FUNCTION_ARGS);

//End only
float8 *EuclideanEnd(const Trajectory *trajectoryA, const Trajectory *trajectoryB, float8 *result);
PGDLLEXPORT Datum EuclideanEndV1(PG_FUNCTION_ARGS);

//Start and End
float8 *EuclideanStartEnd(const Trajectory *trajectoryA, const Trajectory *trajectoryB, float8 *result);
PGDLLEXPORT Datum EuclideanStartEndV1(PG_FUNCTION_ARGS);

//L1
float8 *Manhattan(const Trajectory *trajectoryA, const Trajectory *trajectoryB, int32 precision, float8 *result);
PGDLLEXPORT Datum ManhattanV1(PG_FUNCTION_ARGS);

//L2
float8 *Euclidean(const Trajectory *trajectoryA, const Trajectory *trajectoryB, int32 precision, float8 *result);
PGDLLEXPORT Datum EuclideanV1(PG_FUNCTION_ARGS);

//LInf
float8 *Chebyshev(const Trajectory *trajectoryA, const Trajectory *trajectoryB, int32 precision, float8 *result);
PGDLLEXPORT Datum ChebyshevV1(PG_FUNCTION_ARGS);

//DISSIMilarity between trajectories
float8 *DISSIM(const Trajectory *trajectoryA, const Trajectory *trajectoryB, int32 precision, float8 *result, float8 *error);
PGDLLEXPORT Datum DISSIMExact(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum DISSIMApproximate(PG_FUNCTION_ARGS);

//Dynamic Time Warping
float8 *DTW(const Trajectory *trajectoryA, const Trajectory *trajectoryB, int32 w, float8 w_percentage, int32 lp, float8 *result);
PGDLLEXPORT Datum DTWV1(PG_FUNCTION_ARGS);

//Longest Common Subsequence
float8 *LCSS(const Trajectory *trajectoryA, const Trajectory *trajectoryB, int32 w, float8 w_percentage, float8 eps, float8 *result);
PGDLLEXPORT Datum LCSSV1(PG_FUNCTION_ARGS);

//Edit Distance on Real Sequences
float8 *EDR(const Trajectory *trajectoryA, const Trajectory *trajectoryB, float8 eps, float8 *result);
PGDLLEXPORT Datum EDRV1(PG_FUNCTION_ARGS);

//Edit Distance with Real Penalty
float8 *ERP(const Trajectory *trajectoryA, const Trajectory *trajectoryB, int32 lp, float8 *result);
PGDLLEXPORT Datum ERPV1(PG_FUNCTION_ARGS);

#endif
