/*
* @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the LineSP data type.
 *
 * @see @ref LineSP
 * @see @ref data_types_spatial
 */

#ifndef HE_LINESP_RELATIONAL_OPERATORS_H_
#define HE_LINESP_RELATIONAL_OPERATORS_H_

#include "Definition.h"

bool isParallelLineSP_LineSP(const LineSP *lineA, const LineSP *lineB);
PGDLLEXPORT Datum isParallelLineSP_LineSPV1(PG_FUNCTION_ARGS);

#endif
