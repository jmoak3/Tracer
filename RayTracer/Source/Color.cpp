#include "Color.h"

Colors::Colors(const RGB &diff, const RGB &spec, const RGB &amb)
{
	Diffuse = diff;
	Specular = spec;
	Ambient = amb;
}
