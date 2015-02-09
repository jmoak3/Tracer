#ifndef NORMAL_H
#define NORMAL_H

#include "Inlines.h"
#include "Vector.h"

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
	Normal operator*(const float f) const;
	Normal& operator*=(const float f);
	Normal operator/(const float f) const;
	Normal& operator/=(const float f);
	Normal operator-() const;
	float operator[](int i) const;
	float& operator[](int i);

	float Length() const;
	float LengthSquared() const;

	bool HasNans() const;

	float x, y, z;
};

#endif