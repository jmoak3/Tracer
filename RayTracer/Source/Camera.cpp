#include "Camera.h"
#include <math.h>
#include "Point.h"

Camera::Camera(int height, int width)
{
	Position = Point(0.f, 0.f, 0.f);
	Fov = 90.f;
	//Angle = 2.f*atan(tan(Fov*2.f*3.14159f*0.002778f * 0.5)*(float)(width / height));
	Angle = atan((Fov * 0.5)*2.f*3.14159f*0.002778f);
	NearPlane = 0.1f;
	FarPlane = 1000.f;
}