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
	Scene = new std::vector<Primitive*>();
	Samples = quality.Samples;
	InvSamples = 1.f/(float)Samples;
	Quality = quality;

	std::vector<Triangle*> *Tris = new std::vector<Triangle*>();
	std::vector<Primitive*>::iterator iScene;
	for (iScene = scene->begin(); iScene!=scene->end();++iScene)
	{	
		if ((*iScene)->Type == 2)
		{
			Tris->push_back((Triangle*)(*iScene));
		}
		else
			Scene->push_back((*iScene));
	}
	Root = KDNode::Build(Tris);
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
				Vector jitter = Vector(jit*r(), jit*r(), jit*r());
				int j = i>0;
				Vector direction = Normalize((destination - origin) + jitter*j);
				Ray ray(origin, direction, 0.0f);
				
				//Begin tracing
				pixelColor += Trace(ray);
			}
			pixelColor *= InvSamples;
			pixelColor.Bound(MIN_COLOR, MAX_COLOR);

			pixelColor *= 255.f;
			outFile << (char)pixelColor.red << (char)pixelColor.green << (char)pixelColor.blue;
			//printf("Finished Pixel\n");
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

RGB Renderer::Trace(const Ray & ray)
{
	return RGB();
}

bool Renderer::FindClosest(const Ray &ray, Hit *hit) 
{
	Hit triHit;
	bool didWeHit = Root->Intersect(ray, &triHit);
	//else do primitives:
	
	std::vector<Primitive*>::iterator iScene;
	Hit currHit, bestHit;
	for (iScene = Scene->begin(); iScene!=Scene->end(); ++iScene)
	{	
		if ((*iScene)->Intersect(ray, &currHit) && currHit.tHit < bestHit.tHit)
		{
			bestHit = currHit;
			didWeHit = true;
		}
	}
	*hit = triHit.tHit < bestHit.tHit ? triHit : bestHit;
	return didWeHit;
}