#include "Inlines.h"
#include "Shape.h"
#include "Sphere.h"
#include "Light.h"
#include <vector>
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
	//Will be rewritten!!!!!!!!!! 
	//NEED TRACING OBJECT!!!!
	const int height = 1000.f;
	const int width = 1000.f;

	std::vector<Shape*> shapeList;
	std::vector<Shape*>::iterator iShape;
	std::vector<Light*> lightList;
	std::vector<Light*>::iterator iLight;

	RGB color; color.red = 255; color.green = 0.f; color.blue = 0;
	RGB color2; color2.red = 0; color2.green = 255; color2.blue = 0;
	RGB bg; bg.red = 166; bg.green = 166; bg.blue = 166;
	Transform sph1T = Translate(Vector(100.f, 0.f, -500.f));
	Transform sph1InvT = Translate(Vector(-100.f, 0.f, 500.f));
	Transform sph2T = Translate(Vector(-100.f, 0.f, -500.f));
	Transform sph2InvT = Translate(Vector(100.f, 0.f, 500.f));
	Shape * sphere = new Sphere(&sph1T, &sph1InvT, color, false, 75.f);
	Shape * sphere2 = new Sphere(&sph2T, &sph2InvT, color2, false, 75.f);
	
	shapeList.push_back(sphere);
	shapeList.push_back(sphere2);

	RGB color3; color3.red = 255; color3.green = 255; color3.blue = 255;
	Light light1(Point(0.f, 100.f, 300.f), color3);
	lightList.push_back(&light1);
	
	//Camera cam;

	printf("P6 %i %i 255 ", height, width);
	
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			RGB pixelColor = bg;
			float wid2 = ((float)width / 2.f);
			float hei2 = ((float)height / 2.f);
			float x1 = (2.f*x/(float)width - 1.f);
			float y1 = (2.f*-y/(float)height + 1.f);
			//float x1 = (2.f * ((x + 0.5f) / width) - 1) * cam.Angle;
			//float y1 = (1.f - 2.f * ((y + 0.5f) / height)) * cam.Angle;
			float x2 = x1*width/2.f;
			float y2 = y1*height/2.f;
			Point p1 = Point(x1, y1, 0.f);
			Point p2 = Point(x2, y2, height);

			Vector dir = p2 - p1;
			Ray ray = Ray(p1, dir, 0.f);
			Ray reflRay = ray;
			Normal normal;
			Normal dummy;
			float minHit = INFINITY;
			float currHit = 0.f;
			float eps = 0.f;
			
			bool hit = false;
			bool reflective = false;
			for (iShape = shapeList.begin(); iShape!=shapeList.end(); ++iShape)
			{
				Ray rRay;
				if ((*iShape)->Intersect(ray, &currHit, &eps, &rRay, &dummy) && currHit < minHit)
				{
					pixelColor = (*iShape)->GetColor();
					hit = true;
					minHit = currHit;
					reflRay = rRay;
					normal = dummy;
					reflective = (*iShape)->Reflective;
				}
			}

			if (hit && reflective)
			{
				while (ray.depth < 5)
				{
					hit = false;
					for (iShape = shapeList.begin(); iShape!=shapeList.end(); ++iShape)
					{
						Ray rRay;
						if ((*iShape)->Intersect(ray, &currHit, &eps, &rRay, &normal) && currHit < minHit)
						{
							pixelColor = (*iShape)->GetColor();
							hit = true;
							minHit = currHit;
							reflRay = rRay;
						}
					}
					if (hit)
					{
						ray.o = reflRay.o;
						ray.d = reflRay.d;
						++ray.depth;
					}
					else
						ray.depth = INT_MAX;
				}
			}
			else if (hit)
			{
				minHit = INFINITY;
				float dummyHit = 0.f;
				hit = false;
				for (iShape = shapeList.begin(); iShape!=shapeList.end(); ++iShape)
				{
					Ray rRay;
					if ((*iShape)->Intersect(reflRay, &dummyHit, &eps, &rRay, &dummy) && currHit < minHit)
					{
						/*pixelColor = (*iShape)->GetColor();
						pixelColor.blue *= .2f;
						pixelColor.green*= .2f;
						pixelColor.red  *= .2f;
						hit = true;
						minHit = dummyHit;*/
					}
				}
				if (!hit)
				{
					for (iLight = lightList.begin(); iLight!=lightList.end(); ++iLight)
					{
						pixelColor = computeColor(normal, ray, reflRay, (*iLight)->Position, pixelColor, currHit);
					}
				}
			}
			else
			{
				pixelColor = bg;
			}
							
			if (pixelColor.red == 0 || pixelColor.green == 0)
				bool fsda = false;
			printf("%c%c%c", pixelColor.red, pixelColor.green, pixelColor.blue);
		}
	}

	//assert(writeToTGA((RGB*)pixels, height, width));
	return 0;
}