#include "Transform.h"
#include <assert.h>
#include <math.h>
#include "
Transform::Transform()
{
	m = Matrix4x4();
	mInv = m;
}

Transform::Transform(const float mat[4][4])
{
	m = Matrix4x4(mat[0][0], mat[0][1], mat[0][2], mat[0][3],
				  mat[1][0], mat[1][1], mat[1][2], mat[1][3],
				  mat[2][0], mat[2][1], mat[2][2], mat[2][3],
				  mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
	mInv = Inverse(m);
}

Transform::Transform(const Matrix4x4 &mat)
{
	m = mat;
	mInv = Inverse(m);
}

Transform::Transform(const Matrix4x4 &mat, const Matrix4x4 &inv)
{
	m = mat;
	mInv = inv;
}

Transform Inverse(const Transform &t)
{
	return Transform(t.mInv, t.m);
}

bool Transform::IsIdentity()
{
	Matrix4x4 i;
	return (m == i);
}

bool Transform::HasScale()
{
	assert(false);
	// NOT IMPLEMENTED/NECESSARY???
}

bool Transform::operator==(const Transform &t)
{
	return (m == t.m && mInv == t.mInv);
}

bool Transform::operator!=(const Transform &t)
{
	return !(m == t.m && mInv == t.mInv);
}

