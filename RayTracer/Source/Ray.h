
#ifndef _RAY_H_
#define _RAY_H_

#include "Point.h"
#include "Inlines.h"
#include "Vector.h"

class Ray
{
public:
	Ray();
	Ray(const Point &oo, const Vector &dd, float start, float end = INFINITY, float t = 0.f, float dep = 0);
	Ray(const Point &oo, const Vector &dd, const Ray &parent, float start, float end = INFINITY);
	~Ray() {};

	Point o;
	Vector d;

	mutable float mint, maxt;
	float time;
	int depth;
};

#endif