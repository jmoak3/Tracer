#include "BoundingBox.h"
#include <utility>
#include <assert.h>
BoundingBox::BoundingBox(const Point& p)
{
	Min = p;
	Max = p;
	Intersectable = true;
}

BoundingBox::BoundingBox(const Point& min, const Point& max)
{
	Min = min;
	Max = max;
	Intersectable = true;
}

BoundingBox::BoundingBox(const BoundingBox & box, bool canIntersect)
{
	Min = box.Min;
	Max = box.Max;
	Intersectable = canIntersect;
}

BoundingBox::~BoundingBox()
{
}

Point BoundingBox::operator[](int i) const
{
	return (&Min)[i*12];
}

bool BoundingBox::CanIntersect() const
{
	return Intersectable;
}

Point BoundingBox::GetCenter() const
{
	Point center = Average(Min, Max);
	return center;
}

bool BoundingBox::Intersect(const Ray& ray) const
{
	float tmin, tmax, tymin, tymax, tzmin, tzmax;
	tmin =  ((&Min)[1*ray.sign[0]].x	   - ray.o.x) * ray.invd.x;
	tmax =  ((&Min)[1 - 1*ray.sign[0]].x   - ray.o.x) * ray.invd.x;
	tymin = ((&Min)[1*ray.sign[1]].y	   - ray.o.y) * ray.invd.y;
	tymax = ((&Min)[1-1*ray.sign[1]].y     - ray.o.y) * ray.invd.y;
	if ((tmin > tymax) || (tymin > tmax))
		return false;
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;
	tzmin = ((&Min)[1*ray.sign[2]].z - ray.o.z) * ray.invd.z;
	tzmax = ((&Min)[1-1*ray.sign[2]].z - ray.o.z) * ray.invd.z;
	if ((tmin > tzmax) || (tzmin > tmax))
		return false;
    if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;
    return ((tmax > ray.mint) && (tmin < ray.maxt));
}

bool BoundingBox::IntersectFast(const Ray& ray) const
{
	float t0 = ray.mint;
	float t1 = ray.maxt;
	for (int i = 0; i < 3; ++i)
	{
		float invRayDir = 1.f / ray.d[i];
		float tNear = (Min[i] - ray.o[i]) * invRayDir;
		float tFar = (Max[i] - ray.o[i]) * invRayDir;
		if (tNear > tFar)
			std::swap(tNear, tFar);
		t0 = tNear > t0 ? tNear : t0;
		t1 = tFar < t1 ? tFar : t1;
		if (t0 > t1) return false;
	}
	if (t0 < ray.mint || t1 > ray.maxt)
		return false;
	return true;
}

bool BoundingBox::Intersect(const Ray& ray, Hit * hit) const
{
	float t0 = ray.mint;
	float t1 = ray.maxt;
	for (int i = 0; i < 3; ++i)
	{
		float invRayDir = 1.f / ray.d[i];
		float tNear = (Min[i] - ray.o[i]) * invRayDir;
		float tFar = (Max[i] - ray.o[i]) * invRayDir;
		if (tNear > tFar)
			std::swap(tNear, tFar);
		t0 = tNear > t0 ? tNear : t0;
		t1 = tFar < t1 ? tFar : t1;
		if (t0 > t1) return false;
	}
	return true;
}

bool BoundingBox::Contains(const Point& p) const
{
	bool in =  p.x >= Min.x && p.x <= Max.x;
	in = in && p.y >= Min.y && p.y <= Max.y;
	in = in && p.z >= Min.z && p.z <= Max.z;
	return in;
}

bool BoundingBox::Touches(const BoundingBox &box) const
{
	return (((Max.z > box.Min.z) && (Min.z < box.Max.z)) &&
		    ((Max.x > box.Min.x) && (Min.x < box.Max.x)) &&
		    ((Max.y > box.Min.y) && (Min.y < box.Max.y)));
}