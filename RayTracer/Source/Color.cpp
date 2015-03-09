#include "Color.h"
#include "Ray.h"
#include <math.h>
#include "Renderer.h"
#include <time.h>
#include <assert.h>

inline float ra(float f)
{
	return f*(float)rand()/(float)RAND_MAX;
}

Material::Material(const RGB &diff, const float spec, const float rough)
{
	assert(spec <= 1.0f && rough <= 1.0f && spec >= 0.f && rough >= 0.f);
	Diffuse = diff;
	Specular = spec;
	Rough = rough;
}

Ray Material::ReflectRay(const Ray &ray, const Hit &hit)
{
	Ray r(ray.o, ray.d, 0.f);
	
	float jit = Rough;
	Vector jitter = Vector(jit - ra(jit), jit - ra(jit), jit - ra(jit));
	Vector dir = ray.d;

	r.o = ray.o + ray.d*hit.tHit;
	r.d = dir - Vector(2.f*(Dot(dir, hit.normal))*hit.normal);
	r.d = Normalize(r.d + jitter);
	r.depth = ray.depth;
	return r;
}
