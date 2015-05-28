/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_BOXSP_DEFINITION_H_
#define	HE_BOXSP_DEFINITION_H_

#include "../PointSP/Definition.h"

/*
 * lh ___ h
 *   |   |
 *   |___|
 *  l     hl
 *
 * This is an axis-aligned box.
 */
typedef struct {
	PointSP l; //The bottom left point
	PointSP h; //The top right point
} BoxSP;

BoxSP *cloneBoxSP(const BoxSP *box, BoxSP *result);
PGDLLEXPORT Datum cloneBoxSPV1(PG_FUNCTION_ARGS);

/*
 *	constructorBoxSP()	-	Constructs a spatial box from x and y coordinates.
 *
 *	Input parameters
 *		lx	-	lowest x
 *		ly	-	lowest y
 *		hx	-	highest x
 *		hy	-	highest y
 *
 *	Output parameters
 *		result	-	The box to update.
 *		error	-	Error message, if any.
 *
 *	Return value
 *		Pointer to a BoxSP or null pointer if an error has occurred.
 *		You should check if the returned pointer is NULL before you deallocate it with pfree()!
 *
 *	Correct examples (best practices)
 *		1.	BoxSP box; constructorBoxSP(0,0, 1,1, &box, NULL);
 *		2.	char *error = NULL; BoxSP *box = constructorBoxSP(0,0, 1,1, NULL, &error);
 *
 *	Wrong examples (common mistakes)
 *		1.	BoxSP *box = constructorBoxSP(1,1, 0,0, NULL, NULL); pfree(box);
 */
BoxSP *constructorBoxSP(int32 lx, int32 ly, int32 hx, int32 hy, BoxSP *result, char **error);
PGDLLEXPORT Datum constructorBoxSPV1(PG_FUNCTION_ARGS);

/*
 *	constructorHighBoxSP()	-	Constructs a spatial box from the low and high points.
 *
 *	Input parameters
 *		l	-	lowest point
 *		h	-	highest point
 *
 *	Output parameters
 *		result	-	Same as constructorBoxSP().
 *		error	-	Same as constructorBoxSP().
 *
 *	Return value
 *		Same as constructorBoxSP().
 */
BoxSP *constructorHighBoxSP(const PointSP *l, const PointSP *h, BoxSP *result, char **error);
PGDLLEXPORT Datum constructorHighBoxSPV1(PG_FUNCTION_ARGS);

/*
 *	inBoxSP()	-	Constructs a spatial box from the text representation of the object.
 *
 *	Input parameters
 *		str	-	text representation
 *			The format is "((lx, ly), (hx, hy))".
 *
 *	Output parameters
 *		result	-	Same as constructorBoxSP().
 *		error	-	Same as constructorBoxSP().
 *
 *	Return value
 *		Same as constructorBoxSP().
 */
BoxSP *inBoxSP(const char *str, BoxSP *result, char **error);
PGDLLEXPORT Datum inBoxSPV1(PG_FUNCTION_ARGS);

/*
 *	outBoxSP()	-	Constructs the text representation of a spatial box.
 *
 *	Input parameters
 *		box	-	Pointer to a BoxSP object.
 *
 *	Return value
 *		The text representation of the box.
 */
char *outBoxSP(const BoxSP *box);
PGDLLEXPORT Datum outBoxSPV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum recvBoxSP(PG_FUNCTION_ARGS);

/*
 *	BoxSP2Bytea()	-	Gives the byte array of a spatial box.
 *
 *	Input parameters
 *		box	-	Pointer to a BoxSP object.
 *
 *	Return value
 *		The byte array.
 */
bytea *BoxSP2Bytea(const BoxSP *box);
PGDLLEXPORT Datum sendBoxSP(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum hashBoxSP(PG_FUNCTION_ARGS);

/*
 *	equalsBoxSP()	-	Checks if the two boxes are equal.
 *
 *	Input parameters
 *		boxA	-	Pointer to the first BoxSP object.
 *		boxB	-	Pointer to the second BoxSP object.
 *
 *	Return value
 *		True if they are equal. False otherwise.
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
