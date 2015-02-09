#ifndef VECTOR_H
#define VECTOR_H

#include "Point.h" 
#include "Inlines.h"

class Vector
{
public:
	Vector();
	Vector(float xx, float yy, float zz);
	Vector(Point &p1, Point &p2); 
	explicit Vector(Normal &n);
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

#endif