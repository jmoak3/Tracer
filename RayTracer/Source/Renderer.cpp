#include "Renderer.h"
#include <math.h>
#include "Ray.h"
#include <time.h>

#define MAX_COLOR 255
#define MIN_COLOR 20

inline float r(int f)
{
	return f*(float)rand()/(float)RAND_MAX;
}

Renderer::Renderer(std::vector<Shape*>* vshapes, std::vector<Light*>*vlights, const Camera &ccamera)
{
	camera = ccamera;
	shapes = vshapes;
	lights = vlights; 
}

RGB Renderer::computeColor(const Ray &reflRay, const Hit &hit)
{
	Normal normal = hit.normal;
	Material shapeMaterial = hit.material;

	RGB finalColor; finalColor.red = 0; finalColor.blue = 0; finalColor.green = 0;
	std::vector<Light*>::iterator iLight;
	std::vector<Shape*>::iterator iShape;

	for (iLight = lights->begin(); iLight!=lights->end(); ++iLight)
	{	
		Point lightPos = (*iLight)->Position;
		Vector l = Normalize(lightPos - reflRay.o);

		//Test if this light is blocked!!!

		Ray shadowRay(reflRay.o, l, 0.f);
		bool hitSphere = false;
		Hit hitData;
		int thisShapeID = hit.shapeID;
		for (iShape = shapes->begin(); iShape!=shapes->end(); ++iShape)
		{
			if ((*iShape)->Intersect(shadowRay, &hitData) && hitData.shapeID != thisShapeID && hitData.tHit >= l.Length())
			{
				hitSphere = true;
				continue;
			}
		}
		if (hitSphere)
			continue;

		Vector v = Normalize(camera.ScreenToWorld(Point(0.f, 0.f, 0.f))- reflRay.o);

		Vector dir = l;
		
		Vector r = Vector(2.f*Dot(dir, normal)*normal) - dir;

		float spec = pow(std::min(1.0f, std::max(0.f, Dot(r, v))),20.f);
		RGB specular;
		specular.red = (int)(shapeMaterial.Specular.red*spec);
		specular.green = (int)(shapeMaterial.Specular.green*spec);
		specular.blue = (int)(shapeMaterial.Specular.blue*spec);

		float diff = std::min(1.0f, std::max(0.f, Dot(normal, l)));
		RGB diffuse;
		diffuse.red = (int)((float)shapeMaterial.Diffuse.red*diff);
		diffuse.green = (int)((float)shapeMaterial.Diffuse.green*diff);
		diffuse.blue = (int)((float)shapeMaterial.Diffuse.blue*diff);

		RGB ambient;
		ambient.red = shapeMaterial.Ambient.red;
		ambient.green = shapeMaterial.Ambient.green;
		ambient.blue = shapeMaterial.Ambient.blue;


		RGB lightColor;
		lightColor.red  = diffuse.red + specular.red + ambient.red;
		lightColor.green= diffuse.green + specular.green + ambient.green;
		lightColor.blue = diffuse.blue + specular.blue + ambient.blue;

		lightColor.red = std::min(lightColor.red, MAX_COLOR);
		lightColor.green = std::min(lightColor.green, MAX_COLOR);
		lightColor.blue = std::min(lightColor.blue, MAX_COLOR);

		lightColor.red = std::max(lightColor.red, MIN_COLOR); //(int)((float)shapeMaterial.red*0.3f));
		lightColor.green = std::max(lightColor.green, MIN_COLOR); //(int)((float)shapeMaterial.green*0.2f));
		lightColor.blue = std::max(lightColor.blue, MIN_COLOR); //(int)((float)shapeMaterial.blue*0.2f));

		finalColor.red += lightColor.red;
		finalColor.green += lightColor.green;
		finalColor.blue += lightColor.blue;
	}
	return finalColor;
}

void Renderer::Render()
{
	srand(time(0));
	//Identify x and y using camera's specs, and make for loop
	const int height = camera.height;
	const int width = camera.width;

	printf("P6 %i %i 255 ", height, width);
	for (int y=0;y<height;++y)
	{
		for (int x=0;x<width;++x)
		{
			float jit = (float)camera.height/250.f;
			int samples = 1+camera.height/25.f;
			
			//Transform from Raster To Screen and Screen to World
			RGB pixelColor; pixelColor.red = 0; pixelColor.green = 0; pixelColor.blue = 0;
			for (int i=0;i<samples;++i)
			{
				Point origin = camera.RasterToWorld(Point((float)x, (float)y, 0.f));
				Point destination = camera.WorldToFarPlane(origin);
				Vector jitter = Vector(jit-r(jit/2.f), jit-r(jit/2.f), jit-r(jit/2.f));
				Vector direction = (destination - origin)+jitter*(bool)i;
				Ray ray(origin, direction, 0.f);
				RGB bg; bg.red = 100; bg.green = 100; bg.blue = 166;
			
				//Begin tracing
				RGB color; color.red = 0; color.green = 0; color.blue = 0;
				int depth = Trace(ray, &color, -1);
				pixelColor.red += color.red;
				pixelColor.green += color.green;
				pixelColor.blue += color.blue;

				
				//if (depth < 1)
				//	pixelColor = bg;
			}

			pixelColor.red /= samples;
			pixelColor.green /= samples;
			pixelColor.blue /= samples;
			pixelColor.red = std::min(pixelColor.red, MAX_COLOR);
			pixelColor.green = std::min(pixelColor.green, MAX_COLOR);
			pixelColor.blue = std::min(pixelColor.blue, MAX_COLOR);

			pixelColor.red = std::max(pixelColor.red, MIN_COLOR);
			pixelColor.green = std::max(pixelColor.green, MIN_COLOR);
			pixelColor.blue = std::max(pixelColor.blue, MIN_COLOR);


			printf("%c%c%c", pixelColor.red, pixelColor.green, pixelColor.blue);
		}
	}
}

int Renderer::Trace(const Ray &reflRay, RGB * color, const int lastShape)
{
	if (reflRay.depth > 15)
		return reflRay.depth;

	std::vector<Shape*>::iterator iShape;
	RGB bg; bg.red = 166; bg.green = 166; bg.blue = 166;
	bool hit = false;
	Hit bestHit;
	Hit hitData;

	for (iShape = shapes->begin(); iShape!=shapes->end(); ++iShape)
	{
		if ((*iShape)->Intersect(reflRay, &hitData), hitData.tHit < bestHit.tHit && hitData.shapeID != lastShape)
		{
			bestHit = hitData;
			hit = true;
		}
	}
	if (hit)
	{
		float jit = 1.f;
		Vector jitter = Vector(jit-r(jit/2.f), jit-r(jit/2.f), jit-r(jit/2.f));
		Ray nextReflRay = bestHit.material.ReflectRay(reflRay, bestHit, jitter);
		RGB c = computeColor(nextReflRay, bestHit);
		float falloff = (1.f/(float)(reflRay.depth*10.f+1));
		color->red += (float)c.red*falloff;
		color->green += (float)c.green*falloff;
		color->blue += (float)c.blue*falloff;

		color->red = std::min(color->red, MAX_COLOR);
		color->green = std::min(color->green, MAX_COLOR);
		color->blue = std::min(color->blue, MAX_COLOR);

		color->red = std::max(color->red, MIN_COLOR);
		color->green = std::max(color->green, MIN_COLOR);
		color->blue = std::max(color->blue, MIN_COLOR);

		nextReflRay.depth = reflRay.depth + 1;

		return Trace(nextReflRay, color, bestHit.shapeID);
	}

	return reflRay.depth;
}
			