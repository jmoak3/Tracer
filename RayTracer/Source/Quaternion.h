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

inline Quaternion Normalize(const Quaternion &q) { return q / sqrtf(Dot(q, q)); }

inline Quaternion operator*(const float f, const Quaternion &q) { return q*f; }

inline float Dot(const Quaternion& q1, const Quaternion& q2) { return Dot(q1.v, q2.v) + q1.w * q2.w; }

#endif
