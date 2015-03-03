#ifndef COLOR_H	
#define COLOR_H

class Ray;
struct Hit;
class Vector;

struct RGB
{
	int red, green, blue;
};

class Material
{
public:
	Material() {};
	~Material() {};
	Material(const RGB &diff, const RGB &spec, const RGB &amb);
	
	Ray ReflectRay(const Ray &ray, const Hit &hit, const Vector & jitter);
	RGB Diffuse, Specular, Ambient;
	static RGB bg;

};

#endif
