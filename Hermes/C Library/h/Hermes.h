/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HERMES_H_
#define	HERMES_H_

#include <postgres.h>
#include <fmgr.h>

PGDLLEXPORT Datum hermesLicense(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum hermesVersionMajor(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum hermesVersionMinor(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum hermesVersionPatch(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum hermesVersionString(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum hermesVersionCodename(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum hermesVersionReleaseDate(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum hermesDataTypes(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum hermesDataTypesAsXML(PG_FUNCTION_ARGS);

#endif
