#ifndef SCRIPTING_H
#define SCRIPTING_H

#include "Renderer.h"

Renderer* init_renderer_from_script(char const * filename, std::string* err);

#endif