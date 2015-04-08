#include "Renderer.h"
#include <math.h>
#include "Ray.h"
#include <time.h>
#include "Sphere.h"
#include <fstream>
#include <sys/timeb.h>
#include <thread>
#include <mutex>

#define MAX_COLOR 0.999f
#define MIN_COLOR 0.078f

static const int NumThreads = 5;
std::mutex MyMutex;


inline float r(RNG& rng)
{
	std::uniform_real_distribution<float> d(-0.5f, 0.5f);
	return d(rng);
	//return (float)rand()/((float)RAND_MAX) - 0.5f;
}

inline float r1(RNG &rng)
{
	std::uniform_real_distribution<float> d(0.f, 1.f);
	return d(rng);
	//return (float)rand()/((float)RAND_MAX));
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

void Renderer::ThreadedTrace(int y, RGB *imgBuffer, RNG &rng)
{
	for (int x=0;x<Width;++x)
	{
		Point p1 = Cam.WorldToFarPlane(Point(0.f, 0.f, 0.f));
		Point p2 = Cam.WorldToFarPlane(Point(1.f, 1.f, 0.f));
		float jit = (p2.x - p1.x); // the movement between a pixel

		//Transform from Raster To Screen and Screen to World
		RGB pixelColor; pixelColor.red = 0.f; pixelColor.green = 0.f; pixelColor.blue = 0.f;
		for (int i=0;i<Samples;++i)
		{
			Point origin = Cam.ScreenToWorld(Point(0.f, 0.f, 0.f));
			Point destination = Cam.WorldToFarPlane(Point((float)x, (float)y, 0.f));
			Vector jitter = Vector(jit*r(rng), jit*r(rng), jit*r(rng));
			int j = i>0;
			Vector direction = Normalize((destination - origin) + jitter*j);
			Ray ray(origin, direction, 0.0f);
				
			//Begin tracing
			pixelColor += Trace(ray, rng);
		}
		pixelColor *= InvSamples;

		pixelColor.Bound(MIN_COLOR, MAX_COLOR);
		pixelColor *= 255.f;

		imgBuffer[y*Width+x] = pixelColor;
	}
}


void Renderer::Render()
{
	srand(time(0));
	Height = Cam.height;
	Width = Cam.width;
	
	std::thread thrd[NumThreads];
	RNG* rng = new RNG[NumThreads];

	RGB *imgBuffer = new RGB[Width*Height];

	long double startTime = GetMS();
	int lastPercent = 0;
	int y = 0;
	while (y < Height)
	{
		int numCreatedThreads = 0;
		for (int i=0;i<NumThreads;++i)
		{
			rng[i].seed(i*1000*rand());
			thrd[i] = std::thread(&Renderer::ThreadedTrace, this, y, imgBuffer, rng[i]);
			++numCreatedThreads;
			++y;
			if (y+i+1>=Height) 
				break;
		}

		for (int i=0;i<numCreatedThreads;++i)
			if (thrd[i].joinable())
				thrd[i].join();

		int percent = 100*(float)(y)/(float)Height;
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

	std::ofstream outFile;
	outFile.open("file.ppm", std::ios::out);
	if (outFile.bad())
		return;
	outFile << "P6 " << Height << " " << Width << " 255 ";

	for (int i=0; i<Width*Height;++i)
	{
		outFile << (char)imgBuffer[i].red << (char)imgBuffer[i].green << (char)imgBuffer[i].blue;
	}
	outFile.close();
	printf("\n");
}

RGB Renderer::Trace(const Ray & ray, RNG &rng)
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