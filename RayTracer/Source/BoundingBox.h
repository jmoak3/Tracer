#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "Point.h"
#include "Ray.h"

class BoundingBox
{
public:
	BoundingBox(const Point& min, const Point& max);
	~BoundingBox();

	bool Intersect(const Ray& ray);

private:
	Point Min, Max;
};

#endif
