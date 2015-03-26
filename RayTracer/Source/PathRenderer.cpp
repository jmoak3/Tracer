#include "PathRenderer.h"

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

PathRenderer::PathRenderer(std::vector<Primitive*>* scene, const Camera &ccamera, const QualityDesc &quality)
		:Renderer(scene, ccamera, quality)
{
}

void PathRenderer::Render()
{
	const int height = Cam.height;
	const int width = Cam.width;
	printf("%ix%i Path %i Samples\n", width, height, Samples);
	Renderer::Render();
}

RGB PathRenderer::Trace(const Ray &reflRay)
{
	Hit bestHit;
	if (FindClosest(reflRay, &bestHit))
	{
		if (bestHit.type == 1) 
			return bestHit.material.Color*bestHit.material.Emissive;
		RGB c = bestHit.material.Color;

		Ray nextReflRay = bestHit.material.ReflectRay(reflRay, bestHit, true);
		
		float maxRefl = c.red>c.green && c.red>c.blue ? c.red : c.green>c.blue ? c.green : c.blue;
		
		if (nextReflRay.depth>5)
		{	
			if (r1() < maxRefl*0.9f) 
				c *= (0.9f*maxRefl);
			else 
				return RGB(true);
		
		}

		return c*Trace(nextReflRay);
	}
	else 
		return RGB();
}



