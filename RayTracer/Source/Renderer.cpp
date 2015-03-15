#include "Renderer.h"
#include <math.h>
#include "Ray.h"
#include <time.h>
#include "Sphere.h"
#include <fstream>
#include <sys/timeb.h>

#define MAX_COLOR 0.999f
#define MIN_COLOR 0.078f

inline float r()
{
	return (float)((rand()/(float)RAND_MAX)-0.5f);
}

inline long double GetMS()
{
	timeb t;
	ftime(&t);
	return 1000.0*t.time+t.millitm;
}

Renderer::Renderer(std::vector<Primitive*>* scene, const Camera &ccamera)
{
	Cam = ccamera;
	Scene = scene;
	Lights = new std::vector<Primitive*>();
	std::vector<Primitive*>::iterator iScene;
	for (iScene = Scene->begin(); iScene!=Scene->end(); ++iScene)
	{	
		if ((*iScene)->Type == 1)
			Lights->push_back((*iScene));
	}
	Samples = 4;
	LightSamples = 16;
	GlossyReflectiveSamples = 64;
	InvSamples = 1.f/(float)Samples;
	InvLightSamples = 1.f/(float)LightSamples;
	InvGlossyReflectiveSamples = 1.f/(float)GlossyReflectiveSamples;
}

RGB Renderer::computeColor(const Ray &reflRay, const Hit &hit)
{
	Normal normal = hit.normal;
	Material shapeMaterial = hit.material;
	Point hitPos = reflRay.o + hit.tHit*reflRay.d;
	Vector v = -reflRay.d;

	RGB finalColor;
	std::vector<Primitive*>::iterator iLight;
	if (hit.type == 1) 
		return hit.material.Color;

	for (iLight = Lights->begin(); iLight!=Lights->end(); ++iLight)
	{	
		Sphere * currLight = dynamic_cast<Sphere*>(*iLight);
		//FIX, SPHERE IS HARDCODED IN COLOR CALCS!!!
		Material lightMaterial = currLight->GetMaterial();
		float radius = currLight->Radius;
		Point lightPos = currLight->ObjectToWorld(Point(0.f, 0.f, 0.f));
			
		RGB sampleColor;
		for (int i=0;i<LightSamples;++i)
		{
			Vector jitter = Vector(r(), r(), r());
			jitter = radius*Normalize(jitter);

			Vector l = ((lightPos+jitter) - hitPos);
			float distToLight = l.Length();
			l = Normalize(l);
			if (ShadowTest(Ray(hitPos, l, hit.eps, distToLight)))
				continue;
			
			//Vector r = static_cast<Vector>(2.f*Dot(l, normal)*normal) - l;
			Vector r = Vector(2.f*Dot(l, normal)*normal) - l;

			float spec = pow(std::min(1.0f, std::max(0.f, Dot(r, v))),20.f);
			RGB specular = lightMaterial.Color*shapeMaterial.Specular*spec;

			float diff = std::min(1.0f, std::max(0.f, Dot(normal, l)));
			RGB diffuse = shapeMaterial.Color*diff*shapeMaterial.Diffuse*lightMaterial.Color;

			sampleColor += diffuse + specular;
		}
		finalColor += sampleColor*InvLightSamples;
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
	printf("%ix%i. %i AA Samples, %i LightSamples, %i GlossyReflectionSamples\n", width, height, Samples, LightSamples, GlossyReflectiveSamples);

	long double startTime = GetMS();
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
				pixelColor += Trace(ray);
			}
			pixelColor *= InvSamples;
			pixelColor.Bound(MIN_COLOR, MAX_COLOR);

			pixelColor *= 255.f;
			outFile << (char)pixelColor.red << (char)pixelColor.green << (char)pixelColor.blue;
		}
		int percent = 100*(float)y/(float)height;
		if (lastPercent != percent) 
		{
			printf("%i%% Complete\n", percent);
			lastPercent = percent;
		}
	}
	long double endTime = GetMS();
	printf("Run Time: %0.01f Milliseconds", (long double)(endTime - startTime));
	outFile.close();
}

RGB Renderer::Trace(const Ray &reflRay)
{
	if (reflRay.depth > 2)
		return RGB();

	Hit bestHit;
	if (FindClosest(reflRay, &bestHit))
	{
		RGB glossyReflectiveSampleColor;
		int GlossyReflectiveSamplesToRun = 1;
		bool isReflective = (reflRay.depth == 0 
							&& bestHit.material.GlossyReflective > 0.001f
							&& bestHit.material.Reflective > 0.001f);

		if (isReflective)  //Only primary rays should diffuse... it's very slow!
			GlossyReflectiveSamplesToRun = GlossyReflectiveSamples;

		for (int i=0;i<GlossyReflectiveSamplesToRun;++i)
		{
			RGB c = computeColor(reflRay, bestHit);

			Ray nextReflRay = bestHit.material.ReflectRay(reflRay, bestHit);

			float refl = bestHit.material.Reflective;
			glossyReflectiveSampleColor += (refl<0.001f || c.IsBlack()) ? c : c + c*Trace(nextReflRay)*refl;
		}

		if (isReflective) // If a primary ray we have done diffusion so average
			glossyReflectiveSampleColor *= InvGlossyReflectiveSamples;

		return glossyReflectiveSampleColor;
	}

	return RGB();
}

bool Renderer::FindClosest(const Ray &ray, Hit *hit) 
{
	std::vector<Primitive*>::iterator iScene;
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
	assert(ray.maxt != INFINITY);

	std::vector<Primitive*>::iterator iScene;
	Hit currHit;
	for (iScene = Scene->begin(); iScene!=Scene->end(); ++iScene)
	{	
		if ((*iScene)->Intersect(ray, &currHit) && currHit.type == 0)
			return true;
	}
	return false;
}



