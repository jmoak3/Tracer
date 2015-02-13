#ifndef CAMERA_H
#define CAMERA_H
#include "Vector.h"
#include "Point.h"

//FIX ME
class Camera
{
public:
	Camera(const Transform *w2o, const Transform *o2w, const float fov, const float near, const float &far);
	~Camera() {};
	
	
private:
	float Fov, NearPlane, FarPlane, AspectRatio, 
};

#endif