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

		Vector r = Vector(2.f*Dot(l, normal)*normal) - l;

		float spec = pow(std::min(1.0f, std::max(0.f, Dot(r, v))),20.f);
		RGB white; white.red = 255; white.blue = 255; white.green = 255;
		RGB temp = (white - shapeMaterial.Diffuse);
		RGB specular = temp*spec*shapeMaterial.Specular;

		float diff = std::min(1.0f, std::max(0.f, Dot(normal, l)));
		RGB diffuse = shapeMaterial.Diffuse*diff;

		RGB lightColor = diffuse + specular;

		lightColor.red = std::min(lightColor.red, MAX_COLOR);
		lightColor.green = std::min(lightColor.green, MAX_COLOR);
		lightColor.blue = std::min(lightColor.blue, MAX_COLOR);

		lightColor.red = std::max(lightColor.red, MIN_COLOR); //(int)((float)shapeMaterial.red*0.3f));
		lightColor.green = std::max(lightColor.green, MIN_COLOR); //(int)((float)shapeMaterial.green*0.2f));
		lightColor.blue = std::max(lightColor.blue, MIN_COLOR); //(int)((float)shapeMaterial.blue*0.2f));

		finalColor += lightColor;
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
			//float jit = (float)camera.height/500.f;
			int samples = 10.f;
			
			//Transform from Raster To Screen and Screen to World
			RGB pixelColor; pixelColor.red = 0; pixelColor.green = 0; pixelColor.blue = 0;
			for (int i=0;i<samples;++i)
			{
				Point origin = camera.RasterToWorld(Point((float)x, (float)y, 0.f));
				Point destination = camera.WorldToFarPlane(origin);
				//Vector jitter = Vector(jit-r(jit/2.f), jit-r(jit/2.f), jit-r(jit/2.f));
				Vector direction = (destination - origin);//+jitter*(bool)i;
				Ray ray(origin, direction, 0.f);
				RGB bg; bg.red = 100; bg.green = 100; bg.blue = 166;
				
				//Begin tracing
				RGB color; color.red = 0; color.green = 0; color.blue = 0;
				color = Trace(ray, -1);
				pixelColor += color;
				
				//if (depth < 1)
				//	pixelColor = bg;
			}
			float invSamples = 1.f/samples;
			pixelColor *= invSamples;
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

RGB Renderer::Trace(const Ray &reflRay, const int lastShape)
{
	if (reflRay.depth > 15)
		return RGB();

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
		Ray nextReflRay = bestHit.material.ReflectRay(reflRay, bestHit);
		RGB c = computeColor(nextReflRay, bestHit);
		
		nextReflRay.depth = reflRay.depth + 1;
		nextReflRay.time += (bestHit.tHit)*(1.f-bestHit.material.Specular) + reflRay.time;
		//glass should reflect brick, but brick shouldn't reflect glass
		float invDistFunc = (4.0f*3.14159f*nextReflRay.time*nextReflRay.time)/1.f;
		float falloff = (1.f/(float)(1.f + reflRay.depth * invDistFunc));
		c *= falloff;

		return c + Trace(nextReflRay, bestHit.shapeID);
	}

	return RGB();
}
			