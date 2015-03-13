#include "Shape.h"
#include "Transform.h"

int Shape::currShapeID = 1;

Shape::Shape(const Transform *o2w, const Material &material)
	: Primitive(0), ObjectToWorld(*o2w), WorldToObject(Inverse(*o2w))
{
	Mat = material;
	ShapeID = currShapeID;
	++currShapeID;
}

Material Shape::GetMaterial() const
{
	return Mat;
}

