#include "Inlines.h"
#include "Shape.h"
#include "Sphere.h"
#include "Light.h"
#include "SphereLight.h"
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
	color1.Color.red = 255; color1.Color.green = 0; color1.Color.blue = 0;
	color1.Specular = 0.f;
	color1.Rough = 0.f;
	//color1.Specular.red = 0; color1.Specular.green = 255; color1.Specular.blue = 255;
	//color1.Ambient.red = 0; color1.Ambient.green = 0; color1.Ambient.blue = 0;

	Material color2;
	color2.Color.red = 0; color2.Color.green = 255; color2.Color.blue = 0;
	color2.Specular = 0.0f;
	color2.Rough = 0.0f;
	//color2.Specular.red = 255; color2.Specular.green = 0; color2.Specular.blue = 255;
	//color2.Ambient.red = 0; color2.Ambient.green = 0; color2.Ambient.blue = 0;

	Material color3;
	color3.Color.red = 255; color3.Color.green = 255; color3.Color.blue = 255;
	color3.Specular = 0.f;
	color3.Rough = 0.f;
	//color3.Specular.red = 55; color3.Specular.green = 55; color3.Specular.blue = 255;
	//color3.Ambient.red = 30; color3.Ambient.green = 30; color3.Ambient.blue = 30;

	Material color4;
	color4.Color.red = 0; color4.Color.green = 0; color4.Color.blue = 255;
	color4.Specular = 0.f;
	color4.Rough = 0.f;
	//color4.Specular.red = 0; color4.Specular.green = 255; color4.Specular.blue = 255;
	//color4.Ambient.red = 0; color4.Ambient.green = 0; color4.Ambient.blue = 0;
	
	Material color5;
	color5.Color.red = 250; color5.Color.green = 250; color5.Color.blue = 250;
	color5.Specular = 0.f;
	color5.Rough = 0.f;

	Material color6;
	color6.Color.red = 0; color6.Color.green = 0; color6.Color.blue = 0;
	color6.Specular = 1.f;
	color6.Rough = 0.f;

	RGB bg; bg.red = 166; bg.green = 166; bg.blue = 166;
	Transform sph1T = Translate(Vector(75.f, 0.f, -500.f));
	Transform sph2T = Translate(Vector(-100.f, 0.f, -500.f));
	Transform sph3T = Translate(Vector(0.f, 15150.f, 0.f));
	Transform sph4T = Translate(Vector(0.f, -15150.f, 0.f));
	Transform sph5T = Translate(Vector(0.f, 0.f, 15800.f));
	Transform sph6T = Translate(Vector(0.f, 0.f, -15800.f));
	Transform sph7T = Translate(Vector(15200.f, 0.f, 0.f));
	Transform sph8T = Translate(Vector(-15200.f, 0.f, 0.f));
	Transform sph9T = Translate(Vector(-15.f, -50.f, -410.f));
	Transform sph10T = Translate(Vector(0.f, 50.f, -600.f));
	Transform sph11T = Translate(Vector(-20.f, 100.f, -600.f));

	Shape * sphere =  new Sphere(&sph1T, color6, 75.f);
	Shape * sphere2 = new Sphere(&sph2T, color2, 75.f);
	Shape * sphere3 = new Sphere(&sph3T, color5, 15000.f);
	Shape * sphere4 = new Sphere(&sph4T, color5, 15000.f);
	Shape * sphere5 = new Sphere(&sph5T, color5, 15000.f);
	Shape * sphere6 = new Sphere(&sph6T, color5, 15000.f);
	Shape * sphere7 = new Sphere(&sph7T, color1, 15000.f);
	Shape * sphere8 = new Sphere(&sph8T, color4, 15000.f);
	Shape * sphere9 = new Sphere(&sph9T, color2, 20.f);
	Shape * sphere10 = new Sphere(&sph10T, color5, 20.f);
	Shape * sphere11 = new Sphere(&sph11T, color5, 20.f);
	
	shapeList.push_back(sphere);
	shapeList.push_back(sphere2);
	shapeList.push_back(sphere3);
	//shapeList.push_back(sphere4);
	shapeList.push_back(sphere5);
	shapeList.push_back(sphere6);
	shapeList.push_back(sphere7);
	shapeList.push_back(sphere8);
	shapeList.push_back(sphere9);
	shapeList.push_back(sphere10);
	shapeList.push_back(sphere11);


	RGB white; white.red = 255; white.green = 255; white.blue = 255;


	Transform lightT1 = Translate(Vector(0.f, -15300.f, 0.f));
	Light * light1 =  new SphereLight(&lightT1, white, 100.f, 15000.f);
	lightList.push_back(light1);
	//lightList.push_back(&light2);
	//lightList.push_back(&light3);


	Transform camTrans = Translate(Vector(0.f, 0.f, 0.f));
	Camera camera(camTrans, 500, 500, 500);
	Renderer renderer(&shapeList, &lightList, camera);
	renderer.Render();
	//debugTrace();
	return 0;
}

