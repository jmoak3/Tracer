#include "Sphere.h"
#include "Point.h"
#include "Ray.h"
#include "Inlines.h"

Sphere::Sphere(Point &p, RGB &color, float r)
{
	Position = p;
	Color = color;
	Radius = r;
}

bool Sphere::CanIntersect()
{
	return true;
}

bool Sphere::Intersect(const Ray &ray, float *tHit, float *rayEpsilon) const
{
	Vector sphereToOrigin = Vector(Position, Point(0.f, 0.f, 0.f));
	Ray r = Ray(ray);
	r.o -= sphereToOrigin;
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

bool Sphere::IntersectP(const Ray &ray) const
{
	//Here we'd hit a BB or something
	return true;
}

RGB Sphere::GetColor()
{
	return Color;
}