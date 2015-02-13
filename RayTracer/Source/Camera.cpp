#include "Camera.h"
#include <math.h>
#include "Point.h"

Camera(const Transform *w2o, const Transform *o2w, const float fov, const float &near, const float &far)
	: WorldToObject(w2o), ObjectToWorld(o2w)
{
	
}