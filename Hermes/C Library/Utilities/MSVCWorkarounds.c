/*
* Authors: Marios Vodas (mvodas@gmail.com).
*/

#include "../h/Utilities/MSVCWorkarounds.h"

#if defined(WIN32) && _MSC_VER < 1800

double round(double x) {
	return (x >= 0.0) ? floor(x + 0.5) : ceil(x - 0.5);
}

#endif
