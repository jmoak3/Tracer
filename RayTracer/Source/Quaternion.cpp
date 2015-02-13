#include "Quaternion.h"
#include "Inlines.h"


Quaternion::Quaternion()
{
	v = Vector(0.f, 0.f, 0.f);
	w = 1.f;
}

Quaternion::Quaternion(const Vector& vv, float ww)
{
	v = vv;
	w = ww;
}

Quaternion Quaternion::operator+(const Quaternion& q) const
{
	return Quaternion(v + q.v, w + q.w);
}

Quaternion& Quaternion::operator+=(const Quaternion& q)
{
	v += q.v;
	w += q.w;
	return *this;
}

Quaternion Quaternion::operator-(const Quaternion& q) const
{
	return Quaternion(v - q.v, w - q.w);
}

Quaternion& Quaternion::operator-=(const Quaternion& q)
{
	v -= q.v;
	w -= q.w;
	return *this;
}

Quaternion Quaternion::operator*(const Quaternion& q) const
{
	return Quaternion(
		Cross(v, q.v) + w * q.v + q.w * v,
		w * q.w - Dot(v, q.v));
}

Quaternion Quaternion::operator*=(const Quaternion& q)
{
	float ww = w;
	w = ww * q.w - Dot(v, q.v);
	v = Cross(v, q.v) + ww * q.v + q.w * v;
}

Quaternion Quaternion::operator*(const float f) const
{
	return Quaternion(v * f, w * f);
}

Quaternion& Quaternion::operator*=(const float f)
{
	v *= f;
	w *= f;
	return *this;
}

Quaternion Quaternion::operator/(const float f) const
{
	float inv = 1.f / f;
	return Quaternion(v * inv, f * inv);
}

Quaternion& Quaternion::operator/=(const float f)
{
	float inv = 1.f / f;
	v *= inv;
	w *= inv;
	return *this;
}