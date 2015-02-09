#ifndef _VECTOR_H_
#define _VECTOR_H_

class Vector
{
public:
	Vector();
	Vector(float xx, float yy, float zz);
	~Vector() {};

	float x, y, z;
};

#endif