#ifndef RENDERER_H	
#define RENDERER_H
#include "Shape.h"
#include "Camera.h"
#include "QualityDesc.h"
#include "KDTree.h"
#include <random>

#include <vector>

typedef std::mt19937 RNG;

class Renderer
{
public:
	Renderer(std::vector<Primitive*>* scene, const Camera &ccamera, const QualityDesc &quality);
	virtual ~Renderer() {delete Root, Scene;}
	virtual void Render();

protected:
	virtual bool FindClosest(const Ray &ray, Hit *hit); 
	void ThreadedTrace(int y, RGB *imgBuffer, RNG &rng);
	virtual RGB Trace(const Ray &reflRay, RNG &rng);
	std::vector<Primitive*> *Scene;
	Camera Cam;
	int Samples;
	int Height;
	int Width;
	float InvSamples;
	KDNode *Root; 
	QualityDesc Quality;
};



#endif