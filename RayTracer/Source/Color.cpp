#include "Color.h"
#include "Ray.h"

Material::Material(const RGB &diff, const RGB &spec, const RGB &amb)
{
	Diffuse = diff;
	Specular = spec;
	Ambient = amb;
}

Ray Material::ReflectRay(const Ray &ray, const Hit &hit, const Vector & jitter)
{
	Ray r(ray.o, ray.d, 0.f);
	
	Vector dir = ray.d;

	r.o = ray.o + ray.d*hit.tHit;
	r.d = dir - Vector(2.f*(Dot(dir, hit.normal))*hit.normal);
	r.d = Normalize(r.d + jitter);
	r.depth = ray.depth;
	return r;
}