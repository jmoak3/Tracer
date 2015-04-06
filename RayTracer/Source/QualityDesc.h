#ifndef QUALITYDESC_H
#define QUALITYDESC_H

struct QualityDesc
{
	int Samples = 4;
	int LightSamples = 4;
	int GlossyReflectiveSamples = 4;
	int Depth = 1;

	//Makes it look better for less samples
	bool PathEnableDirectLighting = true;
	bool PathEnableIndirectIllum = true;
};

#endif