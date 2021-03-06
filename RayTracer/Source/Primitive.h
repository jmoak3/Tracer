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
	Primitive(const int type, const Transform *o2w, const Transform *w2o) 
		:ObjectToWorld(o2w), WorldToObject(w2o)
	{Type = type; ShapeID = currShapeID; ++currShapeID;};
	virtual ~Primitive() {};

	virtual bool Intersect(const Ray &ray, Hit *hit) const = 0;
	virtual bool CanIntersect() const = 0;
	virtual BoundingBox ObjectBound() const = 0;
	virtual BoundingBox WorldBound() const = 0;
	virtual Material GetMaterial() const = 0;
	const Transform *WorldToObject, *ObjectToWorld;
	int Type;
	int ShapeID;
	static int currShapeID;
};

#endif
