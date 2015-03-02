#ifndef SHAPE_H
#define SHAPE_H

#include "Color.h"
#include "Transform.h"
#include "Ray.h"
#include "Inlines.h"

class Shape
{
public:
	Shape(const Transform *w2o, const Colors &color, const bool &reflect);
	virtual ~Shape() {};

	virtual bool Intersect(const Ray &ray, float * tHit, float *rayEpsilon, Ray * rRay, Normal * normal) const = 0;
	virtual bool CanIntersect(const Ray &ray) const = 0;
	virtual BoundingBox GetBBox() const = 0;
	virtual Colors GetColor() const;

	const Transform WorldToObject, ObjectToWorld;
	bool Reflective;
	Colors Color;
	int ShapeID;
	static int currShapeID;
};


#endif
