
#ifndef _RAY_H_
#define _RAY_H_

#include "Point.h"
#include "Inlines.h"
#include "Vector.h"

class Ray
{
public:
	Ray();
	Ray(Point oo, Vector &dd);
	Ray(Point oo, Point p1, Point p2);
	~Ray() {};

	Point o;
	Vector d;

	mutable float mint, maxt;
	float time;
	int depth;
};

#endif