#include "Matrix4x4.h"
#include <utility>
#include <assert.h>
#include <math.h>

Matrix4x4::Matrix4x4()
{
	m[0][0] = 1.f;
	m[0][1] = 0.f;
	m[0][2] = 0.f;
	m[0][3] = 0.f;

	m[1][0] = 0.f;
	m[1][1] = 1.f;
	m[1][2] = 0.f;
	m[1][3] = 0.f;

	m[2][0] = 0.f;
	m[2][1] = 0.f;
	m[2][2] = 1.f;
	m[2][3] = 0.f;

	m[3][0] = 0.f;
	m[3][1] = 0.f;
	m[3][2] = 0.f;
	m[3][3] = 1.f;
}

Matrix4x4::Matrix4x4(float mat[4][4])
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			m[i][j] = mat[i][j];
}

Matrix4x4::Matrix4x4(float t00, float t01, float t02, float t03,
					 float t10, float t11, float t12, float t13,
					 float t20, float t21, float t22, float t23,
					 float t30, float t31, float t32, float t33)
{
	m[0][0] = t00;
	m[0][1] = t01;
	m[0][2] = t02;
	m[0][3] = t03;

	m[1][0] = t10;
	m[1][1] = t11;
	m[1][2] = t12;
	m[1][3] = t13;

	m[2][0] = t20;
	m[2][1] = t21;
	m[2][2] = t22;
	m[2][3] = t23;

	m[3][0] = t30;
	m[3][1] = t31;
	m[3][2] = t32;
	m[3][3] = t33;
}

Matrix4x4 Transpose(const Matrix4x4 &mat)
{
	return Matrix4x4(mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
					 mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
					 mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
					 mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]);
}

bool Matrix4x4::operator==(const Matrix4x4 &mat)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			if (mat.m[i][j] != m[i][j])
				return false;
	return true;
}

Matrix4x4 Matrix4x4::Mul(const Matrix4x4 &m1, const Matrix4x4 &m2)
{
	Matrix4x4 r;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4;++j)
			r.m[i][j] = m1.m[i][0] * m2.m[0][j] +
						m1.m[i][1] * m2.m[1][j] +
						m1.m[i][2] * m2.m[2][j] +
						m1.m[i][3] * m2.m[3][j];
	return r;
}

Matrix4x4 Inverse(const Matrix4x4 &mat)
{
	//SPECIAL THANKS TO
	//PHYSICALLY BASED RENDERING: FROM THEORY TO IMPLEMENTATION
	//BY MATT PHARR AND GREG HUMPHREYS
	//FOR THEIR GREAT IMPLEMENTATION OF 
	//OF A NUMERICALLY STABLE GUASSIAN-JORDAN ROUTINE!!!
	int indxc[4], indxr[4];
	int ipiv[4] = { 0, 0, 0, 0 };
	float minv[4][4];
	memcpy(minv, mat.m, 4 * 4 * sizeof(float));
	for (int i = 0; i < 4; ++i) {
		int irow = -1, icol = -1;
		float big = 0.;
		for (int j = 0; j < 4; j++) {
			if (ipiv[j] != 1) {
				for (int k = 0; k < 4; ++k) {
					if (ipiv[k] == 0) {
						if (fabsf(minv[j][k]) >= big) {
							big = float(fabsf(minv[j][k]));
							irow = j;
							icol = k;
						}
					}
					assert(ipiv[k] <= 1);
				}
			}
		}
		++ipiv[icol];
		if (irow != icol) {
			for (int k = 0; k < 4; ++k)
				std::swap(minv[irow][k], minv[icol][k]);
		}
		indxr[i] = irow;
		indxc[i] = icol;
		assert(minv[icol][icol] != 0.f);
		float pivinv = 1.f / minv[icol][icol];
		minv[icol][icol] = 1.f;
		for (int j = 0; j < 4; j++)
			minv[icol][j] *= pivinv;

		for (int j = 0; j < 4; ++j) {
			if (j != icol) {
				float save = minv[j][icol];
				minv[j][icol] = 0;
				for (int k = 0; k < 4; k++)
					minv[j][k] -= minv[icol][k] * save;
			}
		}
	}
	for (int j = 3; j >= 0; --j) {
		if (indxr[j] != indxc[j]) {
			for (int k = 0; k < 4; k++)
				std::swap(minv[k][indxr[j]], minv[k][indxc[j]]);
		}
	}
	return Matrix4x4(minv);
}