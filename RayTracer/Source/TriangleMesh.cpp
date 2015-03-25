#include "TriangleMesh.h"

TriangleMesh::TriangleMesh(const Transform * o2w, const Transform * w2o, Material material, int numTris, int numVerts,
		const int * vertIndices, const Point * vertPoints, 
		const Normal * normals, const float * uv)
		: Shape(o2w, w2o, material)
{
	NumTris = numTris;
	NumVerts = numVerts;
	VertIndices = new int[3 * NumTris];
	printf("NumTris: %i\n", NumTris);
	memcpy(VertIndices, vertIndices, 3 * NumTris * sizeof(int));
	printf("VertIndices successfully added\n");
	
	if (vertPoints != NULL)
	{
		VertPoints = new Point[NumVerts];
		memcpy(VertPoints, vertPoints, NumVerts * sizeof(Point));
		for (int i=0;i<NumVerts;++i)
			VertPoints[i] = (*ObjectToWorld)(vertPoints[i]);
		printf("VertPoints successfully added\n");
	}

	if (normals != NULL)
	{
		Normals = new Normal[NumTris];
		memcpy(Normals, normals, NumTris * sizeof(Normal));
		for (int i=0;i<NumTris;++i)
			Normals[i] = Normalize((*ObjectToWorld)(normals[i]));
		for (int i=0;i<NumTris;++i)
			Normals[i];
		printf("TriNormals successfully added\n");
	}
	else
		CalculateNormals();

	if (uv != NULL)
	{
		UVs = new float[NumVerts*2];
		memcpy(UVs, uv, NumVerts* 2 * sizeof(float));
		printf("UVs successfully added\n");
	}

	WorldBounds = WorldBound();//speed
	printf("TriangleMesh Successfully added \n");

	//PreCache all tris till kdtrees/triangle refine/subdivide algo is implemented
	Triangles = new std::vector<Triangle>();
	for (int i=0;i<NumTris;++i)
	{
		Triangle Tri(ObjectToWorld, WorldToObject, 
					 Mat, this, i);
		Triangles->push_back(Tri);
	}
}

bool TriangleMesh::CanIntersect() const
{
	return true; //should be false; too slow
}

bool TriangleMesh::Intersect(const Ray & ray, Hit * hit) const
{
	//if (!CanIntersect())
	//	return false;

	if (!WorldBounds.Intersect(ray)) // BB
		return false;//speed

	for (int currTri=0;currTri<NumTris;++currTri)
	{
		//Triangle Tri(ObjectToWorld, WorldToObject, 
		//			 Mat, this, currTri);
		
		if ((*Triangles)[currTri].Intersect(ray, hit))
			return true;
	}
	
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
		Material material, const TriangleMesh *mesh, int num)
		: Shape(o2w, w2o, material)
{
	Mesh = mesh;
	Vert = &(Mesh->VertIndices[3*num]);
	Num = num;
	WorldBounds = WorldBound();//speed
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
	//if (!CanIntersect())
	//	return false;
	if (!WorldBounds.Intersect(ray))
		return false;
	Ray r = ray;

	Point a = Mesh->VertPoints[Vert[0]];
	Point b = Mesh->VertPoints[Vert[1]];
	Point c = Mesh->VertPoints[Vert[2]];
	Vector e1 = b-a;
	Vector e2 = c-a;
	Vector s1 = Cross(r.d, e2);

	float denominator = Dot(s1, e1);
	if (denominator==0.f)
		return false;
	float invDenom = 1.f/denominator;
	Vector s = r.o - a;
	float b1 = Dot(s, s1)*invDenom;
	if (b1 < 0 || b1 > 1.f)
		return false;

	Vector s2 = Cross(s, e1);
	float b2 = Dot(r.d, s2)*invDenom;
	if (b2 < 0.f || b1+b2 > 1.f)
		return false;

	float t = Dot(e2, s2)*invDenom;
	if (t < r.mint || t > r.maxt)
		return false;

	hit->tHit = t;
	hit->eps = 1e-3*t;
	hit->shapeID = ShapeID;
	hit->type = Type;
	hit->material = Mat;
	hit->normal = Mesh->Normals[Num];
	return true;
}

Normal Triangle::GetNormal() const
{
	assert(false);
	return Normal();
}