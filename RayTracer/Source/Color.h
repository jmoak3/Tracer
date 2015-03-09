#ifndef COLOR_H	
#define COLOR_H

class Ray;
struct Hit;
class Vector;

struct RGB
{
	int red=0, green=0, blue=0;
	RGB operator*(const float f)
	{
		RGB c;
		c.red = f * red;
		c.blue = f * blue;
		c.green = f * green;
		return c;
	}
	RGB operator*(const RGB & f)
	{
		RGB c;
		c.red = f.red * red;
		c.blue = f.blue * blue;
		c.green = f.green * green;
		return c;
	}
	RGB operator+(const RGB &col)
	{
		RGB c;
		c.red = col.red + red;
		c.blue = col.blue + blue;
		c.green = col.green + green;
		return c;
	}
	RGB operator-(const RGB &col)
	{
		RGB c;
		c.red = red - col.red;
		c.blue = blue - col.blue;
		c.green = green - col.green;
		return c;
	}
	RGB& operator*=(const float f)
	{
		*this = *this * f;
		return *this;
	}
	RGB& operator+=(const RGB &col)
	{
		*this = *this + col;
		return *this;
	}
	RGB& operator-=(const RGB &col)
	{
		*this = *this - col;
		return *this;
	}
};

class Material
{
public:
	Material() {};
	~Material() {};
	Material(const RGB &diff, const float spec, const float rough);
	
	Ray ReflectRay(const Ray &ray, const Hit &hit);
	RGB Color;
	float Specular, Rough;
	static RGB bg;

};

#endif
