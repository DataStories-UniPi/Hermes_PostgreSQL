/**
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the Trajectory data type.
 *
 * @see @ref Trajectory
 * @see @ref data_type_trajectory
 * @see @ref similarity
 * 
 */

/** @brief The function returns the euclidean distance between the first points
 * 	of the trajectories
 *
 *  @param[in] trajectoryA The first trajectory
 *	@param[in] trajectoryB The second trajectory
 *	@param[out] result The result
 *
 *	@return the distance between the first points of the trajectories
 *
 *	@see @ref similarity_euclidean
 *
 */
CREATE FUNCTION EuclideanStart(trajectoryA Trajectory, trajectoryB Trajectory) RETURNS double precision AS
	'$libdir/Hermes','EuclideanStartV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the euclidean distance between the last points
 * 	of the trajectories
 *
 *  @param[in] trajectoryA The first trajectory
 *	@param[in] trajectoryB The second trajectory
 *
 *	@return distance between the last points of the trajectories
 *
 *	@see @ref similarity_euclidean
 *
 */
CREATE FUNCTION EuclideanEnd(trajectoryA Trajectory, trajectoryB Trajectory) RETURNS double precision AS
	'$libdir/Hermes','EuclideanEndV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the average euclidean distance between the
 * 	first and last points of the trajectories
 *
 *  @param[in] trajectoryA The first trajectory
 *	@param[in] trajectoryB The second trajectory
 *
 *	@return the average euclidean distance between the
 * 	first and last points of the trajectories
 * 
 * 	@see @ref similarity_euclidean
 *
 */
CREATE FUNCTION EuclideanStartEnd(trajectoryA Trajectory, trajectoryB Trajectory) RETURNS double precision AS
	'$libdir/Hermes','EuclideanStartEndV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the Manhattan norm between the trajectories
 *
 *  @param[in] trajectoryA The first trajectory
 *	@param[in] trajectoryB The second trajectory
 *	@param[in] interpolationPrecision The interpolation precision
 *
 *	@return the Manhattan norm
 *
 *	@see @ref similarity_manhattan
 *
 */
CREATE FUNCTION Manhattan(trajectoryA Trajectory, trajectoryB Trajectory, interpolationPrecision integer DEFAULT HInterpolationPrecision()) RETURNS double precision AS
	'$libdir/Hermes','ManhattanV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the Euclidean norm between the trajectories
 *
 *  @param[in] trajectoryA The first trajectory
 *	@param[in] trajectoryB The second trajectory
 *	@param[in] interpolationPrecision The interpolation precision
 *
 *	@return the Euclidean norm
 *
 *	@see @ref similarity_euclidean
 *
 */
CREATE FUNCTION Euclidean(trajectoryA Trajectory, trajectoryB Trajectory, interpolationPrecision integer DEFAULT HInterpolationPrecision()) RETURNS double precision AS
	'$libdir/Hermes','EuclideanV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the Chebyshev norm between the trajectories
 *
 *  @param[in] trajectoryA The first trajectory
 *	@param[in] trajectoryB The second trajectory
 *	@param[in] interpolationPrecision The interpolation precision
 *
 *	@return the Chebyshev norm
 *
 *	@see @ref similarity_chebyshev
 *
 */
CREATE FUNCTION Chebyshev(trajectoryA Trajectory, trajectoryB Trajectory, interpolationPrecision integer DEFAULT HInterpolationPrecision()) RETURNS double precision AS
	'$libdir/Hermes','ChebyshevV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the exact DISSIM
 *
 *  @param[in] trajectoryA The first trajectory
 *	@param[in] trajectoryB The second trajectory
 *	@param[in] interpolationPrecision The interpolation precision
 *
 *	@return the exact DISSIM
 *
 *	@see @ref similarity_dissim
 *
 */
CREATE FUNCTION DISSIMExact(trajectoryA Trajectory, trajectoryB Trajectory, interpolationPrecision integer DEFAULT HInterpolationPrecision()) RETURNS double precision AS
	'$libdir/Hermes','DISSIMExact'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the aproximate DISSIM
 *
 *  @param[in] trajectoryA The first trajectory
 *	@param[in] trajectoryB The second trajectory
 *	@param[in] interpolationPrecision The interpolation precision
 *	@param[out] dissim The dissim precision
 *	@param[out] error The error 
 *
 *	@return the aproximate DISSIM
 *
 *	@see @ref similarity_dissim
 *
 */
CREATE FUNCTION DISSIMApproximate(trajectoryA Trajectory, trajectoryB Trajectory, interpolationPrecision integer DEFAULT HInterpolationPrecision(),
	OUT dissim double precision, OUT error double precision
) AS
	'$libdir/Hermes','DISSIMApproximate'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the DTW
 *
 *  @param[in] trajectoryA The first trajectory
 *	@param[in] trajectoryB The second trajectory
 *	@param[in] w the locality constraint
 *	@param[in] w_percentage if w < 0 then someone can use percentage of the 
 *				the trajectory by defining this value 
 *	@param[in] lp the norm to be used (currently only 1 and 2)
 *
 *	@return the DTW
 *
 *	@see @ref similarity_dtw
 *
 */
CREATE FUNCTION DTW(trajectoryA Trajectory, trajectoryB Trajectory, w integer DEFAULT -1, w_percentage double precision DEFAULT 1, lp integer DEFAULT 2) RETURNS double precision AS
	'$libdir/Hermes','DTWV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns the LCSS
 *
 *  @param[in] trajectoryA The first trajectory
 *	@param[in] trajectoryB The second trajectory
 *	@param[in] w the locality constraint
 *	@param[in] w_percentage if w < 0 then someone can use percentage of the 
 *				the trajectory by defining this value 
 *	@param[in] eps the threshold, which determines whether or not two elements match.
 *				if eps<=0 or eps>=1 then eps = min(stedev(trajectories))
 *
 *	@return the LCSS
 *
 *	@see @ref similarity_lcss
 *
 */
CREATE FUNCTION LCSS(trajectoryA Trajectory, trajectoryB Trajectory, w integer DEFAULT -1, w_percentage double precision DEFAULT 1, eps double precision DEFAULT 1) RETURNS double precision AS
	'$libdir/Hermes','LCSSV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns EDR
 *
 *  @param[in] trajectoryA The first trajectory
 *	@param[in] trajectoryB The second trajectory
 *	@param[in] eps the threshold, which determines whether or not two elements match.
 *
 *	@return the EDR
 *
 *	@see similarity_edr
 *
 */
CREATE FUNCTION EDR(trajectoryA Trajectory, trajectoryB Trajectory, eps double precision DEFAULT 1) RETURNS double precision AS
	'$libdir/Hermes','EDRV1'
LANGUAGE C IMMUTABLE STRICT;

/** @brief The function returns ERP
 *
 *  @param[in] trajectoryA The first trajectory
 *	@param[in] trajectoryB The second trajectory
 *	@param[in] lp the norm to be used (currently only 1 and 2)
 *
 *	@return the ERP
 *
 *	@see @ref similarity_erp
 *
 */
CREATE FUNCTION ERP(trajectoryA Trajectory, trajectoryB Trajectory, lp integer DEFAULT 2) RETURNS double precision AS
	'$libdir/Hermes','ERPV1'
LANGUAGE C IMMUTABLE STRICT;
