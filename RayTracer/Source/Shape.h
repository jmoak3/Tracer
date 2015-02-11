#ifndef SHAPE_H
#define SHAPE_H

#include "Ray.h"
#include "Inlines.h"

class Shape
{
public:
	Shape() {};
	virtual ~Shape() {};
	virtual bool CanIntersect() = 0;
	virtual bool Intersect(const Ray &ray, float * tHit, float *rayEpsilon) const = 0;
	virtual bool IntersectP(const Ray &ray) const = 0;
	virtual RGB GetColor() = 0;
};

#endif
