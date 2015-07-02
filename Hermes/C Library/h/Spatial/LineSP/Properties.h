/*
* @file
 * @author Marios Vodas (mvodas@gmail.com).
 * @brief File containing the implementation of the LineSP data type.
 *
 * @see @ref LineSP
 * @see @ref data_types_spatial
 */

#ifndef HE_LINESP_PROPERTIES_H_
#define HE_LINESP_PROPERTIES_H_

#include "Definition.h"

float8 angleXXLineSP(const LineSP *line);
PGDLLEXPORT Datum angleXXLineSPV1(PG_FUNCTION_ARGS);

float8 slopeLineSP(const LineSP *line);
PGDLLEXPORT Datum slopeLineSPV1(PG_FUNCTION_ARGS);

#endif
