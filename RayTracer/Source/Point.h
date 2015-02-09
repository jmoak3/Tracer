
#ifndef POINT_H
#define POINT_H
#include "Inlines.h"
#include "Vector.h"//Pleeeeeease don't break this

class Point
{
public:
	Point();
	Point(float xx, float yy, float zz);
	~Point() {};

	Point operator+(const Vector &v) const;
	Point& operator+=(const Vector &v);
	Point operator-(const Vector &v) const;
	Point& operator-=(const Vector &v);

	float x, y, z;
};

#endif