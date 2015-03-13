#ifndef RENDERER_H	
#define RENDERER_H
#include "Shape.h"
#include "Camera.h"

#include <vector>

class Renderer
{
public:
	Renderer(std::vector<Primitive*>* scene, const Camera &ccamera);
	void Render();
	RGB Trace(const Ray &reflRay);
	RGB computeColor(const Ray &reflRay, const Hit & hit);

private:
	bool FindClosest(const Ray &ray, Hit *hit); 
	bool ShadowTest(const Ray &ray); 
	std::vector<Primitive*> *Scene;
	Camera Cam;
	int Samples;
	int LightSamples;
	int DiffuseReflectiveSamples;
	float InvSamples;
	float InvLightSamples;
	float InvDiffuseReflectiveSamples;
};

#endif