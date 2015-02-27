#include "Sphere.h"
#include "Point.h"
#include "Ray.h"
#include "Inlines.h"

Sphere::Sphere(const Transform *w2o, const Transform *o2w, const RGB &color, const bool &reflect, const float r)
	: Shape(w2o, o2w, color, reflect)
{
	Radius = r;
}

bool Sphere::Intersect(const Ray &ray, float *tHit, float *rayEpsilon) const
{
	Ray r;
	WorldToObject(ray, &r);

	float A = r.d.x*r.d.x + r.d.y*r.d.y + r.d.z*r.d.z;
	float B = 2.f*(r.d.x*r.o.x + r.d.y*r.o.y + r.d.z*r.o.z);
	float C = r.o.x*r.o.x + r.o.y*r.o.y + r.o.z*r.o.z - Radius*Radius;

	float t0, t1 = 0.f;
	if (!Quadratic(A, B, C, &t0, &t1))
		return false;

	if (t0 > r.maxt || t1 < r.mint)
		return false;
	float thit = t0;
	if (t0 < r.mint)
	{
		thit = t1;
		if (thit > r.maxt)
			return false;
	}

	*tHit = thit;
	*rayEpsilon = 5e-4 * *tHit;
	return true;
}

bool Sphere::CanIntersect(const Ray &ray) const
{
	return GetBBox().Intersect(ray);
}

RGB Sphere::GetColor() const
{
	return Color;
}

BoundingBox Sphere::GetBBox() const
{
	return BoundingBox(Point(-Radius, -Radius, -Radius), Point(Radius, Radius, Radius));
}