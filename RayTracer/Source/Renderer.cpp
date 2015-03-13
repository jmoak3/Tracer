#include "Renderer.h"
#include <math.h>
#include "Ray.h"
#include <ctime>
#include "Sphere.h"
#include <fstream>

#define MAX_COLOR 0.999f
#define MIN_COLOR 0.078f

inline float r()
{
	return (float)((rand()/(float)RAND_MAX)-0.5f);
}

Renderer::Renderer(std::vector<Primitive*>* scene, const Camera &ccamera)
{
	Cam = ccamera;
	Scene = scene;
	Samples = 4;
	LightSamples = 64;
	DiffuseReflectiveSamples = 256;
	InvSamples = 1.f/(float)Samples;
	InvLightSamples = 1.f/(float)LightSamples;
	InvDiffuseReflectiveSamples = 1.f/(float)DiffuseReflectiveSamples;
}

RGB Renderer::computeColor(const Ray &reflRay, const Hit &hit)
{
	Normal normal = hit.normal;
	Material shapeMaterial = hit.material;

	Point hitPos = reflRay.o + hit.tHit*reflRay.d;

	Vector v = -reflRay.d;

	RGB finalColor;
	std::vector<Primitive*>::iterator iScene;
	if (hit.type == 1) 
	{
		//float distFunc = hit.material.Emissive/(12.56637f*reflRay.time*reflRay.time);
		//distFunc = std::min(1.0f, std::max(0.f, distFunc));
		RGB c = hit.material.Color;
		return c;
	}

	for (iScene = Scene->begin(); iScene!=Scene->end(); ++iScene)
	{	
		if ((*iScene)->Type == 1) //IS LIGHT??
		{
			//FIX, SPHERE IS HARDCODED IN COLOR CALCS!!!
			Material lightMaterial = ((Sphere*)(*iScene))->GetMaterial();
			float radius = ((Sphere*)(*iScene))->Radius;
			Point lightPos = ((Sphere*)(*iScene))->ObjectToWorld(Point(0.f, 0.f, 0.f));
			
			RGB sampleColor;
			for (int i=0;i<LightSamples;++i)
			{
				Vector jitter = Vector(r(), r(), r());
				jitter = radius*Normalize(jitter);

				Vector shadowL = ((lightPos+jitter) - hitPos);
				float distToLight = shadowL.Length();
				shadowL = Normalize(shadowL);
				Vector l = shadowL;//Normalize((lightPos) - hitPos);

				if (ShadowTest(Ray(hitPos, shadowL, hit.eps, distToLight)))
					continue;
				
				Vector r = Vector(2.f*Dot(l, normal)*normal) - l;

				float spec = pow(std::min(1.0f, std::max(0.f, Dot(r, v))),20.f);
				RGB specular = lightMaterial.Color*shapeMaterial.Specular*spec;

				float diff = std::min(1.0f, std::max(0.f, Dot(normal, l)));
				RGB diffuse = shapeMaterial.Color*diff*shapeMaterial.Diffuse*lightMaterial.Color;

				sampleColor += diffuse + specular;
			}
			finalColor += sampleColor*InvLightSamples;
		}
	}

	return finalColor;
}

void Renderer::Render()
{
	srand(time(0));
	std::ofstream outFile;
	outFile.open("file.ppm", std::ios::out);

	//Identify x and y using camera's specs, and make for loop
	const int height = Cam.height;
	const int width = Cam.width;

	outFile << "P6 " << height << " " << width << " 255 ";
	printf("Tracing Rays...\n");

	long double startTime = time(0);
	long double lastTime = startTime;
	int lastPercent = 0;
	for (int y=0;y<height;++y)
	{
		for (int x=0;x<width;++x)
		{
			float jit = (float)Cam.height/500.f;
			//Transform from Raster To Screen and Screen to World
			RGB pixelColor; pixelColor.red = 0; pixelColor.green = 0; pixelColor.blue = 0;
			for (int i=0;i<Samples;++i)
			{
				Point origin = Cam.ScreenToWorld(Point(0.f, 0.f, 0.f));
				Point destination = Cam.WorldToFarPlane(Point((float)x, (float)y, 0.f));
				Vector jitter = Vector(jit*r(), jit*r(), jit*r());
				Vector direction = Normalize((destination - origin) + jitter*(bool)i);
				Ray ray(origin, direction, 0.f);
				
				//Begin tracing
				RGB color; color.red = 0; color.green = 0; color.blue = 0;
				color = Trace(ray);
				pixelColor += color;
			}
			pixelColor *= InvSamples;
			pixelColor.Bound(MIN_COLOR, MAX_COLOR);

			pixelColor *= 255.f;
			outFile << (char)pixelColor.red << (char)pixelColor.green << (char)pixelColor.blue;
			
		}
		int percent = 100*(float)y/(float)height;
		if (lastPercent != percent) 
		{
			long double currTime = time(0);
			long double estimate = ((long double)(100-percent)*(currTime-startTime)/(long double)(percent));
			printf("%i%% Complete, %0.01f Seconds Remaining\n", percent, estimate);
			lastPercent = percent;
			lastTime = currTime;
		}
	}
	long double endTime = time(0)*1000.0;
	startTime *= 1000.0;
	printf("Run Time: %0.01f Milliseconds", (endTime - startTime));
	outFile.close();
}

RGB Renderer::Trace(const Ray &reflRay)
{
	if (reflRay.depth > 2)
		return RGB();

	Hit bestHit;
	if (FindClosest(reflRay, &bestHit))
	{
		RGB diffuseSampleColor;
		int DiffuseReflectiveSamplesToRun = 1;

		if (reflRay.depth == 0 && bestHit.material.DiffuseReflective > 0.01f) //Only primary rays should diffuse... it's very slow!
			DiffuseReflectiveSamplesToRun = DiffuseReflectiveSamples;

		for (int i=0;i<DiffuseReflectiveSamplesToRun;++i)
		{
			RGB c;
			c = computeColor(reflRay, bestHit);

			Ray nextReflRay = bestHit.material.ReflectRay(reflRay, bestHit);

			//nextReflRay.time = reflRay.time + bestHit.tHit*(2.f - 2.f*bestHit.material.Specular);
			float refl = bestHit.material.Reflective;
			diffuseSampleColor += c + c*Trace(nextReflRay)*refl;
			//CalcColor + (CalcColor*NextColor*Reflectivity)
		}

		if (reflRay.depth == 0  && bestHit.material.DiffuseReflective > 0.01f) // If a primary ray we have done diffusion so average
			diffuseSampleColor *= InvDiffuseReflectiveSamples;

		return diffuseSampleColor;
	}

	return RGB();
}

bool Renderer::FindClosest(const Ray &ray, Hit *hit) 
{
	std::vector<Primitive*>::iterator iScene;
	//if (currRay.mint > 0.f) currRay.o += Normalize(currRay.d)*currRay.mint;
	Hit currHit, bestHit;
	bool didWeHit = false;
	for (iScene = Scene->begin(); iScene!=Scene->end(); ++iScene)
	{	
		if ((*iScene)->Intersect(ray, &currHit), currHit.tHit < bestHit.tHit)
		{
			bestHit = currHit;
			didWeHit = true;
		}
	}
	*hit = bestHit;
	return didWeHit;
}

bool Renderer::ShadowTest(const Ray &ray) 
{
	std::vector<Primitive*>::iterator iScene;
	//if (currRay.mint > 0.f) currRay.o += Normalize(currRay.d)*currRay.mint;
	Hit currHit;
	for (iScene = Scene->begin(); iScene!=Scene->end(); ++iScene)
	{	
		if ((*iScene)->Intersect(ray, &currHit) && currHit.type == 0)
		{
			return true;
		}
	}
	return false;
}



