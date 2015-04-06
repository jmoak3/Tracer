#include "RayRenderer.h"
#include "Sphere.h"

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

RayRenderer::RayRenderer(std::vector<Primitive*>* scene, const Camera &ccamera, const QualityDesc &quality)
		:Renderer(scene, ccamera, quality)
{
	Lights = new std::vector<Primitive*>();
	std::vector<Primitive*>::iterator iScene;
	for (iScene = Scene->begin(); iScene!=Scene->end(); ++iScene)
	{	
		if ((*iScene)->Type == 1)
			Lights->push_back((*iScene));
	}
	LightSamples = quality.LightSamples;
	GlossyReflectiveSamples = quality.GlossyReflectiveSamples;
	Depth = quality.Depth;
	InvLightSamples = 1.f/(float)LightSamples;
	InvGlossyReflectiveSamples = 1.f/(float)GlossyReflectiveSamples;
}

void RayRenderer::Render()
{
	const int height = Cam.height;
	const int width = Cam.width;
	printf("%ix%i Ray %i AA Samples, %i LightSamples, %i GlossyReflectionSamples, %i Depth\n", width, height, Samples, LightSamples, GlossyReflectiveSamples, Depth);
	Renderer::Render();
}

RGB RayRenderer::computeColor(const Ray &reflRay, const Hit &hit)
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
		
		RGB sampleColor;
		for (int i=0;i<LightSamples;++i)
		{
			Vector jitter = Vector(r(), r(), r());
			float shouldJitter = 
				std::min(((float)LightSamples-1.f), 1.f);
			jitter = radius*Normalize(jitter)*(shouldJitter);

			Vector l = ((lightPos+jitter) - hitPos);
			float distToLight = l.Length();
			l = Normalize(l);

			if (ShadowTest(Ray(hitPos, l, hit.eps, distToLight)))
				continue;

			Vector r = Vector(2.f*Dot(l, normal)*normal) - l;
			
			float spec = pow(std::min(1.0f, std::max(0.f, Dot(r, v))),20.f);
			RGB specular = lightMaterial.Color*shapeMaterial.Specular*spec;

			float diff = std::min(1.0f, std::max(0.f, Dot(normal, l)));
			RGB diffuse;
			if (shapeMaterial.Diffuse < 0.f)
				diffuse = shapeMaterial.Color;
			else
				diffuse = shapeMaterial.Color*diff*shapeMaterial.Diffuse*lightMaterial.Color;
			
			sampleColor += diffuse + specular;
			sampleColor.Bound(MIN_COLOR, MAX_COLOR);
		}
		finalColor += sampleColor*InvLightSamples;
	}

	return finalColor;
}

RGB RayRenderer::Trace(const Ray &reflRay)
{
	if (reflRay.depth > Depth)
		return RGB();

	Hit bestHit;
	if (FindClosest(reflRay, &bestHit))
	{
		RGB sampleColor;
		int GlossyReflectiveSamplesToRun = 1;
		bool isGlossyReflective = 
							(reflRay.depth == 0 
							&& bestHit.material.GlossyReflective > 0.001f
							&& bestHit.material.Reflective > 0.001f);

		if (isGlossyReflective)  //Only primary rays should diffuse... it's very slow!
			GlossyReflectiveSamplesToRun = GlossyReflectiveSamples;
		RGB absorb = bestHit.material.Color*bestHit.material.RefrAbsorbance*-bestHit.tHit;
		RGB transparency = RGB(expf(absorb.red), expf(absorb.green), expf(absorb.blue));
		for (int i=0;i<GlossyReflectiveSamplesToRun;++i)
		{
			RGB c = computeColor(reflRay, bestHit);
			
			//Early out before calcing the refr and refl!
			if (reflRay.depth+1 <= Depth) 
			{
				bool isRefr = (bestHit.material.RefrAbsorbance<1.f);
				Ray nextReflRay = bestHit.material.ReflectRay(reflRay, bestHit, false);
				Ray nextRefrRay = bestHit.material.RefractRay(reflRay, bestHit, &isRefr);
				float refl = bestHit.material.Reflective;
				bool canRefl = refl > 0.f && !c.IsBlack();
				bool canRefr = isRefr && !transparency.IsBlack();
				sampleColor += c + 
					(canRefl ? 	c*Trace(nextReflRay)*refl : RGB()) 
					+ (canRefr ? transparency*Trace(nextRefrRay) : RGB());
				//OPTIMIZE FURTHER!
			}
			else
				sampleColor += c;
		}

		if (isGlossyReflective) // If a primary ray we have done diffusion so average
			sampleColor *= InvGlossyReflectiveSamples;

		return sampleColor;
	}

	return RGB();
}

bool RayRenderer::ShadowTest(const Ray &ray) 
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
