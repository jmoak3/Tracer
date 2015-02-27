#ifndef POINT_H
#define POINT_H

#include "Vector.h"


class Point
{
public:
	Point();
	Point(float xx, float yy, float zz);
	~Point() {};

	Point operator+(const Vector &v) const;
	Point& operator+=(const Vector &v);
	Point operator-(const Vector &v) const;
	Vector operator-(const Point &p) const;
	Point& operator-=(const Vector &v);
	Point operator/(const float f) const;
	Point& operator/=(const float f);
	float operator[](int i) const;
	float& operator[](int i);
	bool HasNans() const;

	float x, y, z;
};


inline float Distance(const Point &p1, const Point &p2) { return (p1 - p2).Length(); }

inline float DistanceSquared(const Point &p1, const Point &p2) { return Vector(p1 - p2).LengthSquared(); }

#endif