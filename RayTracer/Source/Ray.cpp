#include "Ray.h"
#include "Point.h"
#include "Vector.h"
#include "Color.h"

Ray::Ray()
{
	o = Point(0.f, 0.f, 0.f);
	d = Vector(0.f, 0.f, 0.f);
	mint = 5e-4;
	maxt = INFINITY;
	time = 0.f;
	depth = 0;
	refrIndex = 0.f;
}

Ray::Ray(const Point &oo, const Vector &dd, float start, float end, float t, int dep, float rIndex)
{
	o = oo;
	d = dd;
	mint = start;
	maxt = end;
	time = t;
	depth = dep;
	refrIndex = rIndex;
}

Ray::Ray(const Point &oo, const Vector &dd, const Ray &parent, float start, float end = INFINITY)
{
	o = oo;
	d = dd;
	mint = start;
	maxt = end;
	time = parent.time;
	depth = parent.depth;
	refrIndex = 0.f;
}
