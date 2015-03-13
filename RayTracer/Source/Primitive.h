#ifndef PRIMITIVE_H	
#define PRIMITIVE_H

#include "Point.h"
#include "Color.h"
#include "Transform.h"
#include "BoundingBox.h"

//TYPE
//0 -> Shape
//1 -> Light

class Primitive
{
public:
	Primitive(const int type) {Type = type;};
	virtual ~Primitive() {};

	virtual bool Intersect(const Ray &ray, Hit *hit) const = 0;
	virtual bool CanIntersect(const Ray &ray) const = 0;
	virtual BoundingBox GetBBox() const = 0;
	virtual Material GetMaterial() const = 0;
	int Type;
};

#endif
