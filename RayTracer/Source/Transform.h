#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Matrix4x4.h"

class Transform
{
public:
	Transform() {};
	Transform(const float mat[4][4]);
	Transform(const Matrix4x4 &mat, const Matrix4x4 &inv);
	~Transform() {};
	
private:
	
};

#endif
