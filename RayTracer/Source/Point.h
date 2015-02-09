#include "Vector.h"//Pleeeeeease don't break this

#ifndef _POINT_H_
#define _POINT_H_

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