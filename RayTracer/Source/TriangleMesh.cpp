#include "TriangleMesh.h"

TriangleMesh::TriangleMesh(const Transform * o2w, const Transform * w2o, Material material, int numTris, int numVerts,
		const int * vertIndices, const Point * vertPoints, 
		const Normal * normals, const float * uv)
		: Shape(o2w, w2o, material)
{
	NumTris = numTris;
	NumVerts = numVerts;
	VertIndices = new int[3 * NumVerts];
	memcpy(VertIndices, vertIndices, 3 * NumTris * sizeof(int));
	
	if (vertPoints != NULL)
	{
		memcpy(VertPoints, vertPoints, NumVerts * sizeof(Point));
		for (int i=0;i<NumVerts;++i)
			VertPoints[i] = (*ObjectToWorld)(vertPoints[i]);
	}

	if (normals != NULL)
		memcpy(Normals, normals, NumVerts * sizeof(Normals));
	else
		CalculateNormals();

	if (uv != NULL)
		memcpy(UVs, uv, NumVerts * sizeof(int));
}

bool TriangleMesh::CanIntersect() const
{
	return false;
}

BoundingBox TriangleMesh::ObjectBound() const
{
	BoundingBox bounds;
	for (int i=0;i<NumVerts;++i)
		bounds = Union(bounds, (*WorldToObject)(VertPoints[i]));
	return bounds;
}

BoundingBox TriangleMesh::WorldBound() const
{
	BoundingBox bounds;
	for (int i=0;i<NumVerts;++i)
		bounds = Union(bounds, VertPoints[i]);
	return bounds;
}

void TriangleMesh::CalculateNormals() 
{
	assert(false);
}


//TRIANGLES
Triangle::Triangle(const Transform * o2w, const Transform * w2o,
		Material material, TriangleMesh *mesh, int num)
		: Shape(o2w, w2o, material)
{
	Mesh = mesh;
	Vert = &(Mesh->VertIndices[3*num]);
}

bool Triangle::CanIntersect() const
{
	return true;
}

BoundingBox Triangle::ObjectBound() const
{
	BoundingBox bounds;
	Point p1 = Mesh->VertPoints[Vert[0]];
	Point p2 = Mesh->VertPoints[Vert[1]];
	Point p3 = Mesh->VertPoints[Vert[2]];
	bounds = Union(Union(Union(bounds, (*WorldToObject)(p1)),
				(*WorldToObject)(p2)),	(*WorldToObject)(p3));
	return bounds;
}

BoundingBox Triangle::WorldBound() const
{
	BoundingBox bounds;
	Point p1 = Mesh->VertPoints[Vert[0]];
	Point p2 = Mesh->VertPoints[Vert[1]];
	Point p3 = Mesh->VertPoints[Vert[2]];
	bounds = Union(Union(Union(bounds, p1), p2), p3);
	return bounds;
}

bool Triangle::Intersect(const Ray & ray, Hit * hit) const
{
	if (!CanIntersect())
		return false;

	Ray r;
	(*WorldToObject)(ray, &r);

	Point A = Mesh->VertPoints[Vert[0]];
	Point B = Mesh->VertPoints[Vert[1]];
	Point C = Mesh->VertPoints[Vert[2]];
	Vector AC = C-A;
	Vector AB = B-A;
	Vector normal = Cross(AB, AC);
	float t = -Dot(r.o-A, normal)/Dot(r.d, normal);
	if (t < r.mint || t > r.maxt) return false;
	int k=0;
	if (fabsf(normal.x) > fabsf(normal.y))
	{
		if (fabsf(normal.x) > fabsf(normal.z))
			k=0;
		else
			k=2;
	}
	else
	{
		if (fabsf(normal.y) > fabsf(normal.z))
			k=1;
		else 
			k=2;
	}
	int u = (k+1)%3;
	int v = (k+2)%3;
	float H[3];
	H[u] = r.o[u] + t*r.d[u];
	H[v] = r.o[v] + t*r.d[v];

	float denominator = 1.f/(AC[u]*AB[v] - AC[v]*AB[u]);
	float b = (AC[u]*H[v] - AC[v]*H[u])*denominator;
	if (b<0 || b>1) 
		return false;

	float c = (AB[v]*H[u] - AB[u]*H[v])*denominator;
	if (c<0 || b+c>1)
		return false;
	
	hit->tHit = t;
	hit->eps = 5e-4*t;
	hit->shapeID = ShapeID;
	hit->type = Type;
	hit->material = Mat;
	hit->normal = Normal(normal);
}

Normal Triangle::GetNormal() const
{
	assert(false);
}