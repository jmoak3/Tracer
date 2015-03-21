#include "ObjLoader.h"
#include <fstream>

TriangleMesh ObjLoader::Load(char* file)
{
	std::fstream fs;

	int *V = 0;
	int *N = 0;
	assert(LoadVerts(file, V));
	assert(LoadNormals(file, N));	
}

bool ObjLoader::LoadVerts(char* file, int * verts)
{
	return false;
}

bool ObjLoader::LoadNormals(char* file, int * normals)
{
	return false;
}