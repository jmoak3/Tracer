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
	Samples = quality.Samples;
	InvSamples = 1.f/(float)Samples;
	SetupSpaceDivisions();
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

bool Renderer::SetupSpaceDivisions()
{
	Space = new std::vector<SpaceDivision>();
	SpaceDivision initial(Scene);
	Space->push_back(initial);

	while (SpaceDivisionsTooFull())
	{
		std::vector<SpaceDivision>::iterator iSpace;
		for (iSpace = Space->begin(); iSpace != Space->end(); ++iSpace)
		{
			if (iSpace->ShouldSplit())
			{
				SpaceDivision * newSplit = iSpace->GetSplit();
				for (int i=0;i<7;++i)
					Space->push_back(newSplit[i]);
				iSpace = Space->erase(iSpace);
				--iSpace; // counteract for-loop.
			}
		}
	}
	bool foundCam = false;
	Point camPoint = Cam.ScreenToWorld(Point(0.f,0.f,0.f));
	std::vector<SpaceDivision>::iterator iSpace;
	for (iSpace = Space->begin(); iSpace != Space->end(); ++iSpace)
	{
		if (iSpace->Bounds.Contains(camPoint))
		{
			currSpaceDiv = *iSpace;
			foundCam = true;
		}
	}
	return foundCam;
}

bool Renderer::SpaceDivisionsTooFull()
{
	std::vector<SpaceDivision>::iterator iSpace;
	for (iSpace = Space->begin(); iSpace != Space->end(); ++iSpace)
	{
		if (iSpace->ShouldSplit())
			return true;
	}
	return false;
}

bool Renderer::FindClosest(const Ray &ray, Hit *hit) 
{
	///currSpaceDiv = *GetNextDivision(ray); //intersect among adjacent

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


SpaceDivision* Renderer::GetNextDivision(const Ray &ray) 
{
	// How to find adjacent ones, fast?
	// No intersect! Use value of x,y,z in dir and set accordingly.
	// Have references to up, forward, right, and others cached in setup!!
	

	//Change division reference to index?
	//Plug in index get reference from vector
	
	//Move to SpaceDivision, and get index from there?
	//PRECACHE Adjacent list through GetSplit's SpaceDivision custom constructor


	SpaceDivision * next;
	return next;
}
