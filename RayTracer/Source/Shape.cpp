#include "Shape.h"

Shape(const Transform *w2o, const Transform *o2w, const RGB &color, const bool &reflect)
	: WorldToObject(w2o), ObjectToWorld(o2w)
{
	Position = p;
	Color = color;
	Reflective = reflect;
}

RGB Shape::GetColor() const
{
	return Color;
}