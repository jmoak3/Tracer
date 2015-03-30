#include "BoundingBox.h"
#include <utility>

BoundingBox::BoundingBox(const Point& p)
{
	Min = p;
	Max = p;
}

BoundingBox::BoundingBox(const Point& min, const Point& max)
{
	Min = min;
	Max = max;
}


BoundingBox::~BoundingBox()
{
}

Point BoundingBox::GetCenter() const
{
	Point center = Point((Max-Min)/2.f);
	return center;
}

bool BoundingBox::Intersect(const Ray& ray) const
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
	bool in = p.x > Min.x && p.x < Max.x;
	in = in && p.y > Min.y && p.y < Max.y;
	in = in && p.z > Min.z && p.z < Max.z;
	return true;
}