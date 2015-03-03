#ifndef LIGHT_H	
#define LIGHT_H
#include "Point.h"
#include "Color.h"

class Light
{
public:
	Light() {};
	~Light() {};
	Light(const Point &p, const Material &color);
	Point Position;
	Material Color;
};

#endif
