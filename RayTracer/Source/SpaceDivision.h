#ifndef SPACEDIVISION_H
#define SPACEDIVISION_H
#include "Primitive.h"
#include <vector>

class SpaceDivision
{
public:
	SpaceDivision() {Bounds = BoundingBox();}
	SpaceDivision(std::vector<Primitive*> *scene);
	SpaceDivision(const BoundingBox & box, std::vector<Primitive*> *scene);
	~SpaceDivision() {};
	
	BoundingBox GetSceneBounds(std::vector<Primitive*> *scene);
	bool ShouldSplit() const;
	SpaceDivision * GetSplit();

	BoundingBox Bounds;
	std::vector<Primitive*> Objects;
};

#endif
