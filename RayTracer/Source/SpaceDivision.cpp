#include "SpaceDivision.h"

SpaceDivision::SpaceDivision(const BoundingBox & bbox, std::vector<Primitive*> * scene)
{
	//Find objects inside our BB from Scene
	Bounds = bbox;
	std::vector<Primitive*>::iterator iScene;
	for (iScene = scene->begin(); iScene != scene->end(); ++iScene)
	{
		Point p = (*((*iScene)->ObjectToWorld))(Point(0.0f,0.f,0.f));
		if (Bounds.Contains(p))
			Objects.push_back(*(iScene));
	}
}

bool SpaceDivision::ShouldSplit() const
{
	return Objects.size() & ~15; // So proud
}

//return 8 space divisions thru pointer!
SpaceDivision *SpaceDivision::GetSplit() 
{
	SpaceDivision * divisions = new SpaceDivision[8];//(SpaceDivision*)malloc(sizeof(SpaceDivision));
	
	//get a,b,c, and x
	Point center = Bounds.GetCenter();
	float a = center.x;
	float b = center.y;
	float c = center.z;
	float x = center.x + Bounds.Min.x;
	
	//Make 8 spacedivs
	divisions[0] = SpaceDivision(BoundingBox(Point(-x+a,b,-x+c), Point(a,x+b,c)), &Objects);
	divisions[1] = SpaceDivision(BoundingBox(Point(a,b,-x+c), Point(x+a,x+b,c)), &Objects);
	divisions[2] = SpaceDivision(BoundingBox(Point(a,-x+b,-x+c), Point(x+a,b,c)), &Objects);
	divisions[3] = SpaceDivision(BoundingBox(Point(-x+a,-x+b,-x+c), Point(a,b,c)), &Objects);
	divisions[4] = SpaceDivision(BoundingBox(Point(-x+a,b,c), Point(a,x+b,x+c)), &Objects);
	divisions[5] = SpaceDivision(BoundingBox(Point(a,b,c), Point(x+a,x+b,x+c)), &Objects);
	divisions[6] = SpaceDivision(BoundingBox(Point(a,-x+b,c), Point(x+a,b,x+c)), &Objects);
	divisions[7] = SpaceDivision(BoundingBox(Point(-x+a,-x+b,c), Point(a,b,x+c)), &Objects);
	return divisions;
}
