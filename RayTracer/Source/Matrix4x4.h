#ifndef MATRIX4X4_H
#define MATRIX4X4_H

struct Matrix4x4
{
	Matrix4x4();
	Matrix4x4(float mat[4][4]);
	Matrix4x4(float t00, float t01, float t02, float t03,
			  float t10, float t11, float t12, float t13,
			  float t20, float t21, float t22, float t23,
			  float t30, float t31, float t32, float t33);
	friend Matrix4x4 Transpose(const Matrix4x4 &mat);
	static Matrix4x4 Mul(const Matrix4x4 &m1, const Matrix4x4 &m2);
	friend Matrix4x4 Inverse(const Matrix4x4 &mat);
	bool operator==(const Matrix4x4 &mat);

	float m[4][4];
};

#endif