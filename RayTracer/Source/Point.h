#ifndef POINT_H
#define POINT_H
class Vector;
class Normal;


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
	float operator[](int i) const;
	float& operator[](int i);
	bool HasNans() const;

	float x, y, z;
};

#endif