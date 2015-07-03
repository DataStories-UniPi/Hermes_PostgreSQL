/*
 * @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the BoxST data type.
 *
 * @see @ref BoxST
 * @see @ref data_types_spatio_temp
 */

#ifndef HE_BOXST_DEFINITION_H_
#define	HE_BOXST_DEFINITION_H_

#include "../../Temporal/Period/Definition.h"
#include "../../Spatial/BoxSP/Definition.h"

/**
 * BoxST is a spatio-temporal data type and is
 * compromised of a Period "t" and a BoxSP "sp"
 * Consider a BoxST as a cube in 3D space.
 *
 * @see @ref BoxST/Casts.sql
 * @see @ref BoxST/Definition.sql
 * @see @ref BoxST/Editors.sql
 * @see @ref BoxST/Interactions.sql
 * @see @ref BoxST/Properties.sql
 * @see @ref BoxST/PseudoDistance.sql
 * @see @ref BoxST/PseudoRelationalOperators.sql
 * @see @ref BoxST/RelationalOperators.sql
 * @see @ref BoxST/Indexing/GiST.sql
 *
 */
typedef struct {
	Period t; /**< period for which we search */
	BoxSP sp; /**< the area in which we want to
				   search at the particular time*/
} BoxST;

BoxST *cloneBoxST(const BoxST *box, BoxST *result);
PGDLLEXPORT Datum cloneBoxSTV1(PG_FUNCTION_ARGS);

BoxST *constructorBoxST(Timestamp ti, Timestamp te, int32 lx, int32 ly, int32 hx, int32 hy, BoxST *result, char **error);
PGDLLEXPORT Datum constructorBoxSTV1(PG_FUNCTION_ARGS);

BoxST *constructorHighBoxST(const Period *t, const BoxSP *sp, BoxST *result, char **error);
PGDLLEXPORT Datum constructorHighBoxSTV1(PG_FUNCTION_ARGS);

BoxST *inBoxST(const char *str, BoxST *result, char **error);
PGDLLEXPORT Datum inBoxSTV1(PG_FUNCTION_ARGS);

char *outBoxST(const BoxST *box);
PGDLLEXPORT Datum outBoxSTV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum recvBoxST(PG_FUNCTION_ARGS);

bytea *BoxST2Bytea(const BoxST *box);
PGDLLEXPORT Datum sendBoxST(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum hashBoxST(PG_FUNCTION_ARGS);

bool equalsBoxST(const BoxST *boxA, const BoxST *boxB);
PGDLLEXPORT Datum equalsBoxSTV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum getTBoxST(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getTiBoxST(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getTeBoxST(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getSpBoxST(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getLBoxST(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getLxBoxST(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getLyBoxST(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getHBoxST(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getHxBoxST(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getHyBoxST(PG_FUNCTION_ARGS);

#endif
