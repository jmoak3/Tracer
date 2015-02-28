#ifndef LIGHT_H	
#define LIGHT_H
#include "Point.h"
#include "Inlines.h"

class Light
{
public:
	Light(const Point &p, const RGB &color);
	Point Position;
	RGB Color;
};

#endif
