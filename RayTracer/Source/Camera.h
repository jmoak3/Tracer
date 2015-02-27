#ifndef CAMERA_H
#define CAMERA_H
#include "Vector.h"
#include "Point.h"
#include "Transform.h"
//FIX ME
class Camera
{
public:
	Camera(const Transform &proj)
	~Camera() {};
	
	
private:
	Transform CameraToScreen, RasterToCamera;
	Transform ScreenToRaster, RasterToScreen;
	float Fov, NearPlane, FarPlane, AspectRatio;
};

#endif