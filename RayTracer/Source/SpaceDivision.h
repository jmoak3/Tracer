#ifndef SPACEDIVISION_H
#define SPACEDIVISION_H
#include "Primitive.h"
#include <vector>

class SpaceDivision
{
public:
	SpaceDivision() {Bounds = BoundingBox();}
	SpaceDivision(const BoundingBox &bbox, std::vector<Primitive*> *scene);
	~SpaceDivision() {};
	
	bool ShouldSplit() const;
	SpaceDivision * GetSplit();

	BoundingBox Bounds;
	std::vector<Primitive*> Objects;
};

#endif
