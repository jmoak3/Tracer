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

inline float r1()
{
	return (float)((rand()/(float)RAND_MAX));
}

inline long double GetMS()
{
	timeb t;
	ftime(&t);
	return 1000.0*t.time+t.millitm;
}

Renderer::Renderer(std::vector<Primitive*>* scene, const Camera &ccamera, const QualityDesc & quality)
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
	Samples = quality.Samples;
	LightSamples = quality.LightSamples;
	GlossyReflectiveSamples = quality.GlossyReflectiveSamples;
	Depth = quality.Depth;
	Path = quality.Path;
	InvSamples = 1.f/(float)Samples;
	InvLightSamples = 1.f/(float)LightSamples;
	InvGlossyReflectiveSamples = 1.f/(float)GlossyReflectiveSamples;
}

void Renderer::Render()
{
	srand(time(0));
	std::ofstream outFile;
	outFile.open("file.ppm", std::ios::out);
	if (outFile.bad())
		return;

	//Identify x and y using camera's specs, and make for loop
	const int height = Cam.height;
	const int width = Cam.width;

	outFile << "P6 " << height << " " << width << " 255 ";
	if (Path)
		printf("%ix%i Path %i Samples\n", width, height, Samples);
	else
		printf("%ix%i Ray %i AA Samples, %i LightSamples, %i GlossyReflectionSamples, %i Depth\n", width, height, Samples, LightSamples, GlossyReflectiveSamples, Depth);
	

	Point p1 = Cam.WorldToFarPlane(Point(0.f, 0.f, 0.f));
	Point p2 = Cam.WorldToFarPlane(Point(1.f, 1.f, 0.f));
	float jit = (p2.x - p1.x); // the movement between a pixel

	long double startTime = GetMS();
	int lastPercent = 0;
	for (int y=0;y<height;++y)
	{
		for (int x=0;x<width;++x)
		{
			//Transform from Raster To Screen and Screen to World
			RGB pixelColor; pixelColor.red = 0; pixelColor.green = 0; pixelColor.blue = 0;
			for (int i=0;i<Samples;++i)
			{
				Point origin = Cam.ScreenToWorld(Point(0.f, 0.f, 0.f));
				Point destination = Cam.WorldToFarPlane(Point((float)x, (float)y, 0.f));
				//float isNotPath = (float)(!Path)*1.f;
				Vector jitter = Vector(jit*r(), jit*r(), jit*r());//*isNotPath;
				Vector direction = Normalize((destination - origin) + jitter*(bool)i);
				Ray ray(origin, direction, 0.f);
				
				//Begin tracing
				if (Path)
					pixelColor += PathTrace(ray);
				else 
					pixelColor += RayTrace(ray);
			}
			pixelColor *= InvSamples;
			pixelColor.Bound(MIN_COLOR, MAX_COLOR);

			pixelColor *= 255.f;
			outFile << (char)pixelColor.red << (char)pixelColor.green << (char)pixelColor.blue;
		}
		int percent = 100*(float)y/(float)height;
		if (lastPercent != percent) 
		{
			printf("\r%i%% Complete; %0.01f Milliseconds Elapsed", percent, (long double)(GetMS() - startTime));
			lastPercent = percent;
		}
	}
	int percent = 100;
	if (lastPercent != percent) 
	{
		printf("\r%i%% Complete; %0.01f Milliseconds Elapsed", percent, (long double)(GetMS() - startTime));
		lastPercent = percent;
	}
	outFile.close();
	printf("\n");
}

RGB Renderer::computeColor(const Ray &reflRay, const Hit &hit)
{
	Normal normal = hit.normal;
	Material shapeMaterial = hit.material;
	Point hitPos = reflRay.o + hit.tHit*reflRay.d;
	Vector v = -reflRay.d;
	if (Dot(normal, reflRay.d) > 0.f && hit.material.RefrAbsorbance > 0.9f)
		normal = -normal;

	RGB finalColor;
	std::vector<Primitive*>::iterator iLight;
	if (hit.type == 1) 
		return hit.material.Color;

	for (iLight = Lights->begin(); iLight!=Lights->end(); ++iLight)
	{	
		Sphere * currLight = dynamic_cast<Sphere*>(*iLight);
		//FIX, SPHERE IS HARDCODED IN LIGHT CALCS!!!
		Material lightMaterial = currLight->GetMaterial();
		float radius = currLight->Radius;
		Point lightPos = (*currLight->ObjectToWorld)(Point(0.f, 0.f, 0.f));
		
		RGB sampleColor;
		for (int i=0;i<LightSamples;++i)
		{
			Vector jitter = Vector(r(), r(), r());
			float shouldJitter = 
				std::min(((float)LightSamples-1.f), 1.f);
			jitter = radius*Normalize(jitter)*(shouldJitter);

			Vector l = ((lightPos+jitter) - hitPos);
			float distToLight = l.Length();
			l = Normalize(l);

			if (ShadowTest(Ray(hitPos, l, hit.eps, distToLight)))
				continue;

			Vector r = Vector(2.f*Dot(l, normal)*normal) - l;
			
			float spec = pow(std::min(1.0f, std::max(0.f, Dot(r, v))),20.f);
			RGB specular = lightMaterial.Color*shapeMaterial.Specular*spec;

			float diff = std::min(1.0f, std::max(0.f, Dot(normal, l)));
			RGB diffuse;
			if (shapeMaterial.Diffuse < 0.f)
				diffuse = shapeMaterial.Color;
			else
				diffuse = shapeMaterial.Color*diff*shapeMaterial.Diffuse*lightMaterial.Color;
			
			sampleColor += diffuse + specular;
			sampleColor.Bound(MIN_COLOR, MAX_COLOR);
		}
		finalColor += sampleColor*InvLightSamples;
	}

	return finalColor;
}

RGB Renderer::RayTrace(const Ray &reflRay)
{
	if (reflRay.depth > Depth)
		return RGB();

	Hit bestHit;
	if (FindClosest(reflRay, &bestHit))
	{
		RGB sampleColor;
		int GlossyReflectiveSamplesToRun = 1;
		bool isGlossyReflective = 
							(reflRay.depth == 0 
							&& bestHit.material.GlossyReflective > 0.001f
							&& bestHit.material.Reflective > 0.001f);

		if (isGlossyReflective)  //Only primary rays should diffuse... it's very slow!
			GlossyReflectiveSamplesToRun = GlossyReflectiveSamples;
		RGB absorb = bestHit.material.Color*bestHit.material.RefrAbsorbance*-bestHit.tHit;
		RGB transparency = RGB(expf(absorb.red), expf(absorb.green), expf(absorb.blue));
		for (int i=0;i<GlossyReflectiveSamplesToRun;++i)
		{
			RGB c = computeColor(reflRay, bestHit);
			
			//Early out before calcing the refr and refl!
			if (reflRay.depth+1 <= Depth) 
			{
				bool isRefr = (bestHit.material.RefrAbsorbance<1.f);
				Ray nextReflRay = bestHit.material.ReflectRay(reflRay, bestHit);
				Ray nextRefrRay = bestHit.material.RefractRay(reflRay, bestHit, &isRefr);
				float refl = bestHit.material.Reflective;
				bool canRefl = refl > 0.f && !c.IsBlack();
				bool canRefr = isRefr && !transparency.IsBlack();
				sampleColor += c + 
					(canRefl ? 	c*RayTrace(nextReflRay)*refl : RGB()) 
					+ (canRefr ? transparency*RayTrace(nextRefrRay) : RGB());
				//OPTIMIZE FURTHER!
			}
			else
				sampleColor += c;
		}

		if (isGlossyReflective) // If a primary ray we have done diffusion so average
			sampleColor *= InvGlossyReflectiveSamples;

		return sampleColor;
	}

	return RGB();
}

RGB Renderer::PathTrace(const Ray &reflRay)
{
	Hit bestHit;
	if (FindClosest(reflRay, &bestHit))
	{
		if (bestHit.type == 1) 
			return (*Lights)[0]->GetMaterial().Color*(*Lights)[0]->GetMaterial().Emissive;
		RGB c = bestHit.material.Color;

		Ray nextReflRay = bestHit.material.ReflectRay(reflRay, bestHit, Path);
		
		float p = c.red>c.green && c.red>c.blue ? c.red : c.green>c.blue ? c.green : c.blue;
		
		if (nextReflRay.depth>5){
			if (r1() < p*0.999f) 
				c *= (0.999f/p);
			else 
				return (*Lights)[0]->GetMaterial().Color*(*Lights)[0]->GetMaterial().Emissive;
		
		}

		return c*PathTrace(nextReflRay);
	}
	else 
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
	std::vector<Primitive*>::iterator iScene;
	Hit currHit;
	for (iScene = Scene->begin(); iScene!=Scene->end(); ++iScene)
	{	
		if ((*iScene)->Intersect(ray, &currHit) && currHit.type == 0 && currHit.material.RefrAbsorbance > 0.4f)
			return true;
	}
	return false;
}



