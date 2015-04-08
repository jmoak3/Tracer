#ifndef PATHRENDERER_H
#define PATHRENDERER_H
#include "Renderer.h"

class PathRenderer : public Renderer
{
public:
	PathRenderer(std::vector<Primitive*>* scene, const Camera &ccamera, const QualityDesc &quality);
	~PathRenderer() {delete Lights;};

protected:
	RGB CalcDirectLighting(const Ray &reflRay, const Hit &hit, RNG &rng);
	void Render();
	RGB Trace(const Ray &reflRay, RNG &rng);
	RGB DirectLightIndirectIllumTrace(const Ray &reflRay, RNG &rng);
	bool ShadowTest(const Ray &ray);
	std::vector<Primitive*> *Lights; // for shadow tests (speed!)
};

#endif