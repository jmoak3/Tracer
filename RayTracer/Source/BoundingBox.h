#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "Point.h"
#include "Ray.h"
#include <algorithm>

class BoundingBox
{
public:
	BoundingBox() {};
	BoundingBox(bool canInstersect) {Intersectable = canInstersect;};
	BoundingBox(const Point& p);
	BoundingBox(const Point& min, const Point& max);
	BoundingBox(const BoundingBox & box, bool canIntersect = true);
	~BoundingBox();

	Point GetCenter() const;
	bool CanIntersect() const;
	bool Intersect(const Ray& ray) const;
	bool Intersect(const Ray& ray, Hit * hit) const;
	bool Contains(const Point &p) const;
	Point Min, Max;
	bool Intersectable;
};

inline BoundingBox Union(const BoundingBox &b1, const BoundingBox &b2)
{
	BoundingBox newBB;
	newBB.Min.x = std::min(b1.Min.x, b2.Min.x);
	newBB.Min.y = std::min(b1.Min.y, b2.Min.y);
	newBB.Min.z = std::min(b1.Min.z, b2.Min.z);
	newBB.Max.x = std::max(b1.Max.x, b2.Max.x);
	newBB.Max.y = std::max(b1.Max.y, b2.Max.y);
	newBB.Max.z = std::max(b1.Max.z, b2.Max.z);
	return newBB;
}

#endif
