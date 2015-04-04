#ifndef RENDERER_H	
#define RENDERER_H
#include "Shape.h"
#include "Camera.h"
#include "QualityDesc.h"
#include "SpaceDivision.h"

#include <vector>

class Renderer
{
public:
	Renderer(std::vector<Primitive*>* scene, const Camera &ccamera, const QualityDesc &quality);
	virtual ~Renderer() {delete Space;}
	virtual void Render();

protected:
	virtual bool FindClosest(const Ray &ray, Hit *hit); 
	virtual RGB Trace(const Ray &reflRay);
	bool SetupSpaceDivisions();
	bool SpaceDivisionsTooFull();
	std::vector<Primitive*> *Scene;
	Camera Cam;
	SpaceDivision* CurrSpaceDiv;
	SpaceDivision* CameraDiv;
	int Samples;
	float InvSamples;
	std::vector<SpaceDivision> *Space; 
};



#endif