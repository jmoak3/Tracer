#ifndef RENDERER_H	
#define RENDERER_H
#include "Light.h"
#include "Shape.h"
#include "Camera.h"

#include <vector>

class Renderer
{
public:
	Renderer(std::vector<Shape*> *vshapes, std::vector<Light*>* vlights, const Camera &ccamera);
	void Render();
	RGB Trace(const Ray &reflRay, const int lastShape);
	RGB computeColor(const Ray &reflRay, const Hit & hit);

private:
	std::vector<Shape*> *shapes;
	std::vector<Light*> *lights;
	Camera camera;
};

#endif