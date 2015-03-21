#ifndef COLOR_H	
#define COLOR_H

class Ray;
struct Hit;
class Vector;
#include <algorithm>

struct RGB
{
	float red=0.f, green=0.f, blue=0.f;
	RGB() 
	{
		red = 0.f; 
		green = 0.f; 
		blue = 0.f;
	}
	~RGB() {}
	RGB(float r, float g, float b)
	{
		red = r;
		green = g;
		blue = b;
	}
	RGB operator*(float f) const
	{
		RGB c;
		c.red = f * red;
		c.blue = f * blue;
		c.green = f * green;
		return c;
	}
	RGB operator*(const RGB & col) const
	{
		RGB c;
		c.red = col.red * red;
		c.blue = col.blue * blue;
		c.green = col.green * green;
		return c;
	}
	RGB operator+(const RGB &col) const
	{
		RGB c;
		c.red = col.red + red;
		c.blue = col.blue + blue;
		c.green = col.green + green;
		return c;
	}
	RGB operator-(const RGB &col) const
	{
		RGB c;
		c.red = red - col.red;
		c.blue = blue - col.blue;
		c.green = green - col.green;
		return c;
	}
	RGB& operator*=(float f)
	{
		*this = *this * f;
		return *this;
	}
	RGB& operator*=(const RGB & col)
	{
		*this = *this * col;
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

	void Bound(float min, float max)
	{
		red = std::min(red, max);
		green = std::min(green, max);
		blue = std::min(blue, max);

		red = std::max(red, min);
		green = std::max(green, min);
		blue = std::max(blue, min);
	}

	bool IsBlack() const
	{
		return (red < 0.001f && green < 0.001f && blue < 0.001f);
	}
};

class Material
{
public:
	Material() {Specular = 1.f; Diffuse = 1.f; Reflective = 1.f; 
	GlossyReflective = 0.0f; Emissive = 0.f; 
	Refractive = 1.0f; RefrAbsorbance = 1.f;};
	~Material() {};
	
	Ray ReflectRay(const Ray &ray, const Hit &hit) const;
	Ray RefractRay(const Ray &ray, const Hit &hit, bool * isValid) const;
	RGB Color;
	float Specular, Diffuse, Reflective, GlossyReflective, Refractive, RefrAbsorbance;
	float Emissive;
};

#endif
