#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Matrix4x4.h"
#include "Vector.h"
#include "Point.h"
#include "Ray.h"
#include "BoundingBox.h"

class Transform
{
public:
	Transform();
	Transform(const float mat[4][4]);
	Transform(const Matrix4x4 &mat);
	Transform(const Matrix4x4 &mat, const Matrix4x4 &inv);
	~Transform() {};
	
	friend Transform Inverse(const Transform &t);
	bool IsIdentity();
	bool HasScale();
	
	bool operator==(const Transform &t);
	bool operator!=(const Transform &t);
	
	Point operator()(const Point &p) const;
	void operator()(const Point &p, Point *pTrans) const;
	
	Vector operator()(const Vector &v) const;
	void operator()(const Vector &v, Vector *vTrans) const;
	
	Normal operator()(const Normal &n) const;
	void operator()(const Normal &n, Normal *nTrans) const;
	
	Ray operator()(const Ray &r) const;
	void operator()(const Ray &r, Ray *rTrans) const;

	BoundingBox operator()(const BoundingBox &bbox) const;
	void operator()(const BoundingBox &bbox, BoundingBox *bboxTrans) const;
	
	Transform operator()(const Transform &trans) const;
	void operator()(const Transform &trans, Transform *transTrans) const;

private:
	Matrix4x4 m;
	Matrix4x4 mInv;
};

inline Transform Translate(const Vector &v)
{
	Matrix4x4 mat(1, 0, 0, v.x,
		0, 1, 0, v.y,
		0, 0, 1, v.z,
		0, 0, 0, 1);

	Matrix4x4 inv(1, 0, 0, -v.x,
		0, 1, 0, -v.y,
		0, 0, 1, -v.z,
		0, 0, 0, 1);

	return Transform(mat, inv);
}

inline Transform Scale(float x, float y, float z)
{
	Matrix4x4 mat(x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1);

	Matrix4x4 inv(1.f / x, 0, 0, 0,
		0, 1.f / y, 0, 0,
		0, 0, 1.f / z, 0,
		0, 0, 0, 1);

	return Transform(mat, inv);
}

inline Transform RotateX(float angle)
{
	float sint = sinf((0.017453f*angle));
	float cost = cosf((0.017453f*angle));
	Matrix4x4 mat(1, 0, 0, 0,
		0, cost, -sint, 0,
		0, sint, cost, 0,
		0, 0, 0, 1);


	return Transform(mat, Transpose(mat));
}

inline Transform RotateY(float angle)
{
	float sint = sinf((0.017453f*angle));
	float cost = cosf((0.017453f*angle));
	Matrix4x4 mat(cost, 0, sint, 0,
		0, 1, 0, 0,
		-sint, 0, cost, 0,
		0, 0, 0, 1);

	return Transform(mat, Transpose(mat));
}

inline Transform RotateZ(float angle)
{
	float sint = sinf((0.017453f*angle));
	float cost = cosf((0.017453f*angle));
	Matrix4x4 mat(cost, -sint, 0, 0,
		sint, cost, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	return Transform(mat, Transpose(mat));
}

inline Transform Rotate(float angle, const Vector &axis)
{
	Vector a = Normalize(axis);
	float s = sinf(0.017453f*angle);
	float c = cosf(0.017453f*angle);
	float m[4][4];

	m[0][0] = a.x*a.x + (1.f - a.x*a.x)*c;
	m[0][1] = a.x*a.y*(1.f - c) - a.z*s;
	m[0][2] = a.x*a.z*(1.f - c) + a.y*s;
	m[0][3] = 0;
	
	m[1][0] = a.x*a.y*(1.f - c) + a.z*s;
	m[1][1] = a.y*a.y + (1.f - a.y*a.y)*c;
	m[1][2] = a.y*a.z*(1.f - c) - a.x*s;
	m[1][3] = 0;

	m[2][0] = a.x*a.z*(1.f - c) - a.y*s;
	m[2][1] = a.y*a.z*(1.f - c) + a.x*s;
	m[2][2] = a.z*a.z + (1.f - a.z*a.z)*c;
	m[2][3] = 0;

	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = 0;
	m[3][3] = 1;

	Matrix4x4 mat(m);
	return Transform(mat, Transpose(mat));
}

inline Transform LookAt(const Point &pos, const Point &look, const Vector &up)
{
	float m[4][4];
	Vector dir = Normalize(look - pos);
	Vector left = Normalize(Cross(Normalize(up), dir));
	Vector newUp = Cross(dir, left);
	m[0][0] = left.x;
	m[1][0] = left.y;
	m[2][0] = left.z;
	m[3][0] = 0.f;
	m[0][1] = newUp.x;
	m[1][1] = newUp.y;
	m[2][1] = newUp.z;
	m[3][1] = 0.f;
	m[0][2] = dir.x;
	m[1][2] = dir.y;
	m[2][2] = dir.z;
	m[3][2] = 0.f;

	m[0][3] = pos.x;
	m[1][3] = pos.y;
	m[2][3] = pos.z;
	m[3][3] = 1;
}



#endif
