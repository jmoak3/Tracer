#ifndef QUALITYDESC_H
#define QUALITYDESC_H

struct QualityDesc
{
	int Samples = 4;
	int LightSamples = 4;
	int GlossyReflectiveSamples = 4;
	int Depth = 5;
};

#endif