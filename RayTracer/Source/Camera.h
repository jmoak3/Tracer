#ifndef CAMERA_H
#define CAMERA_H
#include "Vector.h"
#include "Point.h"
class Camera
{
public:
	Camera(int height, int width);
	~Camera() {};

	Point Position;
	float NearPlane;
	float FarPlane;
	float Fov;
	float Angle;
};

#endif