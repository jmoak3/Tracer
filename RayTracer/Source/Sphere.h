#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"
#include "Point.h"
#include "Ray.h"

class Sphere : public Shape
{
public:
	Sphere(const Transform *w2o, const Material &material, const float r);
	~Sphere() {};
	bool Intersect(const Ray &ray, Hit *hit) const;
	bool CanIntersect(const Ray &ray) const;
	BoundingBox GetBBox() const;
	Material GetMaterial() const;

private:
	float Radius;
};

#endif
