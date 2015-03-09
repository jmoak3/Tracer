#ifndef LIGHT_H	
#define LIGHT_H
#include "Point.h"
#include "Color.h"
#include "Transform.h"
#include "BoundingBox.h"

class Light
{
public:
	Light(const Transform *w2o, const RGB &material, const float power);
	virtual ~Light() {};

	virtual bool Intersect(const Ray &ray, Hit *hit) const = 0;
	virtual bool CanIntersect(const Ray &ray) const = 0;
	virtual BoundingBox GetBBox() const = 0;
	virtual RGB GetColor() const;

	const Transform WorldToObject, ObjectToWorld;
	RGB Color;
	float Power;
	int LightID;
};

#endif
