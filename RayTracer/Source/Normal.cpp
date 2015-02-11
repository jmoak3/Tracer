#include "Normal.h"
#include "Vector.h"
#include <math.h>

Normal::Normal()
{
	x = y = z = 0.f;
}

Normal::Normal(float xx, float yy, float zz)
{
	x = xx; y = yy; z = zz;
}

Normal::Normal(const Vector &v)
{
	x = v.x; y = v.y; z = v.z;
}

Normal Normal::operator+(const Normal &n) const
{
	return Normal(x + n.x, y + n.y, z + n.z);
}

Normal& Normal::operator+=(const Normal &n)
{
	x += n.x; y += n.y; z += n.z;
	return *this;
}

Normal Normal::operator-(const Normal &n) const
{
	return Normal(x - n.x, y - n.y, z - n.z);
}

Normal& Normal::operator-=(const Normal &n)
{
	x -= n.x; y -= n.y; z -= n.z;
	return *this;
}

Normal Normal::operator*(const float f) const
{
	return Normal(x * f, y * f, z * f);
}

Normal& Normal::operator*=(const float f)
{
	x *= f; y *= f; z *= f;
	return *this;
}

Normal Normal::operator/(const float f) const
{
	assert(f != 0);
	float inv = 1.f / f;
	return Normal(x * inv, y * inv, z * inv);
}

Normal& Normal::operator/=(const float f)
{
	assert(f != 0);
	float inv = 1.f / f;
	x *= inv; y *= inv; z *= inv;
	return *this;
}

float Normal::operator[](int i) const
{
	assert(i > 0 && i < 3);
	return (&x)[i];
}

float& Normal::operator[](int i)
{
	assert(i > 0 && i < 3);
	return (&x)[i];
}

float Normal::Length() const
{
	return sqrtf(LengthSquared());
}

float Normal::LengthSquared() const
{
	return (x*x + y*y + z*z);
}

bool Normal::HasNans() const
{
	return isnan(x) || isnan(y) || isnan(z);
}
