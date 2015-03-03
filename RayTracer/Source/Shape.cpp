#include "Shape.h"
#include "Transform.h"

int Shape::currShapeID = 1;

Shape::Shape(const Transform *w2o, const Material &material)
	: WorldToObject(*w2o), ObjectToWorld(Inverse(*w2o))
{
	Mat = material;
	ShapeID = currShapeID;
	++currShapeID;
}

Material Shape::GetMaterial() const
{
	return Mat;
}

