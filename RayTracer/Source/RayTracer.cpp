#include "Inlines.h"
#include "Shape.h"
#include "Sphere.h"
#include "Light.h"
#include <vector>
#include "Renderer.h"
#include <fstream>
#include <algorithm>

//FIXME: Doesn't seem to work LOL
bool writeToTGA(RGB * pixels, int height, int width)
{
	std::ofstream outFile("Image.tga", std::ios::binary);
	if (!outFile) return false;

	unsigned char header[18] = { 0 };
	header[2] = 1;
	header[12] = width & 0xFF;
	header[13] = (width >> 8) & 0xFF;
	header[14] = height & 0xFF;
	header[15] = (height >> 8) & 0xFF;
	header[16] = 24;

	outFile.write((const char*)header, 18);

	for (int y = height - 1; y >= 0; --y)
	{
		for (int x = 0; x < width; ++x)
		{
			outFile.put((char)pixels[(y*width) + x].blue);
			outFile.put((char)pixels[(y*width) + x].green);
			outFile.put((char)pixels[(y*width) + x].red);
		}
	}
	outFile.close();
	return true;
}

int main(int argc, char * argv[])
{	
	std::vector<Shape*> shapeList;
	std::vector<Light*> lightList;

	Material color1;
	color1.Diffuse.red = 255; color1.Diffuse.green = 0; color1.Diffuse.blue = 0;
	color1.Specular.red = 0; color1.Specular.green = 255; color1.Specular.blue = 255;
	color1.Ambient.red = 0; color1.Ambient.green = 0; color1.Ambient.blue = 0;

	Material color2;
	color2.Diffuse.red = 0; color2.Diffuse.green = 255; color2.Diffuse.blue = 0;
	color2.Specular.red = 255; color2.Specular.green = 0; color2.Specular.blue = 255;
	color2.Ambient.red = 0; color2.Ambient.green = 0; color2.Ambient.blue = 0;

	Material color3;
	color3.Diffuse.red = 200; color3.Diffuse.green = 200; color3.Diffuse.blue = 0;
	color3.Specular.red = 55; color3.Specular.green = 55; color3.Specular.blue = 255;
	color3.Ambient.red = 30; color3.Ambient.green = 30; color3.Ambient.blue = 30;

	Material color4;
	color4.Diffuse.red = 255; color4.Diffuse.green = 0; color4.Diffuse.blue = 0;
	color4.Specular.red = 0; color4.Specular.green = 255; color4.Specular.blue = 255;
	color4.Ambient.red = 0; color4.Ambient.green = 0; color4.Ambient.blue = 0;
	
	RGB bg; bg.red = 166; bg.green = 166; bg.blue = 166;
	Transform sph1T = Translate(Vector(75.f, 0.f, -500.f));
	Transform sph2T = Translate(Vector(-100.f, 0.f, -500.f));
	Transform sph3T = Scale(1.f, 10.f, 1.f)(Translate(Vector(0.f, 2000.f, -1000.f)));
	Transform sph4T = Scale(1.f, 1.f, 100.f)(Translate(Vector(0.f, 0, -4000.f)));
	Transform sph5T = Translate(Vector(-50.f, -0.f, -410.f));
	Transform sph6T = Translate(Vector(-50.f, 50.f, -410.f));
	Transform sph7T = Translate(Vector(0.f, 0.f, -410.f));
	Transform sph8T = Scale(1.f, 1.f, 1.f)(Translate(Vector(-300.f, -1.f, 500.f)));
	Transform sph9T = Translate(Vector(-15.f, -50.f, -410.f));
	Transform sph10T = Translate(Vector(0.f, 50.f, -600.f));
	Transform sph11T = Translate(Vector(-20.f, 100.f, -600.f));
	Transform sph12T = Translate(Vector(20.f, 150.f, -410.f));

	Shape * sphere =  new Sphere(&sph1T, color4, 75.f);
	Shape * sphere2 = new Sphere(&sph2T, color2, 75.f);
	Shape * sphere3 = new Sphere(&sph3T, color3, 10000.f);
	Shape * sphere4 = new Sphere(&sph4T, color3, 10000.f);
	Shape * sphere5 = new Sphere(&sph5T, color4, 20.f);
	Shape * sphere6 = new Sphere(&sph6T, color4, 20.f);
	Shape * sphere7 = new Sphere(&sph7T, color4, 20.f);
	Shape * sphere8 = new Sphere(&sph8T, color3, 20.f);
	Shape * sphere9 = new Sphere(&sph9T, color2, 20.f);
	Shape * sphere10 = new Sphere(&sph10T, color4, 20.f);
	Shape * sphere11 = new Sphere(&sph11T, color4, 20.f);
	Shape * sphere12 = new Sphere(&sph12T, color4, 20.f);
	
	shapeList.push_back(sphere);
	shapeList.push_back(sphere2);
	shapeList.push_back(sphere3);
	//shapeList.push_back(sphere4);
	shapeList.push_back(sphere5);
	shapeList.push_back(sphere6);
	shapeList.push_back(sphere7);
	//shapeList.push_back(sphere8);
	shapeList.push_back(sphere9);
	shapeList.push_back(sphere10);
	shapeList.push_back(sphere12);
	shapeList.push_back(sphere11);

	Light light1(Point(-230.f, 200.f, -400.f), color3);
	lightList.push_back(&light1);


	Transform camTrans = Translate(Vector(0.f, 0.f, 0.f));
	Camera camera(camTrans, 2600, 2600, 2600);
	Renderer renderer(&shapeList, &lightList, camera);
	renderer.Render();
	//debugTrace();
	return 0;
}

