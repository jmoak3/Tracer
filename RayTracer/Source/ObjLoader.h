#ifndef OBJLOADER_H
#define OBJLOADER_H
#include "TriangleMesh.h"

class ObjLoader 
{
public:
	ObjLoader() {}
	~ObjLoader() {}
	TriangleMesh Load(char* file);
	bool LoadVerts(char* file, int *verts);
	bool LoadNormals(char* file, int *normals);
};

#endif