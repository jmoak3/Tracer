#include "Renderer.h"
#include <math.h>

Renderer::Renderer(std::vector<Shape*>* vshapes, std::vector<Light*>*vlights, const Camera &ccamera)
{
	camera = ccamera;
	shapes = vshapes;
	lights = vlights; 
}

RGB Renderer::computeColor(const Normal &normal, const Ray &reflRay, const Colors & shapeColor, const int lastShape) 
{
	RGB finalColor; finalColor.red = 0; finalColor.blue = 0; finalColor.green = 0;
	std::vector<Light*>::iterator iLight;
	std::vector<Shape*>::iterator iShape;

	for (iLight = lights->begin(); iLight!=lights->end(); ++iLight)
	{	
		Point lightPos = (*iLight)->Position;
		Vector l = Normalize(lightPos - reflRay.o);
		Vector ll = Normalize(reflRay.o - lightPos);

		//Test if this light is blocked!!!

		Ray shadowRay(reflRay.o, l, 0.f);
		Ray garbageRay;
		float currHit = -1.f;
		float minHit = INFINITY;
		float eps;
		Normal garbageNormal;
		bool hit = false;
		for (iShape = shapes->begin(); iShape!=shapes->end(); ++iShape)
		{
			int currID = (*iShape)->ShapeID;
			if ((*iShape)->Intersect(shadowRay, &currHit, &eps, &garbageRay, &garbageNormal) && currHit < minHit && currID != lastShape)
			{
				minHit = currHit;
				hit = true;
			}
		}
		if (hit)
			continue;

		Vector v = Normalize(camera.ScreenToWorld(Point(0.f, 0.f, 0.f))- reflRay.o);

		Vector dir = l;
		
		Vector r = Vector(2.f*Dot(dir, normal)*normal) - dir;

		float spec = pow(std::min(1.0f, std::max(0.f, Dot(r, v))),10.f);
		
		RGB specular;
		specular.red = (int)(shapeColor.Specular.red*spec);
		specular.green = (int)(shapeColor.Specular.green*spec);
		specular.blue = (int)(shapeColor.Specular.blue*spec);

		float diff = std::min(1.0f, std::max(0.f, Dot(normal, l)));
		RGB diffuse;
		diffuse.red = (int)((float)shapeColor.Diffuse.red*diff);
		diffuse.green = (int)((float)shapeColor.Diffuse.green*diff);
		diffuse.blue = (int)((float)shapeColor.Diffuse.blue*diff);

		RGB lightColor;
		lightColor.red  = diffuse.red + specular.red;
		lightColor.green= diffuse.green + specular.green;
		lightColor.blue = diffuse.blue + specular.blue;

		lightColor.red = std::min(lightColor.red, 255);
		lightColor.green = std::min(lightColor.green, 255);
		lightColor.blue = std::min(lightColor.blue, 255);

		lightColor.red = std::max(lightColor.red, 20); //(int)((float)shapeColor.red*0.3f));
		lightColor.green = std::max(lightColor.green, 20); //(int)((float)shapeColor.green*0.2f));
		lightColor.blue = std::max(lightColor.blue, 20); //(int)((float)shapeColor.blue*0.2f));

		finalColor.red += lightColor.red;
		finalColor.green += lightColor.green;
		finalColor.blue += lightColor.blue;
	}
	return finalColor;
}

void Renderer::Render()
{
	//Identify x and y using camera's specs, and make for loop
	const int height = camera.height;
	const int width = camera.width;

	printf("P6 %i %i 255 ", height, width);
	for (int y=0;y<height;++y)
	{
		for (int x=0;x<width;++x)
		{
			//Transform from Raster To Screen and Screen to World
			Point origin = camera.RasterToWorld(Point((float)x, (float)y, 0.f));
			Point destination = camera.WorldToFarPlane(origin);
			Vector direction = destination - origin;
			Ray ray(origin, direction, 0.f);
			RGB bg; bg.red = 166; bg.green = 166; bg.blue = 166;
			RGB pixelColor; pixelColor.red = 0; pixelColor.green = 0; pixelColor.blue = 0;
			
			//Begin tracing
			int depth = Trace(ray, &pixelColor, -1);

			if (depth > 1)
				bool asds= false;

			if (depth < 1)
				pixelColor = bg;

			printf("%c%c%c", pixelColor.red, pixelColor.green, pixelColor.blue);
		}
	}
}

int Renderer::Trace(const Ray &reflRay, RGB * color, const int lastShape)
{
	if (reflRay.depth > 5)
		return reflRay.depth;

	std::vector<Shape*>::iterator iShape;
	float currHit = -1.f;
	float minHit = INFINITY;
	bool hit = false;
	float eps = 0.f;
	Ray nextReflRay;
	Normal hitNormal;
	Normal normal;
	Ray ray;
	Colors shapeColor;
	int hitShapeID = -1;
	for (iShape = shapes->begin(); iShape!=shapes->end(); ++iShape)
	{
		int currID = (*iShape)->ShapeID;
		if ((*iShape)->Intersect(reflRay, &currHit, &eps, &ray, &normal) && currHit < minHit && currID != lastShape)
		{
			minHit = currHit;
			hitNormal = normal;
			nextReflRay = ray;
			shapeColor = (*iShape)->GetColor();
			hit = true;
			hitShapeID = currID;
		}
	}
	if (hit)
	{
		RGB c = computeColor(hitNormal, nextReflRay, shapeColor, hitShapeID);
		color->red += c.red;
		color->green += c.green;
		color->blue += c.blue;

		color->red = std::min(color->red, 255);
		color->green = std::min(color->green, 255);
		color->blue = std::min(color->blue, 255);

		color->red = std::max(color->red, 20);
		color->green = std::max(color->green, 20);
		color->blue = std::max(color->blue, 20);

		nextReflRay.depth = reflRay.depth + 1;

		return Trace(nextReflRay, color, hitShapeID);
	}
	return reflRay.depth;
}
			