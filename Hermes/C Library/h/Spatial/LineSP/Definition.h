/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_LINESP_DEFINITION_H_
#define	HE_LINESP_DEFINITION_H_

#include <postgres.h>
#include <fmgr.h>

/*
 * General Form of the line equation: A*x + B*y + C = 0
 */
typedef struct {
	float8 A;
	float8 B;
	float8 C;
} LineSP;

LineSP *cloneLineSP(const LineSP *line, LineSP *result);
PGDLLEXPORT Datum cloneLineSPV1(PG_FUNCTION_ARGS);

LineSP *constructorLineSP(float8 A, float8 B, float8 C, LineSP *result);
PGDLLEXPORT Datum constructorLineSPV1(PG_FUNCTION_ARGS);

LineSP *inLineSP(const char *str, LineSP *result, char **error);
PGDLLEXPORT Datum inLineSPV1(PG_FUNCTION_ARGS);

char *outLineSP(const LineSP *line);
PGDLLEXPORT Datum outLineSPV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum recvLineSP(PG_FUNCTION_ARGS);

bytea *LineSP2Bytea(const LineSP *line);
PGDLLEXPORT Datum sendLineSP(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum hashLineSP(PG_FUNCTION_ARGS);

bool equalsLineSP(const LineSP *lineA, const LineSP *lineB);
PGDLLEXPORT Datum equalsLineSPV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum getALineSP(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getBLineSP(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum getCLineSP(PG_FUNCTION_ARGS);

#endif
