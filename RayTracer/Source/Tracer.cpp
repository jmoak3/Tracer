#include "Inlines.h"
#include "Shape.h"
#include "Sphere.h"
#include "TriangleMesh.h"
#include <vector>
#include "PathRenderer.h"
#include "RayRenderer.h"
#include <algorithm>
#include "ObjLoader.h"
#include "QualityDesc.h"
#include "Scripting.h"


void addTrisToScene(std::vector<Primitive*> *scene, TriangleMesh* mesh)
{
	for (int i=0;i<mesh->NumTris;++i)
	{
		Primitive *Tri = new Triangle(
			mesh->ObjectToWorld, mesh->WorldToObject, 
			mesh->Mat, mesh, i);
		scene->push_back(Tri);
	}
}

void addSphereTris(std::vector<Primitive*> *scene, Material mat, float r, float x, float y, float z, float a=1.f, float b=1.f, float c=1.f)
{
	Transform *T = new Transform((Translate(Vector(x, y, z)))(Scale(r,r,r)));
	Transform* InvT = new Transform(Inverse(*T));

	ObjLoader loader;
	TriangleMesh * sp = new TriangleMesh(loader.Construct("sphere.obj", T, InvT, mat));
	addTrisToScene(scene, sp);
}

void addSphere(std::vector<Primitive*> *scene, Material mat, float r, float x, float y, float z, float a=1.f, float b=1.f, float c=1.f)
{
	Transform *T = new Transform((Translate(Vector(x, y, z)))(Scale(a,b,c)));
	Transform* InvT = new Transform(Inverse(*T));
	scene->push_back(new Sphere(T, InvT, mat, r));
}

void addBoxTris(std::vector<Primitive*> *scene, Material mat, float r, float x, float y, float z)
{
	ObjLoader loader;
	Transform *T = new Transform((Translate(Vector(x, y, z)))(Scale(r,r,r)));
	Transform* InvT = new Transform(Inverse(*T));
	TriangleMesh * box = new TriangleMesh(loader.Construct("box.obj", T, InvT, mat));
	addTrisToScene(scene, box);
}

void addDrag(std::vector<Primitive*> *scene, Material mat, Transform * T)
{
	ObjLoader loader;
	Transform* InvT = new Transform(Inverse(*T));
	TriangleMesh * drag = new TriangleMesh(loader.Construct("dragon.obj", T, InvT, mat));
	addTrisToScene(scene, drag);
}


int main(int argc, char * argv[])
{
	std::string err;
	Renderer* renderer = init_renderer_from_script("..\\ray.lua", &err);
	if (renderer == NULL)
	{
		printf(err.c_str());
		printf("\n");
	}
	else
	{
		renderer->Render();
	}

	return 0;
}

int stuff()
{
	printf("\n\n");
	std::vector<Primitive*> *scene = new std::vector<Primitive*>();
	
	RGB white; white.red = 1.f; white.green = 1.f; white.blue = 1.f;
	RGB grey; grey.red = 0.6f; grey.green = 0.6f; grey.blue = 0.6f;
	
	float dominant = 0.85f;
	float sub = 0.4f;

	Material whiteWall;
	//whiteWall.Color = white;
	whiteWall.Color.red = dominant; whiteWall.Color.green = dominant; whiteWall.Color.blue = dominant;
	whiteWall.Specular = 0.0f;
	whiteWall.Diffuse = 1.f;
	whiteWall.GlossyReflective = 1.f;
	whiteWall.Reflective = 1.f;
	whiteWall.Refractive = 1.f;
	whiteWall.RefrAbsorbance = 1.f;

	Material greyWall;
	greyWall.Color = grey;
	//greyWall.Color.red = dominant; greyWall.Color.green = dominant; greyWall.Color.blue = dominant;
	greyWall.Specular = 0.0f;
	greyWall.Diffuse = 1.f;
	greyWall.GlossyReflective = 1.f;
	greyWall.Reflective = 1.f;
	greyWall.Refractive = 1.f;
	greyWall.RefrAbsorbance = 1.f;

	Material blueWall;
	blueWall.Color.red = sub; blueWall.Color.green = sub; blueWall.Color.blue = dominant;
	blueWall.Specular = 0.0f;
	blueWall.Diffuse = 1.f;
	blueWall.GlossyReflective = 1.f;
	blueWall.Reflective = 1.f;
	blueWall.Refractive = 1.f;
	blueWall.RefrAbsorbance = 1.f;

	Material redWall;
	redWall.Color.red = dominant; redWall.Color.green = sub; redWall.Color.blue = sub;
	redWall.Specular = 0.0f;
	redWall.Diffuse = 1.f;
	redWall.GlossyReflective = 1.f;
	redWall.Reflective = 1.f;
	redWall.Refractive = 1.f;
	redWall.RefrAbsorbance = 1.f;

	Material color2;
	color2.Color.red = 0.7f; color2.Color.green = 1.f; color2.Color.blue = 0.7f;
	color2.Specular = 1.0f;
	color2.Diffuse = 0.8f;
	color2.GlossyReflective = 0.0f;
	color2.Reflective = 1.f;
	color2.Refractive = 1.f;
	color2.RefrAbsorbance = 1.f;

	Material color3;
	color3.Color.red = 1.0f; color3.Color.green = 1.0f; color3.Color.blue = 1.0f;
	color3.Specular = 1.0f;
	color3.Diffuse = 0.1f;
	color3.GlossyReflective = 0.f;
	color3.Reflective = 1.0f;
	color3.Refractive = 1.05f;
	color3.RefrAbsorbance = 0.15f;
	
	Material color4;
	color4.Color.red = 0.f; color4.Color.green = 0.f; color4.Color.blue = 0.0f;
	color4.Specular = 0.f;
	color4.Diffuse = 0.f;
	color4.GlossyReflective = 0.0f;
	color4.Reflective = 0.0f;
	color4.Refractive = 1.f;
	color4.RefrAbsorbance = 0.f;

	Material color6;
	color6.Color.red = sub; color6.Color.green = dominant; color6.Color.blue = sub;
	color6.Specular = 0.8f;
	color6.Diffuse = 1.f;
	color6.GlossyReflective = 0.f;
	color6.Reflective = 1.f;
	color6.Refractive = 1.f;
	color6.RefrAbsorbance = 1.f;

	Material color7;
	color7.Color.red = 1.0f; color7.Color.green = 1.f; color7.Color.blue = 0.0f;
	color7.Specular = .0f;
	color7.Diffuse = 1.f;
	color7.GlossyReflective = 0.f;
	color7.Reflective = 0.f;
	color7.Refractive = 1.f;
	color7.RefrAbsorbance = 1.f;

	Material dragonColor;
	dragonColor.Color.red = sub; dragonColor.Color.green = dominant; dragonColor.Color.blue = sub;
	dragonColor.Specular = 0.8f;
	dragonColor.Diffuse = 1.f;
	dragonColor.GlossyReflective = 1.f;
	dragonColor.Reflective = 1.f;
	dragonColor.Refractive = 1.f;
	dragonColor.RefrAbsorbance = 1.f;

	addSphere(scene, whiteWall, 160.f, 0.f, 163.f, 0.f);// Y = 6 units
	addSphere(scene, whiteWall, 160.f, 0.f, -163.f, 0.f);// Y = 6 units
	addSphere(scene, whiteWall, 160.f, 0.f, 0.f, -170.f);// Z = 13 units
	addSphere(scene, whiteWall, 160.f, 0.f, 0.f, 163.f); // X = 6 units
	addSphere(scene, blueWall, 160.f, -163.f, 0.f, 0.f);
	addSphere(scene, redWall, 160.f, 163.f, 0.f, 0.f);

	addSphere(scene, color6, 0.8f, 1.0f, -2.2f, -2.f);
	std::vector<void*> smallSphereData;
	if (1)
	for (int i=0;i<8;++i)
	{
		for (int j=0;j<8;++j)
		{
			//addSphere(scene, color7, 0.25f, (j-3.5f)*0.5f, (i-3.5f)*0.5f, 2.9f);
		}
	}
	
	Transform *drag = new Transform((Translate(Vector(0.f, -2.8f, 1.f)))
									(RotateY(-45))
									(RotateX(-90))
									(Scale(0.6f, 0.6f, 0.6f)));	
	
	//addDrag(scene, dragonColor, drag);

	white *= 1.f;
	Material lightMat; lightMat.Color = white; 
	lightMat.Emissive = 3.f;

	addSphere(scene, lightMat, 1.f, 0.f, 1.9f, 1.f);

	Transform camTrans = (Translate(Vector(0.f, -1.0f, -8.f)));

	QualityDesc quality;

	//RayTracer-only Qualities
	quality.LightSamples = 1;
	quality.GlossyReflectiveSamples = 1;
	quality.Depth = 1;
	
	quality.Samples = 1;
	quality.PathEnableDirectLighting = true;
	quality.PathEnableIndirectIllum = true;
	float dim = 512;
	Camera camera(camTrans, dim, dim, dim);
	Renderer* renderer = new PathRenderer(scene, camera, quality);
	renderer->Render();

	delete scene;
	return 0;
}
