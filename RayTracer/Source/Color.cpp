#include "Color.h"
#include "Ray.h"
#include <math.h>
#include "Renderer.h"
#include <assert.h>

inline float ra()
{
	return (float)((rand()/(float)RAND_MAX))-0.5f;
}

Material::Material(const RGB &col, const float spec, const float diffuse, const float emissive, const float reflective, const float glossyReflective)
{
	assert(spec <= 1.0f && spec >= 0.f 
		&& diffuse <= 1.0f && diffuse >= 0.f
		&& reflective <= 1.0f && reflective >= 0.f 
		&& glossyReflective <= 1.0f && glossyReflective >= 0.f);
	Color = col;
	Specular = spec;
	Diffuse = diffuse;
	Reflective = reflective;
	GlossyReflective = glossyReflective;
	Emissive = emissive;
}

Ray Material::ReflectRay(const Ray &ray, const Hit &hit) const
{
	Ray r(Point(), Vector(), ray.mint, ray.maxt, ray.time, ray.depth+1);

	//Save the trouble
	if (Reflective < 0.001f || GlossyReflective < 0.001f || ray.depth != 0)
	{
		r.o = ray.o + ray.d*hit.tHit;
		//r.d = Normalize(ray.d - static_cast<Vector>(hit.normal*Dot(ray.d, hit.normal)*2.f));
		r.d = Normalize(ray.d - Vector(hit.normal*Dot(ray.d, hit.normal)*2.f));
		return r;
	}

	/*Vector basis1 = Dot(hit.normal, r.d) < 0 ?
							Vector(hit.normal) :
							-1.f*Vector(hit.normal);
	Vector basis2 = Cross(Vector(0.f, 1.f, 0.f), basis1).HasNans() ?
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
	r.d = Normalize(Lerp(properReflection, jitter, GlossyReflective));
	*/

	float dx = ra()*GlossyReflective;
	float dy = ra()*GlossyReflective;
	int tries = 0;
	while ((dx*dx) + (dy*dy) > (GlossyReflective*GlossyReflective) && tries < 5)
	{
		dx = ra()*GlossyReflective; 
		dy = ra()*GlossyReflective;
		++tries;
	}
	if (tries >= 5)
	{
		dx = 0; 
		dy = 0;
	}

	//Vector v1 = ray.d - static_cast<Vector>(2.f*(Dot(ray.d, hit.normal))*hit.normal);
	Vector v1 = ray.d - Vector(2.f*(Dot(ray.d, hit.normal))*hit.normal);
	//Vector v2(v1.z, v1.y, -v1.z);
	Vector temp = Vector(0.0f, 0.0f, 1.0f);
	if (v1.z == temp.z) temp = Vector(0.0f, 1.f, 0.f);
	Vector v2 = Cross(v1, temp);
	Vector v3 = Cross(v2, v1);
	r.d = Normalize(v1 + v2*dx + v3*dy*GlossyReflective);
	r.o = ray.o + ray.d*hit.tHit;
	
	return r;
}

