#ifndef INLINES_H
#define INLINES_H


#include "Point.h"
#include "Vector.h"
#include "Normal.h"
#include "Quaternion.h"
#include "Camera.h"
#include <assert.h>
#include <math.h>
#include <utility>

inline bool Quadratic(float A, float B, float C, float *t0, float *t1)
{
	float discrim = (B*B - 4.f * A*C);
	if (discrim <= 0.f) return false;
	float rtDiscrim = sqrtf(discrim);

	float q;

	if (B < 0)
		q = -0.5f*(B - rtDiscrim);
	else
		q = -0.5f*(B + rtDiscrim);
	*t0 = q / A;
	*t1 = C / q;
	if (*t1 <= *t0) std::swap(*t0, *t1);
	return true;
}

#endif