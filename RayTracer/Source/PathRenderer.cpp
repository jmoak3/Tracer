#include "PathRenderer.h"
#include "Sphere.h"
#include <utility>
#define MAX_COLOR 0.999f
#define MIN_COLOR 0.078f
inline float r()
{
	return (float)((rand()/(float)RAND_MAX)-0.5f);
}

inline float r1()
{
	return (float)((rand()/(float)RAND_MAX));
}

PathRenderer::PathRenderer(std::vector<Primitive*>* scene, const Camera &ccamera, const QualityDesc &quality)
		:Renderer(scene, ccamera, quality)
{
	Lights = new std::vector<Primitive*>();
	std::vector<Primitive*>::iterator iScene;
	for (iScene = Scene->begin(); iScene!=Scene->end(); ++iScene)
	{	
		if ((*iScene)->Type == 1)
			Lights->push_back((*iScene));
	}
}

void PathRenderer::Render()
{
	const int height = Cam.height;
	const int width = Cam.width;
	printf("%ix%i Path %i Samples\n", width, height, Samples);
	Renderer::Render();
}


RGB PathRenderer::CalcDirectLighting(const Ray &reflRay, const Hit &hit)
{
	Normal normal = hit.normal;
	Material shapeMaterial = hit.material;
	Point hitPos = reflRay.o + hit.tHit*reflRay.d;
	Vector v = -reflRay.d;
	if (Dot(normal, reflRay.d) > 0.f && hit.material.RefrAbsorbance > 0.9f)
		normal = -normal;

	RGB finalColor;
	std::vector<Primitive*>::iterator iLight;
	if (hit.type == 1) 
		return hit.material.Color;

	for (iLight = Lights->begin(); iLight!=Lights->end(); ++iLight)
	{	
		Sphere * currLight = dynamic_cast<Sphere*>(*iLight);
		//FIX, SPHERE IS HARDCODED IN LIGHT CALCS!!!
		Material lightMaterial = currLight->GetMaterial();
		float radius = currLight->Radius;
		Point lightPos = (*currLight->ObjectToWorld)(Point(0.f, 0.f, 0.f));
		
		Vector jitter = Vector(r(), r(), r());
		jitter = radius*Normalize(jitter);
		
		Vector l = ((lightPos+jitter) - hitPos);
		float distToLight = l.Length();
		l = Normalize(l);

		if (ShadowTest(Ray(hitPos, l, hit.eps, distToLight)))
			continue;

		Vector r = Vector(2.f*Dot(l, normal)*normal) - l;
		
		RGB specular = RGB();
		if (shapeMaterial.Specular > 0.001f)
		{
			float spec = pow(std::min(1.0f, std::max(0.f, Dot(r, v))),20.f);
			specular = lightMaterial.Color*shapeMaterial.Specular*spec*lightMaterial.Color*lightMaterial.Emissive*0.15915f;
		}

		RGB diffuse = RGB();
		if (shapeMaterial.Diffuse > 0.001f)
		{
			float diff = std::min(1.0f, std::max(0.f, Dot(normal, l)));
			diffuse = shapeMaterial.Color*diff*shapeMaterial.Diffuse*lightMaterial.Color*lightMaterial.Emissive*0.31831f;
		}	

		float invsize = 1.f;//Lights->size();
		finalColor += (diffuse + specular)*invsize;
		//finalColor.Bound(MIN_COLOR, MAX_COLOR);
	}

	return finalColor;
}

RGB PathRenderer::Trace(const Ray &reflRay)
{
	if (Quality.PathEnableIndirectIllum && Quality.PathEnableDirectLighting)
		return DirectLightIndirectIllumTrace(reflRay);

	Hit bestHit;
	if (FindClosest(reflRay, &bestHit))
	{
		if (bestHit.type == 1)
			return bestHit.material.Color*bestHit.material.Emissive;

		RGB c = bestHit.material.Color;
		float maxRefl = c.red>c.green && c.red>c.blue ? c.red : c.green>c.blue ? c.green : c.blue;
		if (Quality.PathEnableDirectLighting)
		{
			if (reflRay.depth+1>2)
			{	
				if (r1() < maxRefl*0.9f) 
					c *= (0.9f*maxRefl);
				else 
					return RGB(true);
			}
		}
		else
		{
			if (reflRay.depth+1>5)
			{	
				if (r1() < maxRefl*0.9f) 
					c *= (0.9f*maxRefl);
				else 
					return RGB(true);
			}
		}

		Ray nextReflRay = bestHit.material.ReflectRay(reflRay, bestHit, true);

		if (Quality.PathEnableDirectLighting)
		{
			c += CalcDirectLighting(reflRay, bestHit);
			c.Bound(MIN_COLOR, MAX_COLOR);
		}

		return c*Trace(nextReflRay);
	}
	else 
		return RGB();
}

//Only do Indirect Illumination if DirectLight also enabled.
//Saves on branching in above function, and it looks like 
//garbage alone, anyway
RGB PathRenderer::DirectLightIndirectIllumTrace(const Ray &reflRay)
{
	Hit bestHit;
	if (FindClosest(reflRay, &bestHit))
	{
		if (bestHit.type == 1 && reflRay.depth>0)
			return RGB();
		if (bestHit.type == 1 && reflRay.depth==0)
			return bestHit.material.Color*bestHit.material.Emissive;

		RGB c = RGB();// = bestHit.material.Color;
		float maxRefl = c.red>c.green && c.red>c.blue ? c.red : c.green>c.blue ? c.green : c.blue;
		
		float rr = 1.f;
		if (reflRay.depth+1>2)
		{	
			if (r1() < maxRefl) 
				rr = (1.f/maxRefl);
			else 
				return RGB();
		}
		//rr = std::min(1.f, rr);
		Ray nextReflRay = bestHit.material.ReflectRay(reflRay, bestHit, true);

		c += CalcDirectLighting(reflRay, bestHit);
		c += DirectLightIndirectIllumTrace(nextReflRay)*
					2.f*Dot(bestHit.normal, nextReflRay.d)*bestHit.material.Color*rr;
		//c.Bound(MIN_COLOR, MAX_COLOR);
		return c;
	}
	else 
		return RGB();
}


bool PathRenderer::ShadowTest(const Ray &ray) 
{
	std::vector<Primitive*>::iterator iScene;
	Hit currHit;
	for (iScene = Scene->begin(); iScene!=Scene->end(); ++iScene)
	{	
		if ((*iScene)->Intersect(ray, &currHit) && currHit.type == 0 && currHit.material.RefrAbsorbance > 0.4f)
			return true;
	}
	return Root->Intersect(ray, &currHit);
}
