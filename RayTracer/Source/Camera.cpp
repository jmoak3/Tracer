#include "Camera.h"
#include <math.h>
#include "Point.h"
#include "Transform.h"
#include <assert.h>

Camera::Camera(const Transform &proj, const int width, const int height, const float focalDist)
	: width(width), height(height), focalDist(focalDist)
{
	assert(focalDist == height && width == height);
	ScreenToWorld = proj;
	WorldToScreen = Inverse(proj);
	Transform RasterToScreenTranslation = Translate(Vector(-1.f, 1.f, 0.f));
	Transform RasterToScreenScale = Scale(2.f/(float)width, -2.f/(float)height, 0.f);
	RasterToScreen = RasterToScreenTranslation(RasterToScreenScale);
	RasterToWorld = ScreenToWorld(RasterToScreen);
	WorldToFarPlane = Translate(Vector(0.f,0.f,focalDist))
							(Scale(width/2.f,height/2.f, 0.f));
}