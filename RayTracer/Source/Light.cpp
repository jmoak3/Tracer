#include "Light.h"
#include "Shape.h"

Light::Light(const Transform *w2o, const RGB &color,const float power)
	: WorldToObject(*w2o), ObjectToWorld(Inverse(*w2o))
{
	Color = color;
	Power = power;
	LightID = Shape::currShapeID;
	++Shape::currShapeID;
}

RGB Light::GetColor() const
{
	return Color;
}

