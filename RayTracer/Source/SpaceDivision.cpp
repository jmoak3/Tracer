#include "SpaceDivision.h"
#include <assert.h>
SpaceDivision::SpaceDivision(std::vector<Primitive*> * scene)
{
	//Find objects inside our BB from Scene
	Bounds = BoundingBox(GetSceneBounds(scene), false);
	std::vector<Primitive*>::iterator iScene;
	for (iScene = scene->begin(); iScene != scene->end(); ++iScene)
	{
		Point p = (*((*iScene)->ObjectToWorld))(Point(0.0f,0.f,0.f));
		if (Bounds.Contains(p))
			Objects.push_back(*(iScene));
	}
}

SpaceDivision::SpaceDivision(const BoundingBox &box, std::vector<Primitive*> * scene, SpaceDivision* adj)
{
	//Find objects inside our BB from Scene
	Bounds = box;
	std::vector<Primitive*>::iterator iScene;
	for (iScene = scene->begin(); iScene != scene->end(); ++iScene)
	{
		Point p = (*((*iScene)->ObjectToWorld))(Point(0.0f,0.f,0.f));
		if (Bounds.Contains(p))
			Objects.push_back(*(iScene));
	}

	Adjacent = adj;
}

BoundingBox SpaceDivision::GetSceneBounds(std::vector<Primitive*> * scene)
{
	BoundingBox bounds;
	std::vector<Primitive*>::iterator iScene;
	for (iScene = scene->begin(); iScene != scene->end(); ++iScene)
	{
		bounds = Union(bounds, (*iScene)->WorldBound());
	}
	return bounds;
}


bool SpaceDivision::ShouldSplit() const
{
	return Objects.size() & ~15; // #realProgrammer
}

//return 8 space divisions thru pointer!
SpaceDivision *SpaceDivision::GetSplit() 
{
	SpaceDivision * divisions = new SpaceDivision[8];
	SpaceDivision ** adjacent = new SpaceDivision*[48];
	//get a,b,c, and x
	Point center = Bounds.GetCenter();
	float a = center.x;
	float b = center.y;
	float c = center.z;
	float x = center.x + Bounds.Min.x;
	
	//NO COPIES COPIES ARE EVIL AND SLOW AND BAD

	//Make 8 spacedivs
	SpaceDivision** adj1 = &adjacent[0];
	adj1[0] = &divisions[3];
	adj1[1] = &Adjacent[1];
	adj1[2] = &Adjacent[2];
	adj1[3] = &Adjacent[3];
	adj1[4] = &divisions[5];
	adj1[5] = &divisions[1];
	divisions[0] = SpaceDivision(BoundingBox(Point(-x+a,b,-x+c), Point(a,x+b,c)), &Objects, *adj1);

	SpaceDivision** adj2 = &adjacent[6];
	adj2[0] = &divisions[2];
	adj2[1] = &Adjacent[1];
	adj2[2] = &divisions[0];
	adj2[3] = &Adjacent[3];
	adj2[4] = &Adjacent[4];
	adj2[5] = &divisions[5];
	divisions[1] = SpaceDivision(BoundingBox(Point(a,b,-x+c), Point(x+a,x+b,c)), &Objects, *adj2);
	
	SpaceDivision** adj3 = &adjacent[12];
	adj3[0] = &Adjacent[0];
	adj3[1] = &Adjacent[1];
	adj3[2] = &divisions[3];
	adj3[3] = &divisions[1];
	adj3[4] = &divisions[6];
	adj3[5] = &Adjacent[5];
	divisions[2] = SpaceDivision(BoundingBox(Point(a,-x+b,-x+c), Point(x+a,b,c)), &Objects, *adj3);

	SpaceDivision** adj4 = &adjacent[18];
	adj4[0] = &Adjacent[0];
	adj4[1] = &Adjacent[1];
	adj4[2] = &Adjacent[2];
	adj4[3] = &divisions[0];
	adj4[4] = &divisions[7];
	adj4[5] = &divisions[2];
	divisions[3] = SpaceDivision(BoundingBox(Point(-x+a,-x+b,-x+c), Point(a,b,c)), &Objects, *adj4);
	
	SpaceDivision** adj5 = &adjacent[24];
	adj5[0] = &divisions[7];
	adj5[1] = &divisions[0];
	adj5[2] = &Adjacent[2];
	adj5[3] = &Adjacent[3];
	adj5[4] = &Adjacent[4];
	adj5[5] = &divisions[5];
	divisions[4] = SpaceDivision(BoundingBox(Point(-x+a,b,c), Point(a,x+b,x+c)), &Objects, *adj5);

	SpaceDivision** adj6 = &adjacent[30];
	adj6[0] = &divisions[6];
	adj6[1] = &divisions[1];
	adj6[2] = &divisions[4];
	adj6[3] = &Adjacent[3];
	adj6[4] = &Adjacent[4];
	adj6[5] = &Adjacent[5];
	divisions[5] = SpaceDivision(BoundingBox(Point(a,b,c), Point(x+a,x+b,x+c)), &Objects, *adj6);
	
	SpaceDivision** adj7 = &adjacent[36];
	adj7[0] = &Adjacent[0];
	adj7[1] = &divisions[2];
	adj7[2] = &divisions[7];
	adj7[3] = &divisions[5];
	adj7[4] = &Adjacent[4];
	adj7[5] = &Adjacent[5];
	divisions[6] = SpaceDivision(BoundingBox(Point(a,-x+b,c), Point(x+a,b,x+c)), &Objects, *adj7);
	
	SpaceDivision** adj8 = &adjacent[42];
	adj8[0] = &Adjacent[0];
	adj8[1] = &divisions[3];
	adj8[2] = &Adjacent[2];
	adj8[3] = &divisions[4];
	adj8[4] = &Adjacent[4];
	adj8[5] = &divisions[6];
	divisions[7] = SpaceDivision(BoundingBox(Point(-x+a,-x+b,c), Point(a,b,x+c)), &Objects, *adj8);
	
	return divisions;
}

SpaceDivision* SpaceDivision::GetNextDivision(const Ray &ray) 
{
	// How to find adjacent ones, fast?
	// Have references to up, forward, right, and others cached in setup!!
	//PRECACHE Adjacent list through GetSplit's SpaceDivision custom constructor
	
	SpaceDivision * next = NULL;
	
	Hit bestHit, currHit;
	int bestHitIndex = 9;
	for (int i=0;i<6;++i)
	{
		if (Adjacent[i].Bounds.CanIntersect())
		{
			Adjacent[i].Bounds.Intersect(ray, &currHit);
			bestHit = currHit.tHit < bestHit.tHit ? currHit : bestHit;
			bestHitIndex = currHit.tHit < bestHit.tHit ? i : bestHitIndex;
		}
	}
	assert(bestHitIndex<6);
	next = &Adjacent[bestHitIndex];
	return next;
}