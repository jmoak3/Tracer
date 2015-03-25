#ifndef OBJLOADER_H
#define OBJLOADER_H
#include "TriangleMesh.h"
#include <vector>

class ObjLoader 
{
public:
	ObjLoader() {}
	~ObjLoader() {}
	TriangleMesh Construct(char* file, const Transform * o2w, const Transform * w2o, Material material);
	
	bool LoadMesh(char* file, std::vector<float> *points,
			std::vector<int> * indices, 
			std::vector<float> * normals, 
			std::vector<float> * uv);
};

#endif