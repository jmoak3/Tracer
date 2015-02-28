#ifndef SHAPE_H
#define SHAPE_H

#include "Transform.h"

#include "Ray.h"
#include "Inlines.h"

class Shape
{
public:
	Shape(const Transform *w2o, const Transform *o2w, const RGB &color, const bool &reflect);
	virtual ~Shape() {};

	virtual bool Intersect(const Ray &ray, float * tHit, float *rayEpsilon, Ray * rRay, Normal * normal) const = 0;
	virtual bool CanIntersect(const Ray &ray) const = 0;
	virtual BoundingBox GetBBox() const = 0;
	virtual RGB GetColor() const;

	const Transform WorldToObject, ObjectToWorld;
	bool Reflective;
	RGB Color;
	
};

#endif
