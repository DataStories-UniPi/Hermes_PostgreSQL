/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_BOXST_DEFINITION_H_
#define	HE_BOXST_DEFINITION_H_

#include "../../Temporal/Period/Definition.h"
#include "../../Spatial/BoxSP/Definition.h"

typedef struct {
	Period t;
	BoxSP sp;
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
