#ifndef SPACEDIVISION_H
#define SPACEDIVISION_H
#include "Primitive.h"
#include <vector>

class SpaceDivision
{
public:
	SpaceDivision();
	SpaceDivision(std::vector<Primitive*> *scene);
	SpaceDivision(const BoundingBox & box, std::vector<Primitive*> *scene, SpaceDivision** adj);
	~SpaceDivision() {};
	
	SpaceDivision* GetNextDivision(const Ray &ray);
	BoundingBox GetSceneBounds(std::vector<Primitive*> *scene);
	bool ShouldSplit() const;
	SpaceDivision * GetSplitA();
	SpaceDivision * GetSplitB();

	//Yolo I guess
	//std::vector<SpaceDivision> Adjacent;
	SpaceDivision** Adjacent;

	BoundingBox Bounds;
	std::vector<Primitive*> Objects;
};


//ADJACENCY IS AS FOLLOWS: 
//0 down
//1 backwards
//2 left
//3 up
//4 forward
//5 right
/*
1 2
4 3
	5 6
	8 7
	*/
#endif

