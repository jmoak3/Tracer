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
	RGB Trace(const Ray &reflRay, RNG &rng);
	bool ShadowTest(const Ray &ray);
	RGB computeColor(const Ray &reflRay, const Hit & hit, RNG &rng);
	
	std::vector<Primitive*> *Lights; // for shadow tests (speed!)
	int LightSamples;
	int GlossyReflectiveSamples;
	float InvLightSamples;
	float InvGlossyReflectiveSamples;
	int Depth;
};

#endif