#include "Transform.h"
#include <assert.h>
#include <math.h>
#include <assert.h>
#include "Normal.h"

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
	return false;
}

bool Transform::operator==(const Transform &t)
{
	return (m == t.m && mInv == t.mInv);
}

bool Transform::operator!=(const Transform &t)
{
	return !(m == t.m && mInv == t.mInv);
}

Point Transform::operator()(const Point &p) const
{
	float x = p.x; float y = p.y; float z = p.z;
	float newX = m.m[0][0]*x + m.m[0][1]*y + m.m[0][2]*z + m.m[0][3];
	float newY = m.m[1][0]*x + m.m[1][1]*y + m.m[1][2]*z + m.m[1][3];
	float newZ = m.m[2][0]*x + m.m[2][1]*y + m.m[2][2]*z + m.m[2][3];
	float newW = m.m[3][0]*x + m.m[3][1]*y + m.m[3][2]*z + m.m[3][3];
	if (newW == 1.f) return Point(newX, newY, newZ);
	else return (Point(newX, newY, newZ)/newW);
}

void Transform::operator()(const Point &p, Point *pTrans) const
{
	float x = p.x; float y = p.y; float z = p.z;
	pTrans->x =  m.m[0][0]*x + m.m[0][1]*y + m.m[0][2]*z + m.m[0][3];
	pTrans->y =  m.m[1][0]*x + m.m[1][1]*y + m.m[1][2]*z + m.m[1][3];
	pTrans->z =  m.m[2][0]*x + m.m[2][1]*y + m.m[2][2]*z + m.m[2][3];
	float newW = m.m[3][0]*x + m.m[3][1]*y + m.m[3][2]*z + m.m[3][3];
	if (newW != 1.f) (*pTrans)/=newW;
}

Vector Transform::operator()(const Vector &v) const
{
	float x = v.x; float y = v.y; float z = v.z;
	return Vector(m.m[0][0]*x + m.m[0][1]*y + m.m[0][2]*z,
				  m.m[1][0]*x + m.m[1][1]*y + m.m[1][2]*z,
				  m.m[2][0]*x + m.m[2][1]*y + m.m[2][2]*z);
}

void Transform::operator()(const Vector &v, Vector *vTrans) const
{
	float x = v.x; float y = v.y; float z = v.z;
	vTrans->x =  m.m[0][0]*x + m.m[0][1]*y + m.m[0][2]*z;
	vTrans->y =  m.m[1][0]*x + m.m[1][1]*y + m.m[1][2]*z;
	vTrans->z =  m.m[2][0]*x + m.m[2][1]*y + m.m[2][2]*z;
}

Normal Transform::operator()(const Normal &n) const
{
	float x = n.x; float y = n.y; float z = n.z;
	return Normal(mInv.m[0][0]*x + mInv.m[1][0]*y + mInv.m[2][0]*z,
				  mInv.m[0][1]*x + mInv.m[1][1]*y + mInv.m[2][1]*z,
				  mInv.m[0][2]*x + mInv.m[1][2]*y + mInv.m[2][2]*z);
}

void Transform::operator()(const Normal &n, Normal *nTrans) const
{
	float x = n.x; float y = n.y; float z = n.z;
	nTrans->x =  mInv.m[0][0]*x + mInv.m[1][0]*y + mInv.m[2][0]*z;
	nTrans->y =  mInv.m[0][1]*x + mInv.m[1][1]*y + mInv.m[2][1]*z;
	nTrans->z =  mInv.m[0][2]*x + mInv.m[1][2]*y + mInv.m[2][2]*z;
}

Ray Transform::operator()(const Ray &r) const
{
	Ray ret = r;
	(*this)(ret.o, &ret.o);
	(*this)(ret.d, &ret.d);
	return ret;
}

void Transform::operator()(const Ray &r, Ray *rTrans) const
{
	(*this)(r.o, &(rTrans->o));
	(*this)(r.d, &(rTrans->d));
	rTrans->maxt = r.maxt;
	rTrans->mint = r.mint;
	rTrans->depth = r.depth;
	rTrans->refrIndex = r.refrIndex;
	rTrans->time = r.time;
	rTrans->UpdateInverse();
}

BoundingBox Transform::operator()(const BoundingBox &bbox) const
{
	const Transform &T = (*this);
	BoundingBox ret( T(Point(bbox.Min.x, bbox.Min.y, bbox.Min.z)));
	ret = Union(ret, T(Point(bbox.Max.x, bbox.Min.y, bbox.Min.z)));
	ret = Union(ret, T(Point(bbox.Min.x, bbox.Max.y, bbox.Min.z)));
	ret = Union(ret, T(Point(bbox.Min.x, bbox.Min.y, bbox.Max.z)));
	ret = Union(ret, T(Point(bbox.Min.x, bbox.Max.y, bbox.Max.z)));
	ret = Union(ret, T(Point(bbox.Max.x, bbox.Max.y, bbox.Min.z)));
	ret = Union(ret, T(Point(bbox.Max.x, bbox.Min.y, bbox.Max.z)));
	ret = Union(ret, T(Point(bbox.Max.x, bbox.Max.y, bbox.Max.z)));
	return ret;
}

void Transform::operator()(const BoundingBox &bbox, BoundingBox *bboxTrans) const
{
	const Transform &T = (*this);
	BoundingBox ret( T(Point(bbox.Min.x, bbox.Min.y, bbox.Min.z)));
	ret = Union(ret, T(Point(bbox.Max.x, bbox.Min.y, bbox.Min.z)));
	ret = Union(ret, T(Point(bbox.Min.x, bbox.Max.y, bbox.Min.z)));
	ret = Union(ret, T(Point(bbox.Min.x, bbox.Min.y, bbox.Max.z)));
	ret = Union(ret, T(Point(bbox.Min.x, bbox.Max.y, bbox.Max.z)));
	ret = Union(ret, T(Point(bbox.Max.x, bbox.Max.y, bbox.Min.z)));
	ret = Union(ret, T(Point(bbox.Max.x, bbox.Min.y, bbox.Max.z)));
	ret = Union(ret, T(Point(bbox.Max.x, bbox.Max.y, bbox.Max.z)));
	bboxTrans->Min = ret.Min;
	bboxTrans->Max = ret.Max;
}

Transform Transform::operator()(const Transform &trans) const
{
	Matrix4x4 m1 = Matrix4x4::Mul(m, trans.m);
	Matrix4x4 m2 = Matrix4x4::Mul(trans.mInv, mInv);
	return Transform(m1, m2);
}

void Transform::operator()(const Transform &trans, Transform *transTrans) const
{
	transTrans->m = Matrix4x4::Mul(m, trans.m);
	transTrans->mInv = Matrix4x4::Mul(trans.mInv, mInv);
}

