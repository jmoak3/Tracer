#ifndef INLINES_H
#define INLINES_H

struct RGB
{
	int red, green, blue;
};
#include "Point.h"
#include "Vector.h"
#include "Normal.h"
#include "Quaternion.h"
#include "Camera.h"
#include <assert.h>
#include <math.h>
#include <utility>


inline float Dot(const Vector &v1, const Vector &v2) { return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z); }

inline float Dot(const Normal &n1, const Normal &n2) { return (n1.x*n2.x + n1.y*n2.y + n1.z*n2.z); }

inline float Dot(const Quaternion& q1, const Quaternion& q2) { return Dot(q1.v, q2.v) + q1.w * q2.w; }

inline float AbsDot(const Vector &v1, const Vector &v2) { return fabsf(Dot(v1, v2)); }

inline float AbsDot(const Normal &n1, const Normal &n2) { return fabsf(Dot(n1, n2)); }

inline Vector Cross(const Vector &v1, const Vector &v2)
{
	return Vector((v1.y * v2.z) - (v1.z * v2.y),
			      (v1.z * v2.x) - (v1.x * v2.z),
				  (v1.x * v2.y) - (v1.y * v2.x));
}

inline Normal Cross(const Normal &n1, const Normal &n2)
{
	return Normal((n1.y * n2.z) - (n1.z * n2.y),
				  (n1.z * n2.x) - (n1.x * n2.z),
				  (n1.x * n2.y) - (n1.y * n2.x));
}

inline Vector operator*(const float f, const Vector &v) { return v*f; }

inline Normal operator*(const float f, const Normal &n) { return n*f; }

inline Quaternion operator*(const float f, const Quaternion &q) { return q*f; }

inline Vector Normalize(const Vector &v) { return v / v.Length(); }

inline Normal Normalize(const Normal &n) { return n / n.Length(); }

inline Quaternion Normalize(const Quaternion &q) { return q / sqrtf(Dot(q, q)); }

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

inline float Distance(const Point &p1, const Point &p2) { return (p1 - p2).Length(); }

inline float DistanceSquared(const Point &p1, const Point &p2) { return Vector(p1 - p2).LengthSquared(); }

inline bool Quadratic(float A, float B, float C, float *t0, float *t1)
{
	float discrim = (B*B - 4.f * A*C);
	if (discrim < 0.f) return false;
	float rtDiscrim = sqrtf(discrim);

	float q;

	if (B < 0)
		q = -0.5f*(B - rtDiscrim);
	else
		q = 0.5f*(B + rtDiscrim);
	*t0 = q / A;
	*t1 = C / q;
	if (*t1 < *t0) std::swap(*t0, *t1);
	return true;
}

#endif