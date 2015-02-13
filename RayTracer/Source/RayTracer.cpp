#include "Inlines.h"
#include "Shape.h"
#include "Sphere.h"

#include <fstream>

//FIXME: Doesn't seem to work LOL
bool writeToTGA(RGB * pixels, int height, int width)
{
	std::ofstream outFile("Image.tga", std::ios::binary);
	if (!outFile) return false;

	unsigned char header[18] = { 0 };
	header[2] = 1;
	header[12] = width & 0xFF;
	header[13] = (width >> 8) & 0xFF;
	header[14] = height & 0xFF;
	header[15] = (height >> 8) & 0xFF;
	header[16] = 24;

	outFile.write((const char*)header, 18);

	for (int y = height - 1; y >= 0; --y)
	{
		for (int x = 0; x < width; ++x)
		{
			outFile.put((char)pixels[(y*width) + x].blue);
			outFile.put((char)pixels[(y*width) + x].green);
			outFile.put((char)pixels[(y*width) + x].red);
		}
	}
	outFile.close();
	return true;
}

int main(int argc, char * argv[])
{
	//Will be rewritten!!!!!!!!!! 
	//NEED TRACING OBJECT!!!!
	assert(argc > 2 && argc < 4);
	const int height = atoi(argv[1]);
	const int width = atoi(argv[2]);

	RGB color; color.red = 255; color.green = 0.f; color.blue = 0;
	RGB color2; color2.red = 0; color2.green = 255; color2.blue = 0;
	RGB bg; bg.red = 166; bg.green = 166; bg.blue = 166;
	Shape * sphere = new Sphere(Point(-100.f, 50.f, 400.f), color, 100.f);
	Shape * sphere2 = new Sphere(Point(0.f, 0.f, 200.f), color2, 50.f);
	Camera cam(height, width);

	printf("P6 %i %i 255 ", height, width);
	
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			RGB pixelColor;
			float wid2 = ((float)width / 2.f);
			float hei2 = ((float)height / 2.f);
			float x1 = (2.f*x/(float)width - 1.f);
			float y1 = (2.f*-y/(float)height + 1.f);
			//float x1 = (2.f * ((x + 0.5f) / width) - 1) * cam.Angle;
			//float y1 = (1.f - 2.f * ((y + 0.5f) / height)) * cam.Angle;
			float x2 = x1*1000.f;
			float y2 = y1*1000.f;
			Point p1 = Point(x1, y1, 0.f);
			Point p2 = Point(x2, y2, 1000.f);

			Vector dir = p2 - p1;
			Ray ray = Ray(p1, dir, 0.f);
			float hit1 = 0.f;
			float hit2 = 0.f;
			float eps = 0.f;
			if (sphere->Intersect(ray, &hit1, &eps))
				pixelColor = sphere->GetColor();
			if (sphere2->Intersect(ray, &hit2, &eps))
				pixelColor = sphere2->GetColor();
			if (hit1 < 0.01f && hit2 < 0.01f)
				pixelColor = bg;
			printf("%c%c%c", pixelColor.red, pixelColor.green, pixelColor.blue);
		}
	}

	//assert(writeToTGA((RGB*)pixels, height, width));
	return 0;
}