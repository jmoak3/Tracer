#include "Color.h"
#include "Ray.h"
#include <math.h>
#include "Renderer.h"
#include <assert.h>

inline float ra()
{
	return (float)((rand()/(float)RAND_MAX))-0.5f;
}

Ray Material::ReflectRay(const Ray &ray, const Hit &hit) const
{
	Ray r(Point(), Vector(), hit.eps, ray.maxt, ray.time, ray.depth+1, hit.material.Refractive);

	//Save the trouble
	if (Reflective < 0.001f || GlossyReflective < 0.001f || ray.depth != 0)
	{
		r.o = ray.o + ray.d*hit.tHit;
		r.d = Normalize(ray.d - Vector(hit.normal*Dot(ray.d, hit.normal)*2.f));
		return r;
	}

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


Ray Material::RefractRay(const Ray &ray, const Hit &hit, bool * isValid) const
{
	Ray r(Point(), Vector(), hit.eps, ray.maxt, ray.time, ray.depth+1, hit.material.Refractive);
	float n = ray.refrIndex/r.refrIndex;
	Normal N(hit.normal);
	float cosInc = -Dot(N, ray.d);
	if (cosInc < 0.f) 
	{
		N = -N;
		cosInc = -Dot(N, ray.d);
	}
	float cosT2 = 1.f - n*n*(1.f - (cosInc*cosInc));
	if (cosT2 <= 0.f || !(*isValid))
	{
		*isValid = false;
		return Ray();
	}
	*isValid = true;
	r.o = ray.o + ray.d*hit.tHit;
	r.d = Normalize(n*ray.d + Vector((n*cosInc - sqrt(cosT2))*N));
	return r;
}

