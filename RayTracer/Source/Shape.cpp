#include "Shape.h"
#include "Transform.h"


Shape::Shape(const Transform *o2w, const Transform *w2o, const Material &material)
	: Primitive(0, o2w, w2o)
{
	Mat = material;
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

