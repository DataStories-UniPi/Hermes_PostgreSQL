/*
* Authors: Marios Vodas (mvodas@gmail.com).
*/

#if !defined(HE_MSVCWORKAROUNDS_H_) && defined(WIN32) && _MSC_VER < 1800
#define HE_MSVCWORKAROUNDS_H_

#include <math.h>

#define INFINITY	(HUGE_VAL * HUGE_VAL)
#define NAN			(INFINITY - INFINITY)

double round(double x);

#endif
