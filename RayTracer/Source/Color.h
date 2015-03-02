#ifndef COLOR_H	
#define COLOR_H

struct RGB
{
	int red, green, blue;
};

class Colors
{
public:
	Colors() {};
	~Colors() {};

	Colors(const RGB &diff, const RGB &spec, const RGB &amb);
	RGB Diffuse, Specular, Ambient;
};

#endif
