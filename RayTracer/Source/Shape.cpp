#include "Shape.h"
#include "Transform.h"

int Shape::currShapeID = 1;

Shape::Shape(const Transform *w2o, const Colors &color, const bool &reflect)
	: WorldToObject(*w2o), ObjectToWorld(Inverse(*w2o))
{
	Color = color;
	Reflective = reflect;
	ShapeID = currShapeID;
	++currShapeID;
}

Colors Shape::GetColor() const
{
	return Color;
}

