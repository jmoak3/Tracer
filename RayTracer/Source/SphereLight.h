#ifndef SPHERELIGHT_H
#define SPHERELIGHT_H

#include "Light.h"

class SphereLight : public Light
{
public:
	SphereLight(const Transform *w2o, const RGB &color, const float power, const float r);
	~SphereLight() {};
	bool Intersect(const Ray &ray, Hit *hit) const;
	bool CanIntersect(const Ray &ray) const;
	BoundingBox GetBBox() const;
	RGB GetColor() const;

	float Radius;
};

#endif
