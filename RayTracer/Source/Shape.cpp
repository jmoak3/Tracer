#include "Shape.h"
#include "Transform.h"

int Shape::currShapeID = 1;

Shape::Shape(const Transform *o2w, const Transform *w2o, const Material &material)
	: Primitive(0), ObjectToWorld(o2w), WorldToObject(w2o)
{
	Mat = material;
	ShapeID = currShapeID;
	++currShapeID;
}

bool Shape::Intersect(const Ray &ray, Hit * hit) const
{
	return false;
}

bool Shape::CanIntersect() const
{
	return true;
}

BoundingBox Shape::ObjectBound() const
{
	return BoundingBox();
}

BoundingBox Shape::WorldBound() const
{
	return BoundingBox();
}

Material Shape::GetMaterial() const
{
	return Mat;
}

