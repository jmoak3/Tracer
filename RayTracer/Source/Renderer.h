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
	int Trace(const Ray &reflRay, RGB * currColor, const int lastShape);
	RGB computeColor(const Normal &normal, const Ray &reflRay, const Colors & color, const int lastShape);

private:
	std::vector<Shape*> *shapes;
	std::vector<Light*> *lights;
	Camera camera;
};

#endif