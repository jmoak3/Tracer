#ifndef RAYRENDERER_H
#define RAYRENDERER_H
#include "Renderer.h"

class RayRenderer : public Renderer
{
public:
	RayRenderer(std::vector<Primitive*>* scene, const Camera &ccamera, const QualityDesc &quality);
	~RayRenderer() {delete Lights;};

protected:
	void Render();
	RGB Trace(const Ray &reflRay);
	bool ShadowTest(const Ray &ray);
	RGB computeColor(const Ray &reflRay, const Hit & hit);
	
	std::vector<Primitive*> *Lights; // for shadow tests (speed!)
	int LightSamples;
	int GlossyReflectiveSamples;
	float InvLightSamples;
	float InvGlossyReflectiveSamples;
	int Depth;
};

#endif