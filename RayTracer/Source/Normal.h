#ifndef NORMAL_H
#define NORMAL_H

class Vector;
#include <math.h>

class Normal
{
public:
	explicit Normal();
	explicit Normal(float xx, float yy, float zz);
	explicit Normal(const Vector &v);

	Normal operator+(const Normal &n) const;
	Normal& operator+=(const Normal &v);
	Normal operator-(const Normal &v) const;
	Normal& operator-=(const Normal &v);
	Normal operator*(float f) const;
	Normal& operator*=(float f);
	Normal operator/(float f) const;
	Normal& operator/=(float f);
	Normal operator-() const;
	float operator[](int i) const;
	float& operator[](int i);

	float Length() const;
	float LengthSquared() const;

	bool HasNans() const;

	float x, y, z;
};

inline Normal Normalize(const Normal &n)
{
	return n / n.Length();
}

inline Normal Cross(const Normal &n1, const Normal &n2)
{
	return Normal((n1.y * n2.z) - (n1.z * n2.y),
		(n1.z * n2.x) - (n1.x * n2.z),
		(n1.x * n2.y) - (n1.y * n2.x));
}

inline float Dot(const Normal &n1, const Normal &n2) { return (n1.x*n2.x + n1.y*n2.y + n1.z*n2.z); }

inline Normal operator*(float f, const Normal &n) { return n*f; }

inline float AbsDot(const Normal &n1, const Normal &n2) { return fabsf(Dot(n1, n2)); }

#endif