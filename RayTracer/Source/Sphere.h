#ifndef SPHERE_H
#define SPHERE_H

#include "Inlines.h"
#include "Shape.h"
#include "Point.h"
#include "Ray.h"

class Sphere : public Shape
{
public:
	Sphere(const Transform *w2o, const Transform *o2w, const RGB &color, const bool &reflect, const float r);
	bool Intersect(const Ray &ray, float * tHit, float *rayEpsilon) const;
	bool CanIntersect(const Ray &ray) const;
	BoundingBox GetBBox() const;
	RGB GetColor() const;

private:
	float Radius;
};

#endif
