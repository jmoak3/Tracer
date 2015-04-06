#ifndef RENDERER_H	
#define RENDERER_H
#include "Shape.h"
#include "Camera.h"
#include "QualityDesc.h"
#include "SpaceDivision.h"
#include "KDTree.h"

#include <vector>

class Renderer
{
public:
	Renderer(std::vector<Primitive*>* scene, const Camera &ccamera, const QualityDesc &quality);
	virtual ~Renderer() {delete Root;}
	virtual void Render();

protected:
	virtual bool FindClosest(const Ray &ray, Hit *hit); 
	virtual RGB Trace(const Ray &reflRay);
	virtual bool FindClosestSD(const Ray &ray, Hit *hit); 
	bool SetupSpaceDivisions();
	bool SpaceDivisionsTooFull();
	std::vector<Primitive*> *Scene;
	Camera Cam;
	SpaceDivision* CurrSpaceDiv;
	SpaceDivision* CameraDiv;
	int Samples;
	float InvSamples;
	std::vector<SpaceDivision> *Space; 
	KDNode *Root; 
	QualityDesc Quality;
};



#endif