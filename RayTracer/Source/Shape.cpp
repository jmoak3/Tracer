#include "Shape.h"
#include "Transform.h"

Shape::Shape(const Transform *w2o, const Transform *o2w, const RGB &color, const bool &reflect)
	: WorldToObject(*w2o), ObjectToWorld(*o2w)
{
	Color = color;
	Reflective = reflect;
}

RGB Shape::GetColor() const
{
	return Color;
}