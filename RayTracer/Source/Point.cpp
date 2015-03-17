#include "Point.h"
#include "Vector.h"
#include <math.h>
#include <assert.h>

Point::Point()
{
	x = y = z = 0.f;
}

Point::Point(float xx, float yy, float zz)
{
	x = xx; y = yy; z = zz;
	assert(!HasNans());
}

Point Point::operator+(const Vector &v) const
{
	return Point(x + v.x, y + v.y, z + v.z);
}

Point& Point::operator+=(const Vector &v)
{
	x += v.x; y += v.y; z += v.z;
	return *this;
}

Point Point::operator-(const Vector &v) const
{
	return Point(x - v.x, y - v.y, z - v.z);
}

Vector Point::operator-(const Point &p) const
{
	return Vector(x - p.x, y - p.y, z - p.z);
}

Point& Point::operator-=(const Vector &v)
{
	x -= v.x; y -= v.y; z -= v.z;
	return *this;
}

Point Point::operator/(float f) const
{
	assert(f != 0);
	float inv = 1.f / f;
	return Point(x * inv, y * inv, z * inv);
}

Point& Point::operator/=(float f)
{
	assert(f != 0);
	float inv = 1.f / f;
	x *= inv; y *= inv; z *= inv;
	return *this;
}

float Point::operator[](int i) const
{
	assert(i >= 0 && i < 3);
	return (&x)[i];
}

float& Point::operator[](int i)
{
	assert(i >= 0 && i < 3);
	return (&x)[i];
}

bool Point::HasNans() const
{
	return isnan(x) || isnan(y) || isnan(z);
}