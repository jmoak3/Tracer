#include "Inlines.h"
#include "Shape.h"
#include "Point.h"
#include "Vector.h"
#include "Normal.h"
#include <fstream>

bool writeToTGA(int * pixels, int height, int width)
{
	std::ofstream outFile("Image", std::ios::binary);
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
			outFile.put((char)pixels[(y*width) + x+2]);
			outFile.put((char)pixels[(y*width) + x+1]);
			outFile.put((char)pixels[(y*width) + x]);
		}
	}
	outFile.close();
	return true;
}

int main(int argc, char * argv[])
{
	assert(argc > 1 && argc < 3);
	const int height = atoi(argv[0]);
	const int width = atoi(argv[1]);

	int ***pixels = new int **[height];
	for (int i = 0; i < height; ++i)
	{
		pixels[i] = new int *[width];
		for (int j = 0; j < width; ++j)
		{
			pixels[i][j] = new int[3];
		}
	}

	assert(writeToTGA((int*)pixels, height, width));
	return 0;
}