#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"
#include "Point.h"
#include "Ray.h"

class Sphere : public Shape
{
public:
	Sphere(const Transform *o2w, const Transform *w2o, const Material &material, float r);
	~Sphere() {};
	bool Intersect(const Ray &ray, Hit *hit) const;
	BoundingBox GetBBox() const;
	Material GetMaterial() const;
	float Radius;
};

#endif
