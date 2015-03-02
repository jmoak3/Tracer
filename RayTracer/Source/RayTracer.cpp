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

//ray -> sphere -> ray ->

RGB computeColor(const Normal &normal, const Ray &ray, const Ray &reflRay, const Point &lightPos, const RGB & color, const float currHit) 
{
	float spec = 0.1f*std::max(0.f, Dot(reflRay.d, Vector(reflRay.o - Point(0.f, 0.f, 0.f))));
	RGB specular;
	specular.red = spec;
	specular.green = spec;
	specular.blue = spec;
	
	RGB pixelColor = color;
	Vector l = Normalize(lightPos - reflRay.o);
	float diff = std::min(1.0f, std::max(0.f, Dot(Normalize(normal), l)));
	RGB diffuse;
	diffuse.red = (int)((float)pixelColor.red*diff);
	diffuse.green = (int)((float)pixelColor.green*diff);
	diffuse.blue = (int)((float)pixelColor.blue*diff);

	pixelColor.red  = diffuse.red;// + specular.red;
	pixelColor.green= diffuse.green;// + specular.green;
	pixelColor.blue = diffuse.blue;// + specular.blue;

	pixelColor.red = std::min(pixelColor.red, 255);
	pixelColor.green = std::min(pixelColor.green, 255);
	pixelColor.blue = std::min(pixelColor.blue, 255);

	pixelColor.red = std::max(pixelColor.red, 20);
	pixelColor.green = std::max(pixelColor.green, 20);
	pixelColor.blue = std::max(pixelColor.blue, 20);

	return pixelColor;
}

int main(int argc, char * argv[])
{	
	std::vector<Shape*> shapeList;
	std::vector<Light*> lightList;

	Colors color1;
	color1.Diffuse.red = 255; color1.Diffuse.green = 0; color1.Diffuse.blue = 0;
	color1.Specular.red = 0; color1.Specular.green = 255; color1.Specular.blue = 255;
	color1.Ambient.red = 0; color1.Ambient.green = 0; color1.Ambient.blue = 0;

	Colors color2;
	color2.Diffuse.red = 0; color2.Diffuse.green = 0; color2.Diffuse.blue = 0;
	color2.Specular.red = 255; color2.Specular.green = 255; color2.Specular.blue = 255;
	color2.Ambient.red = 0; color2.Ambient.green = 0; color2.Ambient.blue = 0;

	Colors color3;
	color3.Diffuse.red = 255; color3.Diffuse.green = 255; color3.Diffuse.blue = 255;
	color3.Specular.red = 30; color3.Specular.green = 30; color3.Specular.blue = 30;
	color3.Ambient.red = 0; color3.Ambient.green = 0; color3.Ambient.blue = 0;

	Colors color4;
	color4.Diffuse.red = 0; color4.Diffuse.green = 0; color4.Diffuse.blue = 255;
	color4.Specular.red = 255; color4.Specular.green = 255; color4.Specular.blue = 0;
	color4.Ambient.red = 0; color4.Ambient.green = 0; color4.Ambient.blue = 0;
	
	RGB bg; bg.red = 166; bg.green = 166; bg.blue = 166;
	Transform sph1T = Translate(Vector(50.f, 0.f, -500.f));
	Transform sph2T = Translate(Vector(-100.f, 0.f, -500.f));
	Transform sph3T = Scale(1.f, 10.f, 1.f)(Translate(Vector(0.f, 2000.f, -1000.f)));
	Transform sph4T = Translate(Vector(-50.f, -50.f, -410.f));

	Shape * sphere = new Sphere(&sph1T, color2, false, 75.f);
	Shape * sphere2 = new Sphere(&sph2T, color3, false, 75.f);
	Shape * sphere3 = new Sphere(&sph3T, color3, false, 10000.f);
	Shape * sphere4 = new Sphere(&sph4T, color3, false, 20.f);
	
	shapeList.push_back(sphere);
	shapeList.push_back(sphere2);
	shapeList.push_back(sphere3);
	shapeList.push_back(sphere4);

	Light light1(Point(0.f, 75.f, 300.f), color3);
	lightList.push_back(&light1);


	Transform camTrans = Translate(Vector(0.f, 0.f, 0.f));
	Camera camera(camTrans, 500, 500, 500);
	Renderer renderer(&shapeList, &lightList, camera);
	renderer.Render();
	//debugTrace();
	return 0;
}

