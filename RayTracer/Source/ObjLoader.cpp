#include "ObjLoader.h"
#include <fstream>
#include <vector>
#include <math.h>
TriangleMesh ObjLoader::Construct(char* file, const Transform * o2w, const Transform * w2o, Material material)
{
	std::fstream fs;

	std::vector<int> I;
	std::vector<float> P;
	std::vector<float> N;
	std::vector<float> UV;
	bool result = LoadMesh(file, &P, &I, &N, &UV);
	assert(result);
	//convert to int/float/point/normal pointers!
	int * indices = new int[I.size()];
	for (int i=0;i<I.size();++i)
		indices[i] = I[i];
	if (I.size() == 0)
		indices = 0;

	Point * points = new Point[P.size()/3];
	for (int i=0;i<P.size();i+=3)
		points[i/3] = Point(P[i], P[i+1], P[i+2]);
	if (P.size() == 0)
		points = 0;

	Normal * normals = new Normal[N.size()/3];
	for (int i=0;i<N.size();i+=3)
		normals[i/3] = Normal(N[i], N[i+1], N[i+2]);
	if (N.size() == 0)
		normals = 0;

	float * uv = new float[UV.size()];
	for (int i=0;i<UV.size();++i)
		uv[i] = UV[i];
	if (UV.size() == 0)
		uv = 0;

	int g = I.size()/3;
	return TriangleMesh(o2w, w2o, material, I.size()/3, P.size()/3, indices, points, normals, uv);
}

bool ObjLoader::LoadMesh(char* file, 
					std::vector<float> *points,
					std::vector<int> * indices, 
					std::vector<float> * normals, 
					std::vector<float> * uv)
{
	char header[64];
	std::vector<float>* tempNormals = new std::vector<float>();
	std::ifstream modelFile;
	modelFile.open(file, std::ios::in);
	while(modelFile.good())
	{
		float dummy = 0.f;
		char data[64];
		modelFile >> header;
		
		if (strcmp(header, "v")==0)
		{
			//printf("Adding Points:");
			for (int i=0;i<3;++i)
			{
				modelFile >> data;
				points->push_back(atof(data));
				//printf(" %f", dummy);
			}
			//printf("\n");
			
		}
		else if (strcmp(header, "vt")==0)
		{
			//printf("Adding UVs:");
			for (int i=0;i<2;++i)
			{
				modelFile >> data;
				uv->push_back(atof(data));
				//printf(" %f", dummy);
			}
			//printf("\n");
		}
		else if (strcmp(header, "vn")==0) 
		{
			//printf("Adding Normals:");
			for (int i=0;i<3;++i)
			{
				modelFile >> data;
				tempNormals->push_back(atof(data));
				//printf(" %f", dummy);
			}
			//printf("\n");
		}
		else if (strcmp(header, "f")==0)
		{
			//printf("Adding Indices:");
			int n=-1;
			for (int i=0;i<3;++i)
			{
				char read[64];
				char tri[64];
				char norm[64];
				for (int j=0;j<64;++j)
				{
					read[j] = 0;
					norm[j] = 0;
					tri[j] = 0;
				}
				modelFile >> read;
				//printf("%s\n", read);
				int j=0;
				for (j=0;j<64;++j)
				{
					if (read[j] == '/' && read[j+1] == '/')
					{
						j+=2;
						break;
					}
					tri[j] = read[j];
				}
				int k=0;
				for (j;j<64;++j)
				{
					if (read[j] == ' ' || read[j] == '\n')
						break;
					norm[k] = read[j];
					++k;
				}
				int bummy = atoi(tri);
				n = atoi(norm);
				//printf("%s %i\n", norm, n);
				assert(n!=0);
				//printf(" %i", bummy);
				assert(bummy-1>-1);
				assert(bummy<points->size());
				indices->push_back(bummy-1);
				//normals->push_back((*tempNormals)[n]);
				//normals->push_back((*tempNormals)[n+1]);
			}
			normals->push_back((*tempNormals)[(n-1)*3]);
			normals->push_back((*tempNormals)[(n-1)*3+1]);
			normals->push_back((*tempNormals)[(n-1)*3+2]);
			//printf("\n");
		}
	}
	modelFile.close();

	if (0)
	{
		std::ofstream check;
		check.open("check.txt", std::ios::out);
		if (check.bad())
			return false;

		for (int i=0;i<points->size();i+=3)
		{
			check << "v " << (*points)[i] << " " << (*points)[i+1] << " " << (*points)[i+2] << std::endl;
		}
		for (int i=0;i<normals->size();i+=3)
		{
			check << "vn " << (*normals)[i] << " " << (*normals)[i+1] << " " << (*normals)[i+2] << std::endl;
		}
		for (int i=0;i<indices->size();i+=3)
		{
			check << "f " << (*indices)[i] << " " << (*indices)[i+1] << " " << (*indices)[i+2] << std::endl;
		}
		check.close();
	}
	delete tempNormals;
	return true;
}