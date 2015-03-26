#define NDEBUG

#include "Inlines.h"
#include "Shape.h"
#include "Sphere.h"
#include "TriangleMesh.h"
#include <vector>
#include "Renderer.h"
#include <algorithm>
#include "ObjLoader.h"
#include "QualityDesc.h"


int main(int argc, char * argv[])
{	
	printf("\n\n");
	std::vector<Primitive*> *scene = new std::vector<Primitive*>();
	
	Material whiteWall;
	whiteWall.Color.red = 1.f; whiteWall.Color.green = 1.f; whiteWall.Color.blue = 1.0f;
	whiteWall.Specular = 0.0f;
	whiteWall.Diffuse = 0.3f;
	whiteWall.GlossyReflective = 0.23f;
	whiteWall.Reflective = 0.6f;
	whiteWall.Refractive = 1.f;
	whiteWall.RefrAbsorbance = 1.f;

	Material blueWall;
	blueWall.Color.red = 0.4f; blueWall.Color.green = 0.4f; blueWall.Color.blue = 1.0f;
	blueWall.Specular = 0.0f;
	blueWall.Diffuse = 0.6f;
	blueWall.GlossyReflective = 0.f;
	blueWall.Reflective = 0.0f;
	blueWall.Refractive = 1.f;
	blueWall.RefrAbsorbance = 1.f;

	Material redWall;
	redWall.Color.red = 1.f; redWall.Color.green = 0.4f; redWall.Color.blue = .4f;
	redWall.Specular = 0.0f;
	redWall.Diffuse = 0.5f;
	redWall.GlossyReflective = 0.f;
	redWall.Reflective = 0.0f;
	redWall.Refractive = 1.f;
	redWall.RefrAbsorbance = 1.f;

	Material color2;
	color2.Color.red = 0.7f; color2.Color.green = 1.f; color2.Color.blue = 0.7f;
	color2.Specular = 1.0f;
	color2.Diffuse = 0.2f;
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
	color6.Color.red = 1.f; color6.Color.green = 1.f; color6.Color.blue = 1.f;
	color6.Specular = 1.0f;
	color6.Diffuse = 0.2f;
	color6.GlossyReflective = 0.f;
	color6.Reflective = 1.f;
	color6.Refractive = 1.f;
	color6.RefrAbsorbance = 1.f;

	Material color7;
	color7.Color.red = 1.0f; color7.Color.green = 1.f; color7.Color.blue = 0.0f;
	color7.Specular = 1.f;
	color7.Diffuse = 0.8f;
	color7.GlossyReflective = 0.f;
	color7.Reflective = 0.f;
	color7.Refractive = 1.f;
	color7.RefrAbsorbance = 1.f;

	Material dragonColor;
	dragonColor.Color.red = 1.f; dragonColor.Color.green = 1.f; dragonColor.Color.blue = 1.0f;
	dragonColor.Specular = 0.f;
	dragonColor.Diffuse = 0.4f;
	dragonColor.GlossyReflective = 0.08f;
	dragonColor.Reflective = 0.8f;
	dragonColor.Refractive = 1.f;
	dragonColor.RefrAbsorbance = 1.f;

	Transform *sph1T = new Transform(Translate(Vector(-1.6f, 2.7f, 3.f)));
	Transform *sph2T = new Transform(Translate(Vector(1.7f, 1.3f, 4.f)));
	Transform *sph3T = new Transform(Translate(Vector(0.f, -153.0f, 0.f)));
	Transform *sph4T = new Transform(Translate(Vector(0.f, 156.0f, 0.f)));
	Transform *sph5T = new Transform(Translate(Vector(0.f, 0.f, -157.f)));
	Transform *sph6T = new Transform(Translate(Vector(0.f, 0.f, 157.5f)));
	Transform *sph7T = new Transform(Translate(Vector(-154.5f, 0.f, 0.f)));
	Transform *sph8T = new Transform(Translate(Vector(154.5f, 0.f, 0.f)));
	Transform *sph9T = new Transform(Translate(Vector(0.f, -0.4f, 3.9f)));
	Transform *drag = new Transform((Translate(Vector(0.f, -2.f, 3.5f)))
									(RotateY(-45))
									(RotateX(-90))
									(Scale(0.6f, 0.6f, 0.6f)));

	Transform* Invsph1T = new Transform(Inverse(*sph1T));
	Transform* Invsph2T = new Transform(Inverse(*sph2T));
	Transform* Invsph3T = new Transform(Inverse(*sph3T));
	Transform* Invsph4T = new Transform(Inverse(*sph4T));
	Transform* Invsph5T = new Transform(Inverse(*sph5T));
	Transform* Invsph6T = new Transform(Inverse(*sph6T));
	Transform* Invsph7T = new Transform(Inverse(*sph7T));
	Transform* Invsph8T = new Transform(Inverse(*sph8T));
	Transform* Invsph9T = new Transform(Inverse(*sph9T));
	Transform* Invdrag = new Transform(Inverse(*drag));

	Primitive * sphere =  new Sphere(sph1T, Invsph1T, color6, .6f);
	Primitive * sphere2 = new Sphere(sph2T, Invsph2T, dragonColor, .6f);
	Primitive * sphere3 = new Sphere(sph3T, Invsph3T, blueWall, 151.f);
	Primitive * sphere4 = new Sphere(sph4T, Invsph4T, blueWall, 151.f);
	Primitive * sphere5 = new Sphere(sph5T, Invsph5T, whiteWall, 151.f);
	Primitive * sphere6 = new Sphere(sph6T, Invsph6T, whiteWall, 151.f);
	Primitive * sphere7 = new Sphere(sph7T, Invsph7T, redWall, 151.f);
	Primitive * sphere8 = new Sphere(sph8T, Invsph8T, redWall, 151.f);
	Primitive * sphere9 = new Sphere(sph9T, Invsph9T, color3, .6f);
	
	ObjLoader loader;
	Primitive * dragon = new TriangleMesh(loader.Construct("dragon.obj", drag, Invdrag, dragonColor));
	
	scene->push_back(dragon);

	//scene->push_back(sphere);
	//scene->push_back(sphere2);
	scene->push_back(sphere3);
	scene->push_back(sphere4);
	scene->push_back(sphere5);
	scene->push_back(sphere6); //+Z
	scene->push_back(sphere7);
	scene->push_back(sphere8);
	//scene->push_back(sphere9);
	//scene->push_back(sphere10);
	//scene.push_back(sphere11);
	
	std::vector<void*> smallSphereData;
	if (0)
	for (int i=0;i<8;++i)
	{
		for (int j=0;j<8;++j)
		{
			Transform * T = new Transform(Translate(Vector((j-3.5f)*0.5f, (i-3.5f)*0.5f, 5.7f)));
			Transform * InvT = new Transform(Inverse(*T));
			Primitive * sp =  new Sphere(T, InvT, color7, 0.15f);
			scene->push_back(sp);
			smallSphereData.push_back(T);
			smallSphereData.push_back(InvT);
			smallSphereData.push_back(sp);
		}
	}

	RGB white; white.red = 1.f; white.green = 1.f; white.blue = 1.f;
	Material lightMat; lightMat.Color = white; 
	lightMat.Emissive = 350.f;

	Material lightMat2; lightMat2.Color = white; 
	lightMat2.Emissive = 37.f;
	
	Transform* lightT1 = new Transform(Translate(Vector(0.0f, 2.8f, -3.5f)));
	Transform* lightT2 = new Transform(Translate(Vector(-1.7f, -0.5f, -3.5f)));
	Transform* lightT3 = new Transform(Translate(Vector(1.4f, -0.7f, -.7f)));
	Transform* InvlightT1 = new Transform(Inverse(*lightT1));
	Transform* InvlightT2 = new Transform(Inverse(*lightT2));
	Primitive * light1 = new Sphere(lightT1, InvlightT1, lightMat, 0.15f);//0.6f);
	Primitive * light2 = new Sphere(lightT2, InvlightT2, lightMat2, 0.15f);//0.6f);
	scene->push_back(light1);
	scene->push_back(light2);

	Transform camTrans = Translate(Vector(0.f, 0.7f, -4.f));
	QualityDesc quality;
	quality.AliasSamples = 4;
	quality.LightSamples = 2;
	quality.GlossyReflectiveSamples = 1;
	quality.Depth = 1;
	float dim = 1000.f;
	Camera camera(camTrans, dim, dim, dim);
	Renderer renderer(scene, camera, quality);
	renderer.Render();

	delete scene;
	delete light1, light2;
	delete lightT1, lightT2;
	delete InvlightT1, InvlightT2;
	delete sphere, sphere2, sphere3, sphere4, sphere5, sphere6, sphere7, sphere8, sphere9;
	delete sph1T, sph2T, sph3T, sph4T, sph5T, sph6T, sph7T, sph8T, sph9T;
	delete Invsph1T, Invsph2T, Invsph3T, Invsph4T, Invsph5T, Invsph6T, Invsph7T, Invsph8T, Invsph9T;
	for (int i=0;i<smallSphereData.size();++i)
	{
		delete smallSphereData[i];
	}
	return 0;
}