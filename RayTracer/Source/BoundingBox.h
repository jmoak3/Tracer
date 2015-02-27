#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "Point.h"
#include "Ray.h"
#include <algorithm>

class BoundingBox
{
public:
	BoundingBox() {};
	BoundingBox(const Point& p);
	BoundingBox(const Point& min, const Point& max);
	~BoundingBox();

	bool Intersect(const Ray& ray);

	Point Min, Max;
};

inline BoundingBox Union(const BoundingBox &b1, const BoundingBox &b2)
{
	BoundingBox newBB;
	newBB.Min.x = std::min(b1.Min.x, b2.Min.x);
	newBB.Min.y = std::min(b1.Min.x, b2.Min.x);
	newBB.Min.z = std::min(b1.Min.x, b2.Min.x);
	newBB.Max.x = std::max(b1.Max.x, b2.Max.x);
	newBB.Max.y = std::max(b1.Max.x, b2.Max.x);
	newBB.Max.z = std::max(b1.Max.x, b2.Max.x);
	return newBB;
}

#endif
