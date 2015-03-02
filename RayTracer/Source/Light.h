#ifndef LIGHT_H	
#define LIGHT_H
#include "Point.h"
#include "Color.h"

class Light
{
public:
	Light() {};
	~Light() {};
	Light(const Point &p, const Colors &color);
	Point Position;
	Colors Color;
};

#endif
