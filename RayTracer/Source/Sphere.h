#ifndef SPHERE_H
#define SPHERE_H

#include "Inlines.h"
#include "Shape.h"
#include "Point.h"
#include "Ray.h"

class Sphere : public Shape
{
public:
	Sphere(Point &p, RGB &color, float r);
	bool CanIntersect();
	bool Intersect(const Ray &ray, float * tHit, float *rayEpsilon) const;
	bool IntersectP(const Ray &ray) const;
	RGB GetColor();

private:
	Point Position;
	float Radius;
	RGB Color;
};

#endif
