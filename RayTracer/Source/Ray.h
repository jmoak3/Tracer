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
	float eps = 0.f;
	Material material;
	int shapeID = -1;
	int type = -1;
};


class Ray
{
public:
	Ray();
	Ray(const Point &oo, const Vector &dd, float start, float end = INFINITY, float t = 0.f, int dep = 0, float rIndex = 1.f);
	Ray(const Point &oo, const Vector &dd, const Ray &parent, float start, float end);
	~Ray() {};

	void UpdateInverse();
	Point o;
	Vector d;
	float refrIndex;

	mutable float mint, maxt;
	float time;
	int sign[3];
	int depth;
	Vector invd;
};

#endif