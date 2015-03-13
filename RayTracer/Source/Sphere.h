#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"
#include "Point.h"
#include "Ray.h"

class Sphere : public Shape
{
public:
	Sphere(const Transform *o2w, const Material &material, const float r);
	virtual ~Sphere() {};
	virtual bool Intersect(const Ray &ray, Hit *hit) const;
	virtual bool CanIntersect(const Ray &ray) const;
	virtual BoundingBox GetBBox() const;
	virtual Material GetMaterial() const;
	float Radius;
};

#endif
