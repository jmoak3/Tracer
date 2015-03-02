#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"
#include "Point.h"
#include "Ray.h"

class Sphere : public Shape
{
public:
	Sphere(const Transform *w2o, const Colors &color, const bool &reflect, const float r);
	~Sphere() {};
	bool Intersect(const Ray &ray, float * tHit, float *rayEpsilon, Ray * rRay, Normal * normal) const;
	bool CanIntersect(const Ray &ray) const;
	BoundingBox GetBBox() const;
	Colors GetColor() const;

private:
	float Radius;
};

#endif
