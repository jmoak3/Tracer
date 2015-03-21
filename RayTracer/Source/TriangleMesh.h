#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H

#include "Shape.h"

class TriangleMesh : public Shape
{
public:
	TriangleMesh(const Transform * o2w, const Transform * w2o, Material material, int numTris, int numVerts, 
		const int * vertIndices, const Point * vertPoints, 
		const Normal * normals, const float * uv);
	~TriangleMesh() {};

	void CalculateNormals();
	bool CanIntersect() const;
	BoundingBox ObjectBound() const; // objectspace bbox
	BoundingBox WorldBound() const; // worldspace bbox, saves time
	int NumTris;
	int NumVerts;
	int * VertIndices;
	Point * VertPoints;
	Normal * Normals;
	float * UVs;
};


class Triangle : public Shape
{
public:
	Triangle(const Transform * o2w, const Transform *w2o, 
			Material material, TriangleMesh * mesh, int num);
	~Triangle() {}

	bool Intersect(const Ray &ray, Hit * hit) const;
	bool CanIntersect() const;
	Normal GetNormal() const;
	BoundingBox ObjectBound() const;
	BoundingBox WorldBound() const;
	TriangleMesh * Mesh;
	int *Vert;
	int Num;
};

#endif