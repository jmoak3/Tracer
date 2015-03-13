#include "Inlines.h"
#include "Shape.h"
#include "Sphere.h"
#include <vector>
#include "Renderer.h"
#include <algorithm>

int main(int argc, char * argv[])
{	
	std::vector<Primitive*> scene;

	Material color1;
	color1.Color.red = 1.0f; color1.Color.green = 0; color1.Color.blue = 0;
	color1.Specular = 0.f;
	color1.Diffuse = 1.f;
	color1.DiffuseReflective = 1.f;
	color1.Reflective = 1.f;

	Material color2;
	color2.Color.red = 0.7f; color2.Color.green = 1.f; color2.Color.blue = 0.7f;
	color2.Specular = 1.0f;
	color2.Diffuse = 0.2f;
	color2.DiffuseReflective = 0.000f;
	color2.Reflective = 1.f;

	Material color3;
	color3.Color.red = 0.7f; color3.Color.green = 1.f; color3.Color.blue = 0.7f;
	color3.Specular = 1.0f;
	color3.Diffuse = 0.2f;
	color3.DiffuseReflective = 0.1f;
	color3.Reflective = 1.f;

	Material color6;
	color6.Color.red = 0.7f; color6.Color.green = 1.f; color6.Color.blue = 0.7f;
	color6.Specular = 1.0f;
	color6.Diffuse = 0.2f;
	color6.DiffuseReflective = 0.2f;
	color6.Reflective = 1.f;

	Material color4;
	color4.Color.red = 0; color4.Color.green = 0; color4.Color.blue = 1.0f;
	color4.Specular = 0.f;
	color4.Diffuse = 1.f;
	color4.DiffuseReflective = 1.f;
	color4.Reflective = 1.f;

	Material color5;
	color5.Color.red = 1.0f; color5.Color.green = 1.f; color5.Color.blue = 1.0f;
	color5.Specular = 1.f;
	color5.Diffuse = 0.5f;
	color5.DiffuseReflective = 0.f;
	color5.Reflective = 0.f;

	Transform sph1T = Translate(Vector(-1.6f, -0.4f, 4.f));
	Transform sph2T = Translate(Vector(1.6f, -0.4f, 4.f));
	Transform sph3T = Translate(Vector(0.f, -152.5f, 0.f));
	Transform sph4T = Translate(Vector(0.f, 153.50f, 0.f));
	Transform sph5T = Translate(Vector(0.f, 0.f, -156.f));
	Transform sph6T = Translate(Vector(0.f, 0.f, 156.f));
	Transform sph7T = Translate(Vector(-154.f, 0.f, 0.f));
	Transform sph8T = Translate(Vector(154.f, 0.f, 0.f));
	Transform sph9T = Translate(Vector(0.f, -0.4f, 4.f));
	Transform sph10T = Translate(Vector(0.f, -.50f, 6.f));
	Transform sph11T = Translate(Vector(.20f, -1.f, 6.f));

	Shape * sphere =  new Sphere(&sph1T, color6, .6f);
	Shape * sphere2 = new Sphere(&sph2T, color2, .6f);
	Shape * sphere3 = new Sphere(&sph3T, color5, 151.f);
	Shape * sphere4 = new Sphere(&sph4T, color5, 152.f);
	Shape * sphere5 = new Sphere(&sph5T, color5, 150.f);
	Shape * sphere6 = new Sphere(&sph6T, color5, 150.f);
	Shape * sphere7 = new Sphere(&sph7T, color1, 150.f);
	Shape * sphere8 = new Sphere(&sph8T, color4, 150.f);
	Shape * sphere9 = new Sphere(&sph9T, color3, .6f);
	Shape * sphere10 = new Sphere(&sph10T, color5, .20f);
	Shape * sphere11 = new Sphere(&sph11T, color5, .20f);
	
	scene.push_back(sphere);
	scene.push_back(sphere2);
	scene.push_back(sphere3);
	//scene.push_back(sphere4);
	//scene.push_back(sphere5);
	scene.push_back(sphere6);
	//scene.push_back(sphere7);
	//scene.push_back(sphere8);
	scene.push_back(sphere9);
	//scene.push_back(sphere10);
	//scene.push_back(sphere11);


	RGB white; white.red = 1.f; white.green = 1.f; white.blue = 1.f;
	Material lightMat; lightMat.Color = white; 
	lightMat.Emissive = 350.f;

	Material lightMat2; lightMat2.Color = white; 
	lightMat2.Emissive = 37.f;

	Transform lightT1 = Translate(Vector(0.0f, 3.f, 1.8f));
	Transform lightT2 = Translate(Vector(-1.4f, .7f, 0.2f));

	Primitive * light1 =  new Sphere(&lightT1, lightMat, 0.4f);
	Primitive * light2 =  new Sphere(&lightT2, lightMat2, 0.1f);
	scene.push_back(light1);
	scene.push_back(light2);

	Transform camTrans = Translate(Vector(0.f, 0.f, 0.f));

	float dim = 1000.f;
	Camera camera(camTrans, dim, dim, dim);
	Renderer renderer(&scene, camera);
	renderer.Render();
	return 0;
}

