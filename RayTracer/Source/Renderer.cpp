#include "Renderer.h"

Renderer::Renderer(std::vector<Shape*>* vshapes, std::vector<Light*>*vlights, const Camera &ccamera)
{
	camera = ccamera;
	shapes = vshapes;
	lights = vlights; 
}

RGB Renderer::computeColor(const Normal &normal, const Ray &reflRay, const RGB & shapeColor) 
{
	RGB finalColor; finalColor.red = 0; finalColor.blue = 0; finalColor.green = 0;
	std::vector<Light*>::iterator iLight;

	for (iLight = lights->begin(); iLight!=lights->end(); ++iLight)
	{
		float spec = 0.1f*std::max(0.f, Dot(reflRay.d, Vector(reflRay.o - Point(0.f, 0.f, 0.f))));
		RGB specular;
		specular.red = spec;
		specular.green = spec;
		specular.blue = spec;
	
		Vector l = Normalize((*iLight)->Position - reflRay.o);
		float diff = std::min(1.0f, std::max(0.f, Dot(Normalize(normal), l)));
		RGB diffuse;
		diffuse.red = (int)((float)shapeColor.red*diff);
		diffuse.green = (int)((float)shapeColor.green*diff);
		diffuse.blue = (int)((float)shapeColor.blue*diff);

		RGB lightColor;
		lightColor.red  = diffuse.red;// + specular.red;
		lightColor.green= diffuse.green;// + specular.green;
		lightColor.blue = diffuse.blue;// + specular.blue;

		lightColor.red = std::min(lightColor.red, 255);
		lightColor.green = std::min(lightColor.green, 255);
		lightColor.blue = std::min(lightColor.blue, 255);

		lightColor.red = std::max(lightColor.red, 20);
		lightColor.green = std::max(lightColor.green, 20);
		lightColor.blue = std::max(lightColor.blue, 20);

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
			Point origin = camera.RasterToWorld(Point(x, y, 0.f));
			Point destination = camera.WorldToFarPlane(origin);
			Vector direction = destination - origin;
			Ray ray(origin, direction, 0.f);
			RGB bg; bg.red = 166; bg.green = 166; bg.blue = 166;
			RGB pixelColor; pixelColor.red = 0; pixelColor.green = 0; pixelColor.blue = 0;
			
			//Begin tracing
			int depth = Trace(ray, &pixelColor, -1);

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
	RGB shapeColor;
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
		RGB c = computeColor(hitNormal, nextReflRay, shapeColor);
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
			