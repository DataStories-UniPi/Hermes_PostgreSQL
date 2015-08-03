/**
 * @file
 * @author Giannis Kostis (jkostis@gmail.com)
 * @brief The implementantion of TRACLUS algorithm.
 *
 * @details Similarity functions between trajectories. For more detailed descritpion look at
 * @ref similarity
 *
 * @see @ref similarity
 * @see Trajectory/Similarity.sql
 *
 */

#ifndef HE_TRAJECTORY_SIMILARITY_H_
#define	HE_TRAJECTORY_SIMILARITY_H_

#include "Definition.h"

/** @brief The function returns the euclidean distance between the first points
 * 	of the trajectories
 *
 *  @param[in] trajectoryA The first trajectory
 *	@param[in] trajectoryB The second trajectory
 *	@param[out] result The result
 *
 */
float8 *EuclideanStart(const Trajectory *trajectoryA, const Trajectory *trajectoryB, float8 *result);
PGDLLEXPORT Datum EuclideanStartV1(PG_FUNCTION_ARGS);

/** @brief The function returns the euclidean distance between the last points
 * 	of the trajectories
 *
 *  @param[in] trajectoryA The first trajectory
 *	@param[in] trajectoryB The second trajectory
 *	@param[out] result The result
 *
 */
float8 *EuclideanEnd(const Trajectory *trajectoryA, const Trajectory *trajectoryB, float8 *result);
PGDLLEXPORT Datum EuclideanEndV1(PG_FUNCTION_ARGS);

/** @brief The function returns the average euclidean distance between the
 * 	first and last points of the trajectories
 *
 *  @param[in] trajectoryA The first trajectory
 *	@param[in] trajectoryB The second trajectory
 *	@param[out] result The result
 *
 */
float8 *EuclideanStartEnd(const Trajectory *trajectoryA, const Trajectory *trajectoryB, float8 *result);
PGDLLEXPORT Datum EuclideanStartEndV1(PG_FUNCTION_ARGS);

/** @brief The function returns the Manhattan norm between the trajectories
 *
 *  @param[in] trajectoryA The first trajectory
 *	@param[in] trajectoryB The second trajectory
 *	@param[in] precision The wanted precision
 *	@param[out] result The result
 *
 */
float8 *Manhattan(const Trajectory *trajectoryA, const Trajectory *trajectoryB, int32 precision, float8 *result);
PGDLLEXPORT Datum ManhattanV1(PG_FUNCTION_ARGS);

/** @brief The function returns the euclidean norm between the trajectories
 *
 *  @param[in] trajectoryA The first trajectory
 *	@param[in] trajectoryB The second trajectory
 *	@param[in] precision The wanted precision
 *	@param[out] result The result
 *
 */
float8 *Euclidean(const Trajectory *trajectoryA, const Trajectory *trajectoryB, int32 precision, float8 *result);
PGDLLEXPORT Datum EuclideanV1(PG_FUNCTION_ARGS);

/** @brief The function returns the maximum norm between the trajectories
 *
 *  @param[in] trajectoryA The first trajectory
 *	@param[in] trajectoryB The second trajectory
 *	@param[in] precision The wanted precision
 *	@param[out] result The result
 *
 */
float8 *Chebyshev(const Trajectory *trajectoryA, const Trajectory *trajectoryB, int32 precision, float8 *result);
PGDLLEXPORT Datum ChebyshevV1(PG_FUNCTION_ARGS);


/** @brief The function returns DSSIM
 *
 *  @param[in] trajectoryA The first trajectory
 *	@param[in] trajectoryB The second trajectory
 *	@param[in] precision The wanted precision
 *	@param[out] result The result
 *	@param[out] error The error
 *
 */
//DISSIMilarity between trajectories
float8 *DISSIM(const Trajectory *trajectoryA, const Trajectory *trajectoryB, int32 precision, float8 *result, float8 *error);
PGDLLEXPORT Datum DISSIMExact(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum DISSIMApproximate(PG_FUNCTION_ARGS);

/** @brief The function returns the Dynamic Time Warping
 *
 *  @param[in] trajectoryA The first trajectory
 *	@param[in] trajectoryB The second trajectory
 *	@param[in] w the locality constraint
 *	@param[in] w_percentage if w < 0 then someone can use percentage of the
 *				the trajectory by defining this value
 *	@param[in] lp the norm to be used (currently only 1 and 2)
 *	@param[out] result The result
 *
 */
float8 *DTW(const Trajectory *trajectoryA, const Trajectory *trajectoryB, int32 w, float8 w_percentage, int32 lp, float8 *result);
PGDLLEXPORT Datum DTWV1(PG_FUNCTION_ARGS);

/** @brief The function returns the Longest Common Subsequence
 *
 *  @param[in] trajectoryA The first trajectory
 *	@param[in] trajectoryB The second trajectory
 *	@param[in] w the locality constraint
 *	@param[in] w_percentage if w < 0 then someone can use percentage of the
 *				the trajectory by defining this value
 *	@param[in] eps the threshold, which determines whether or not two elements match.
 *				if eps<=0 or eps>=1 then eps = min(stedev(trajectories))
 *	@param[out] result The result
 *
 */
float8 *LCSS(const Trajectory *trajectoryA, const Trajectory *trajectoryB, int32 w, float8 w_percentage, float8 eps, float8 *result);
PGDLLEXPORT Datum LCSSV1(PG_FUNCTION_ARGS);


/** @brief The function returns the EDR
 *
 *  @param[in] trajectoryA The first trajectory
 *	@param[in] trajectoryB The second trajectory
 *	@param[in] eps the threshold, which determines whether or not two elements match.
 *	@param[out] result The result
 *
 */
float8 *EDR(const Trajectory *trajectoryA, const Trajectory *trajectoryB, float8 eps, float8 *result);
PGDLLEXPORT Datum EDRV1(PG_FUNCTION_ARGS);


/** @brief The function returns the ERP
 *
 *  @param[in] trajectoryA The first trajectory
 *	@param[in] trajectoryB The second trajectory
 *	@param[in] lp the norm to be used (currently only 1 and 2)
 *	@param[out] result The result
 *
 */
float8 *ERP(const Trajectory *trajectoryA, const Trajectory *trajectoryB, int32 lp, float8 *result);
PGDLLEXPORT Datum ERPV1(PG_FUNCTION_ARGS);

#endif
