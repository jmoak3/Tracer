#ifndef RAY_H
#define RAY_H

class Vector;
class Point;
#include "Color.h"
#include <math.h>
#include "Point.h"
#include "Vector.h"

struct Hit
{
	Normal normal = Normal(1.f, 0.f, 0.f);
	float tHit = INFINITY;
	float eps;
	Material material;
	int shapeID = -1;
};


class Ray
{
public:
	Ray();
	Ray(const Point &oo, const Vector &dd, float start, float end = INFINITY, float t = 0.f, int dep = 0);
	Ray(const Point &oo, const Vector &dd, const Ray &parent, float start, float end);
	~Ray() {};

	Point o;
	Vector d;

	mutable float mint, maxt;
	float time;
	int depth;
};

#endif