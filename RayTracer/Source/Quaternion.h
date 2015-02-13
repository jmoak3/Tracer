#ifndef QUATERNION_H
#define QUATERNION_H
#include "Vector.h"

class Quaternion
{
public:
	Quaternion();
	Quaternion(const Vector& vv, float ww);
	~Quaternion() {};

	Quaternion operator+(const Quaternion& q) const;
	Quaternion& operator+=(const Quaternion& q);

	Quaternion operator-(const Quaternion& q) const;
	Quaternion& operator-=(const Quaternion& q);

	Quaternion operator*(const Quaternion& q) const;
	Quaternion operator*=(const Quaternion& q);

	Quaternion operator*(const float f) const;
	Quaternion& operator*=(const float f);

	Quaternion operator/(const float f) const;
	Quaternion& operator/=(const float f);

	Vector v;
	float w;
};

#endif
