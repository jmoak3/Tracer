#ifndef SHAPE_H
#define SHAPE_H

#include "Primitive.h"
#include "Color.h"
#include "Transform.h"
#include "Ray.h"
#include "Inlines.h"

class Shape : public Primitive
{
public:
	Shape(const Transform *o2w, const Material &material);
	virtual ~Shape() {};

	virtual bool Intersect(const Ray &ray, Hit *hit) const = 0;
	virtual bool CanIntersect(const Ray &ray) const = 0;
	virtual BoundingBox GetBBox() const = 0;
	virtual Material GetMaterial() const;

	const Transform WorldToObject, ObjectToWorld;
	Material Mat;
	int ShapeID;
	static int currShapeID;
};


#endif
