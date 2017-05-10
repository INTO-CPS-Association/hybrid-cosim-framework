/*
 * Math.h
 *
 *  Created on: 10 May 2017
 *      Author: kel
 */

#ifndef SEMANTIC_ADAPTATION_SRC_HYFMATH_H_
#define SEMANTIC_ADAPTATION_SRC_HYFMATH_H_

#include <algorithm>
#include <math.h>

namespace adaptation
{

class HyfMath
{
public:
	HyfMath();
	virtual ~HyfMath();

	static double relativeError(double a, double b)
	{
		if (a == 0)
		{
			return 0;
		}
		return fabs((a - b) / a);
	}
	static double absoluteError(double a, double b)
	{
		return fabs(a - b);
	}
	static int is_close(double a, double b, double REL_TOL, double ABS_TOL)
	{
		return ((absoluteError(a, b) < ABS_TOL) && (relativeError(a, b) < REL_TOL));
	}
};

} /* namespace adaptation */

#endif /* SEMANTIC_ADAPTATION_SRC_HYFMATH_H_ */
