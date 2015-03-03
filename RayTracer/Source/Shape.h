#ifndef SHAPE_H
#define SHAPE_H

#include "Color.h"
#include "Transform.h"
#include "Ray.h"
#include "Inlines.h"

class Shape
{
public:
	Shape(const Transform *w2o, const Material &material);
	virtual ~Shape() {};

	virtual bool Intersect(const Ray &ray, Hit *hit) const = 0;
	virtual bool CanIntersect(const Ray &ray) const = 0;
	virtual BoundingBox GetBBox() const = 0;
	virtual Material GetMaterial() const;

	const Transform WorldToObject, ObjectToWorld;
	bool Reflective;
	Material Mat;
	int ShapeID;
	static int currShapeID;
};


#endif
