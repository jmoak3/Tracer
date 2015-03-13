#include "Vector.h"
#include "Normal.h"
#include "Point.h"
#include <math.h>
#include <assert.h>

Vector::Vector()
{
	x = y = z = 0.f;
}

Vector::Vector(float xx, float yy, float zz)
{
	x = xx; y = yy; z = zz;
	assert(!HasNans());
}

Vector::Vector(const Point &p1, const Point &p2)
{
	x = p2.x - p1.x; y = p2.y - p1.y; z = p2.z - p1.z;
}

Vector::Vector(const Normal &n)
{
	x = n.x; y = n.y; z = n.z;
}

Vector Vector::operator+(const Vector &v) const
{
	return Vector(x + v.x, y + v.y, z + v.z);
}

Vector& Vector::operator+=(const Vector &v)
{
	x += v.x; y += v.y; z += v.z;
	return *this;
}

Vector Vector::operator-(const Vector &v) const
{
	return Vector(x - v.x, y - v.y, z - v.z);
}

Vector& Vector::operator-=(const Vector &v)
{
	x -= v.x; y -= v.y; z -= v.z;
	return *this;
}

Vector Vector::operator-() const
{
	return Vector(-x, -y, -z);
}


Vector Vector::operator*(const float f) const
{
	return Vector(x * f, y * f, z * f);
}

Vector& Vector::operator*=(const float f)
{
	x *= f; y *= f; z *= f;
	return *this;
}

Vector Vector::operator/(const float f) const
{
	assert(f != 0);
	float inv = 1.f / f;
	return Vector(x * inv, y * inv, z * inv);
}

Vector& Vector::operator/=(const float f)
{
	assert(f != 0);
	float inv = 1.f / f;
	x *= inv; y *= inv; z *= inv;
	return *this;
}

float Vector::operator[](int i) const
{
	assert(i >= 0 && i < 3);
	return (&x)[i];
}

float& Vector::operator[](int i)
{
	assert(i >= 0 && i < 3);
	return (&x)[i];
}

float Vector::Length() const
{
	return sqrtf(LengthSquared());
}

float Vector::LengthSquared() const
{
	return (x*x + y*y + z*z);
}

bool Vector::HasNans() const
{
	return isnan(x) || isnan(y) || isnan(z);
}
