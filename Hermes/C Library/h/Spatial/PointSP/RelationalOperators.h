/*
* @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the PointSP data type.
 *
 * @see @ref PointSP
 * @see @ref data_types_spatial
 */

#ifndef HE_POINTSP_RELATIONAL_OPERATORS_H_
#define HE_POINTSP_RELATIONAL_OPERATORS_H_

#include "Definition.h"

bool northPointSP_PointSP(const PointSP *pointA, const PointSP *pointB);
PGDLLEXPORT Datum northPointSP_PointSPV1(PG_FUNCTION_ARGS);

bool southPointSP_PointSP(const PointSP *pointA, const PointSP *pointB);
PGDLLEXPORT Datum southPointSP_PointSPV1(PG_FUNCTION_ARGS);

bool eastPointSP_PointSP(const PointSP *pointA, const PointSP *pointB);
PGDLLEXPORT Datum eastPointSP_PointSPV1(PG_FUNCTION_ARGS);

bool westPointSP_PointSP(const PointSP *pointA, const PointSP *pointB);
PGDLLEXPORT Datum westPointSP_PointSPV1(PG_FUNCTION_ARGS);

#endif
