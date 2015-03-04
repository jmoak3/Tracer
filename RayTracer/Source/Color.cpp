#include "Color.h"
#include "Ray.h"
#include <math.h>
#include "Renderer.h"
#include <time.h>

inline float ra(float f)
{
	return f*(float)rand()/(float)RAND_MAX;
}

Material::Material(const RGB &diff, const RGB &spec, const RGB &amb)
{
	Diffuse = diff;
	Specular = spec;
	Ambient = amb;
}

Ray Material::ReflectRay(const Ray &ray, const Hit &hit)
{
	Ray r(ray.o, ray.d, 0.f);
	
	float jit = 10.f;
	Vector jitter = Vector(jit - ra(jit), jit - ra(jit), jit - ra(jit));
	Vector dir = ray.d;

	r.o = ray.o + ray.d*hit.tHit;
	r.d = dir - Vector(2.f*(Dot(dir, hit.normal))*hit.normal);
	r.d = Normalize(r.d + jitter);
	r.depth = ray.depth;
	return r;
}
