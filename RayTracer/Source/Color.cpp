#include "Color.h"
#include "Ray.h"
#include <math.h>
#include "Renderer.h"
#include <assert.h>

inline float ra()
{
	return (float)((rand()/(float)RAND_MAX));
}

Material::Material(const RGB &col, const float spec, const float diffuse, const float emissive, const float reflective, const float diffuseReflective)
{
	assert(spec <= 1.0f && spec >= 0.f 
		&& diffuse <= 1.0f && diffuse >= 0.f
		&& reflective <= 1.0f && reflective >= 0.f 
		&& diffuseReflective <= 1.0f && diffuseReflective >= 0.f);
	Color = col;
	Specular = spec;
	Diffuse = diffuse;
	Reflective = reflective;
	DiffuseReflective = diffuseReflective;
	Emissive = emissive;
}

Ray Material::ReflectRay(const Ray &ray, const Hit &hit)
{
	Ray r(ray.o, ray.d, ray.mint, ray.maxt, ray.time, ray.depth+1);
	
	//Save the trouble
	if (DiffuseReflective < 0.01f || ray.depth != 0)
	{
		r.o = ray.o + ray.d*hit.tHit;
		r.d = Normalize(ray.d - Vector(2.f*(Dot(ray.d, hit.normal))*hit.normal));
		return r;
	}

	Vector basis1 = Dot(hit.normal, r.d) < 0 ?
							Vector(hit.normal) :
							-1.f*Vector(hit.normal);

	Vector basis2 = Cross(Vector(0.f, 1.f, 0.f), basis1).HasNans()  ? 
									Cross(basis1, Vector(1.f, 0.f, 0.f)) :
									Cross(basis1, Vector(0.f, 1.f, 0.f));
	basis2 = Normalize(basis2);

	Vector basis3 = Normalize(Cross(basis2, basis1));

	float u = ra()*6.28319f;
	float v = ra();
	float w = sqrt(v);
	Vector jitter = Normalize(basis2*cosf(u)*w + basis3*sinf(u)*w + basis1*(1-v));
	
	r.o = ray.o + ray.d*hit.tHit;
	Vector properReflection = Normalize(ray.d - Vector(2.f*(Dot(ray.d, hit.normal))*hit.normal));
	
	r.d = Normalize(Lerp(properReflection, jitter, DiffuseReflective));
	
	/*float dx = ra()*DiffuseReflective;
	float dy = ra()*DiffuseReflective;
	while ((dx*dx) + (dy*dy) > (DiffuseReflective*DiffuseReflective))
	{
		dx = ra()*DiffuseReflective;
		dy = ra()*DiffuseReflective;
	}

	Vector v1 = ray.d - Vector(2.f*(Dot(ray.d, hit.normal))*hit.normal);
	Vector v2 = Vector(v1.z, v1.y, -v1.x);
	Vector v3 = Cross(v1, v2);
	r.d = Normalize(v1 + v2*dx + v3*dy*DiffuseReflective);
	r.o = ray.o + ray.d*hit.tHit;*/

	return r;
}
