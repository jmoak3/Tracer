#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H

#include "Shape.h"
#include <vector>

class Triangle;

class TriangleMesh : public Shape
{
public:
	TriangleMesh(const Transform * o2w, const Transform * w2o, Material material, int numTris, int numVerts, 
		const int * vertIndices, const Point * vertPoints, 
		const Normal * normals, const float * uv);
	~TriangleMesh() {delete Triangles;};

	void CalculateNormals();
	bool CanIntersect() const;
	bool Intersect(const Ray &ray, Hit * hit) const;
	BoundingBox ObjectBound() const; // objectspace bbox
	BoundingBox WorldBound() const; // worldspace bbox, saves time
	int NumTris;
	int NumVerts;
	int * VertIndices;
	Point * VertPoints;
	Normal * Normals;
	float * UVs;
	BoundingBox WorldBounds;

	std::vector<Triangle> *Triangles;
};


class Triangle : public Shape
{
public:
	Triangle(const Transform * o2w, const Transform *w2o, 
			Material material, const TriangleMesh * mesh, int num);
	~Triangle() {}

	bool Intersect(const Ray &ray, Hit * hit) const;
	bool CanIntersect() const;
	Normal GetNormal() const;
	BoundingBox ObjectBound() const;
	BoundingBox WorldBound() const;
	const TriangleMesh * Mesh;
	int *Vert;
	int Num;
	BoundingBox WorldBounds;
};

#endif