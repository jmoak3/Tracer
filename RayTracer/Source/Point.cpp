#include "Point.h"

Point::Point()
{
	x = y = z = 0.f;
}

Point::Point(float xx, float yy, float zz)
{
	x = xx; y = yy; z = zz;
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

Point& Point::operator-=(const Vector &v)
{
	x -= v.x; y -= v.y; z -= v.z;
	return *this;
}