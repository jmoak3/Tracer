#ifndef KDTREE_H
#define KDTREE_H
#include <vector>
#include "TriangleMesh.h"

class KDNode
{
public:
	KDNode() {};
	~KDNode() {delete Left, Right;};

	BoundingBox Bounds;
	KDNode *Left;
	KDNode *Right;
	std::vector<Triangle*> Objects;
	bool Leaf;

	static KDNode * Build(std::vector<Triangle*> *scene, int depth=0);
	bool Intersect(const Ray & ray, Hit * hit);
};

#endif