#ifndef PATHRENDERER_H
#define PATHRENDERER_H
#include "Renderer.h"

class PathRenderer : public Renderer
{
public:
	PathRenderer(std::vector<Primitive*>* scene, const Camera &ccamera, const QualityDesc &quality);
	~PathRenderer() {};

protected:
	void Render();
	RGB Trace(const Ray &reflRay);
};

#endif