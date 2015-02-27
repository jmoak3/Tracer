#include "Camera.h"
#include <math.h>
#include "Point.h"
#include "Transform.h"

Camera::Camera(const Transform &proj)
{
	CameraToScreen = proj;
}