/*
* @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the LineSP data type.
 *
 * @see @ref LineSP
 * @see @ref data_types_spatial
 */

#ifndef HE_LINESP_ARITHMETIC_OPERATORS_H_
#define HE_LINESP_ARITHMETIC_OPERATORS_H_

#include "Definition.h"

float8 fxLineSP(const LineSP *line, float8 x);
float8 fyLineSP(const LineSP *line, float8 y);
PGDLLEXPORT Datum fxLineSPV1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum fyLineSPV1(PG_FUNCTION_ARGS);

#endif
