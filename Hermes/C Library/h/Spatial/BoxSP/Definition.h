/*
* @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the BoxSP data type.
 *
 * @see @ref BoxSP
 * @see @ref data_types_spatial
 */

#ifndef HE_BOXSP_DEFINITION_H_
#define	HE_BOXSP_DEFINITION_H_

#include "../PointSP/Definition.h"

/**
 * BoxSP is spatial data type and is comprised of
 * two PointSP(i,e) components where “l” is the low left
 * point and “h” is the high right point
 *
 * lh ___ h         @n
 *   |   |          @n
 *   |___|          @n
 *  l     hl        @n
 *
 * This is an axis-aligned box.
 *
 * @see @ref BoxSP/Casts.sql
 * @see @ref BoxSP/Definition.sql
 * @see @ref BoxSP/Distance.sql
 * @see @ref BoxSP/Interactions.sql
 * @see @ref BoxSP/Properties.sql
 * @see @ref BoxSP/RelationalOperators.sql
 */
typedef struct {
	PointSP l; /**< The bottom left point */
	PointSP h; /**<  The top right point */
} BoxSP;

BoxSP *cloneBoxSP(const BoxSP *box, BoxSP *result);
PGDLLEXPORT Datum cloneBoxSPV1(PG_FUNCTION_ARGS);

/** @brief Constructs a spatial box from x and y coordinates.
 *
 *  Correct examples (best practices)
 *		1.	BoxSP box; constructorBoxSP(0,0, 1,1, &box, NULL);
 *		2.	char *error = NULL; BoxSP *box = constructorBoxSP(0,0, 1,1, NULL, &error);
 *
 *	Wrong examples (common mistakes)
 *		1.	BoxSP *box = constructorBoxSP(1,1, 0,0, NULL, NULL); pfree(box);
 *
 *
 *  @memberof BoxSP
 *
 *	@param[in] lx lowest x
 *	@param[in] ly lowest y
 *	@param[in] hx highest x
 *	@param[in] hy highest y
 *	@param[out] result the box to update.
 *	@param[out] error error message, if any.
 *
 *	@return Pointer to a BoxSP or null pointer if an error has occurred. You should
 *	check if the returned pointer is NULL before you deallocate it with pfree()!
 *
 */
BoxSP *constructorBoxSP(int32 lx, int32 ly, int32 hx, int32 hy, BoxSP *result, char **error);
PGDLLEXPORT Datum constructorBoxSPV1(PG_FUNCTION_ARGS);

/** @brief Constructs a spatial box from the low and high points.
 *
 *  @memberof BoxSP
 *
 *	@param[in] l lowest point
 *	@param[in] h highest point
 *	@param[out] result same as constructorBoxSP().
 *	@param[out] error same as constructorBoxSP().
 *
 *	@return same as @ref constructorBoxSP().
 */
BoxSP *constructorHighBoxSP(const PointSP *l, const PointSP *h, BoxSP *result, char **error);
PGDLLEXPORT Datum constructorHighBoxSPV1(PG_FUNCTION_ARGS);

/** @brief Constructs a spatial box from the text representation of the object.
 *
 *	@memberof BoxSP
 *
 *	@param[in] str text representation and the format is "((lx, ly), (hx, hy))".
 *	@param[out] result same as constructorBoxSP().
 *	@param[out]	error same as constructorBoxSP().
 *
 *	@return same as constructorBoxSP().
 */
BoxSP *inBoxSP(const char *str, BoxSP *result, char **error);
PGDLLEXPORT Datum inBoxSPV1(PG_FUNCTION_ARGS);

/** @brief Constructs the text representation of a spatial box.
 *
 *	@memberof BoxSP
 *
 *	@param[in] box pointer to a BoxSP object.
 *
 *	@return the text representation of the box.
 */
char *outBoxSP(const BoxSP *box);
PGDLLEXPORT Datum outBoxSPV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum recvBoxSP(PG_FUNCTION_ARGS);

/** @brief Gives the byte array of a spatial box.
 *
 *	@memberof BoxSP
 *
 *	@param[in] box pointer to a BoxSP object.
 *
 *	@return The byte array.
 *
 */
bytea *BoxSP2Bytea(const BoxSP *box);
PGDLLEXPORT Datum sendBoxSP(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum hashBoxSP(PG_FUNCTION_ARGS);

/** @brief Checks if the two boxes are equal.
 *
 *	@memberof BoxSP
 *
 *	@param[in] boxA	pointer to the first BoxSP object.
 *	@param[in] boxB	pointer to the second BoxSP object.
 *
 *	@return True if they are equal. False otherwise.
 *
 */
bool equalsBoxSP(const BoxSP *boxA, const BoxSP *boxB);
PGDLLEXPORT Datum equalsBoxSPV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum getLBoxSP(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getLxBoxSP(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getLyBoxSP(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getHBoxSP(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getHxBoxSP(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getHyBoxSP(PG_FUNCTION_ARGS);

#endif
