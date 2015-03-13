#include "Sphere.h"
#include "Point.h"
#include "Ray.h"
#include "Inlines.h"
#include "Transform.h"
#include "Color.h"

Sphere::Sphere(const Transform *o2w, const Material &material, const float r)
	: Shape(o2w, material)
{
	Radius = r;
	if (material.Emissive>0.f) Type = 1;
}

bool Sphere::Intersect(const Ray &ray, Hit *hit) const
{
	//if (!CanIntersect(ray))
	//	return false;

	Ray r;
	WorldToObject(ray, &r);
	r.maxt = ray.maxt;

	float A = r.d.x*r.d.x + r.d.y*r.d.y + r.d.z*r.d.z;
	float B = 2.f*(r.d.x*r.o.x + r.d.y*r.o.y + r.d.z*r.o.z);
	float C = r.o.x*r.o.x + r.o.y*r.o.y + r.o.z*r.o.z - Radius*Radius;
	
	float t0, t1 = 0.f;
	if (!Quadratic(A, B, C, &t0, &t1))
		return false;

	if (t0 > r.maxt || t1 < r.mint)
		return false;
	float thit = t0;
	if (t0 < r.mint)
	{
		thit = t1;
		if (thit > r.maxt)
			return false;
	}
	
	Point hitOnSphere = r.o + r.d*thit;
	
	Normal normal(hitOnSphere-Point(0.f, 0.f, 0.f));
	normal = Normalize(normal);
	
	ObjectToWorld(normal, &normal);

	hit->tHit = thit;
	hit->normal = normal;
	hit->material = GetMaterial();
	hit->shapeID = ShapeID;
	hit->eps = 5e-4 * thit;
	hit->type = Type;
	return true;
}

bool Sphere::CanIntersect(const Ray &ray) const
{
	return GetBBox().Intersect(ray);
}

Material Sphere::GetMaterial() const
{
	return Mat;
}

BoundingBox Sphere::GetBBox() const
{
	return BoundingBox(Point(-Radius, -Radius, -Radius), Point(Radius, Radius, Radius));
}