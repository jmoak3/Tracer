#include "Ray.h"

Ray::Ray()
{
	o = Point(0.f, 0.f, 0.f);
	d = Vector(0.f, 0.f, 0.f);
	mint = 0.f;
	maxt = INFINITY;
	time = 0.f;
	depth = 0;
}

Ray::Ray(const Point &oo, const Vector &dd, float start, float end = INFINITY, float t = 0.f, float dep = 0)
{
	o = oo;
	d = dd;
	mint = start;
	maxt = end;
	time = t;
	depth = dep;
}

Ray::Ray(const Point &oo, const Vector &dd, const Ray &parent, float start, float end = INFINITY)
{
	o = oo;
	d = dd;
	mint = start;
	maxt = end;
	time = parent.time;
	depth = parent.depth;
}
