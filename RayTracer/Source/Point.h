#ifndef POINT_H
#define POINT_H

#include "Vector.h"


class Point
{
public:
	Point();
	Point(float xx, float yy, float zz);
	Point(const Vector & v);
	~Point() {};

	Point operator+(const Vector &v) const;
	Point& operator+=(const Vector &v);
	Point operator-(const Vector &v) const;
	Vector operator-(const Point &p) const;
	Point& operator-=(const Vector &v);
	Point operator/(float f) const;
	Point& operator/=(float f);
	float operator[](int i) const;
	float& operator[](int i);
	bool HasNans() const;

	float x, y, z;
	int filler;
};


inline float Distance(const Point &p1, const Point &p2) { return (p1 - p2).Length(); }

inline Point Average(const Point &p1, const Point &p2) 
{
	Point p((p1.x+p2.x)*0.5f, (p1.y+p2.y)*0.5f, (p1.z+p2.z)*0.5f);

	return p; 
}

inline float DistanceSquared(const Point &p1, const Point &p2) { return Vector(p1 - p2).LengthSquared(); }

#endif