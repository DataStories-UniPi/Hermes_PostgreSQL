/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#ifndef HE_AIS_H_
#define	HE_AIS_H_

#include <postgres.h>
#include <fmgr.h>
#include <utils/xml.h>

int32 computeAISChecksum(const char *ais);
PGDLLEXPORT Datum computeAISChecksumV1(PG_FUNCTION_ARGS);

bool isAuxiliaryMMSI(int32 mmsi);
PGDLLEXPORT Datum isAuxiliaryMMSIV1(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum decodeAISXML(PG_FUNCTION_ARGS);

PGDLLEXPORT Datum decodeAIS(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum decodeAISPositionReport(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum decodeAISStaticData(PG_FUNCTION_ARGS);

#endif
