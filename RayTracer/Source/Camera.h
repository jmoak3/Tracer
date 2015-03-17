#ifndef CAMERA_H
#define CAMERA_H
#include "Vector.h"
#include "Point.h"
#include "Transform.h"
//FIX ME
class Camera
{
public:
	Camera() {};
	Camera(const Transform &proj, const int width, const int height, float focalDist);
	~Camera() {};
	
	Transform ScreenToWorld, WorldToScreen;
	Transform RasterToScreen, RasterToWorld; 
	Transform WorldToFarPlane;
	int width, height, focalDist;

};

#endif