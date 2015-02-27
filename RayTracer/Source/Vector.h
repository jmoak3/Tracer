#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>
class Normal;
class Point;

class Vector
{
public:
	Vector();
	Vector(float xx, float yy, float zz);
	Vector(const Point &p1, const Point &p2);
	explicit inline Vector(Normal &n);
	~Vector() {};

	Vector operator+(const Vector &v) const;
	Vector& operator+=(const Vector &v);
	Vector operator-(const Vector &v) const;
	Vector& operator-=(const Vector &v);
	Vector operator*(const float f) const;
	Vector& operator*=(const float f);
	Vector operator/(const float f) const;
	Vector& operator/=(const float f);
	Vector operator-() const;
	float operator[](int i) const;
	float& operator[](int i);
	
	float Length() const;
	float LengthSquared() const;

	bool HasNans() const;

	float x, y, z;
};

inline Vector Normalize(const Vector &v)
{
	return v / v.Length();
}

inline Vector Cross(const Vector &v1, const Vector &v2)
{
	return Vector((v1.y * v2.z) - (v1.z * v2.y),
		(v1.z * v2.x) - (v1.x * v2.z),
		(v1.x * v2.y) - (v1.y * v2.x));
}


inline float Dot(const Vector &v1, const Vector &v2) { return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z); }

inline float AbsDot(const Vector &v1, const Vector &v2) { return fabsf(Dot(v1, v2)); }

inline Vector operator*(const float f, const Vector &v) { return v*f; }

inline void CoordinateSystem(Vector &v1, Vector &v2, Vector &v3)
{
	if (fabsf(v1.x) > fabsf(v1.y))
	{
		float invLen = 1.f / (fabsf(v1.x*v1.x + v1.z*v1.z));
		v2 = Vector(-v1.z * invLen, 0.f, v1.x * invLen);
	}
	else
	{
		float invLen = 1.f / (fabsf(v1.y*v1.y + v1.z*v1.z));
		v2 = Vector(0.f, v1.z * invLen, -v1.y * invLen);
	}
	v3 = Cross(v1, v2);
}

#endif